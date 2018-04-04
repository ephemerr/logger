#include <QDebug>

#include "logger.h"

LogContext::LogContext(const char *fileName, int lineNumber, const QtMsgType& level)
  : QMessageLogContext(fileName,lineNumber,Q_NULLPTR,Q_NULLPTR)
  , datetime_(QDateTime::currentDateTime())
  , level_(level)
{
}

void LogContext::log(const char *msg, ...) const {
    switch (level_) {
      case QtDebugMsg:   qDebug()    << msg; break;
      case QtInfoMsg:    qInfo()     << msg; break;
      case QtWarningMsg: qWarning()  << msg; break;
      case QtFatalMsg:   qCritical() << msg; break;
    };

    Logger::instance().message(msg,this);
}
