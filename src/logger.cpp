#include <QDebug>
#include <QLoggingCategory>
#include <QColor>
#include <QThread>
#include <QCoreApplication>
#include <QFileInfo>

#include "logger.h"

#define COLOR_NORMAL  "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_GREY    "\033[39m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"
#define COLOR_MAGENTA "\033[35m"

QFile Logger::log_;

void Logger::setLogPath(const QString &log_path) {
  if (log_path.isEmpty())
    return;

  log_.setFileName(log_path);
  if (!log_.open(QIODevice::WriteOnly | QIODevice::Append)) {
    fprintf(stderr, "could not open(%s): %s\n",
            qPrintable(log_.fileName()), qPrintable(log_.errorString()));
  }
}

static QFileSystemWatcher log_rules_watcher_;
static QThread thread_;

void Logger::init() {
  Logger::setLogPath("./default.log");
  qInstallMessageHandler(Logger::messageHandler);

  if (!qEnvironmentVariableIsSet("QT_MESSAGE_PATTERN"))
    qSetMessagePattern("[%{time dd-MM-yyyy HH:mm:ss:zzz}]%CB[%{file}:%{line}]%CE%T: %{message}");

  if (qEnvironmentVariableIsSet("QT_LOGGING_CONF")) {
    Logger::logRulesChanged(qgetenv("QT_LOGGING_CONF"));
    qunsetenv("QT_LOGGING_CONF");
  } else
    Logger::logRulesChanged("./log_rules.ini");

  log_rules_watcher_.moveToThread(&thread_);
  thread_.start();

  connect(&log_rules_watcher_, &QFileSystemWatcher::fileChanged,
          &Logger::logRulesChanged);
}

void colorize(
    const QtMsgType type, QString &log_msg) {
  switch (type) {
    case QtDebugMsg:
      log_msg.replace("%CB", COLOR_NORMAL);
      break;

    case QtWarningMsg:
      log_msg.replace("%CB", COLOR_YELLOW);
      break;

    case QtCriticalMsg:
      log_msg.replace("%CB", COLOR_RED);
      break;

    case QtFatalMsg:
      log_msg.replace("%CB", COLOR_MAGENTA);
      break;

    /*case QtInfoMsg:*/
    default:
      log_msg.replace("%CB", COLOR_CYAN);
      break;
  }
  log_msg.replace("%CE", COLOR_NORMAL);

  auto c_thread = QThread::currentThread();
  if (c_thread == qApp->thread())
    log_msg.replace("%T", COLOR_GREEN "(main)" COLOR_NORMAL);
  else
    log_msg.replace("%T", QString(COLOR_MAGENTA "(%1)" COLOR_NORMAL).arg(c_thread->objectName()));
}

void Logger::messageHandler(
    const QtMsgType type, const QMessageLogContext &context,
    const QString &msg) {
  QTextStream out(stdout);
  QString log_msg = qFormatLogMessage(type, context, msg);

  colorize(type, log_msg);

  QTextStream(stdout) << log_msg << endl;

  if (log_.isOpen()) {
    out.setDevice(&log_);

    // 5242880 = 5 * 1024 * 1024
    if (log_.size() >= 5242880) {
      if (QFile::exists(log_.fileName().append(".backup")))
          QFile::remove(log_.fileName().append(".backup"));

      if (!QFile::copy(log_.fileName(), log_.fileName().append(".backup")))
        fprintf(stderr, "log does not copy\n");

      log_.resize(0);
    }
    out << log_msg << endl;
  }
}

void Logger::logRulesChanged(const QString &file_name) {
  qDebug() << file_name;

  QFile f(file_name);
  while (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QThread::msleep(10);
  }
  log_rules_watcher_.addPath(file_name);

  QByteArray rules;
  while (!f.atEnd()) {
    const auto &line = f.readLine();
    if (line.contains("Rules") || line.contains(";"))
      continue;

    rules.append(line);
  }

  QLoggingCategory::setFilterRules(rules);
  f.close();
}

