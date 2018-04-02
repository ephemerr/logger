#include "logger.h"

LogContext::LogContext(const char *fileName, int lineNumber, const QDateTime &dateTime)
  : QMessageLogContext(fileName,lineNumber,Q_NULLPTR,Q_NULLPTR)
  , datetime_(dateTime) {
}


void Logger::log(
    const char *msg,
    const char *fileName, int lineNumber, const QtMsgType &level) {
    LogContext context(fileName, lineNumber, QDateTime::currentDateTime());
    qt_message_output(level,context,msg);
    emit message(msg,context,level);
}
