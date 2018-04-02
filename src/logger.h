#include <QObject>
#include <QDateTime>
#include <qlogging.h>

class LogContext : public QMessageLogContext  {
  public:
    LogContext(const char *fileName, int lineNumber, const QDateTime &dateTime);
    QDateTime datetime_;
};

class Logger : public QObject {
  Q_OBJECT
public:
  static Logger& instance() {
          static Logger theSingleInstance;
          return theSingleInstance;
  }
  void log(const char *msg,  const char *fileName, int lineNumber, const QtMsgType &);
  void setLevel(const QtMsgType &min);
  void setFormat(const QString &format);
signals:
  void message(const QString &msg, const LogContext &, QtMsgType);
};
