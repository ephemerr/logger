#include "logger.h"

LogContext::LogContext(const char *fileName, int lineNumber, const QDateTime &dateTime)
  : QMessageLogContext(fileName,lineNumber,Q_NULLPTR,Q_NULLPTR)
  , datetime_(dateTime) {
}


void Logger::setLevel(const QtMsgType &level) {
  level_ = level;
}

// Q_NEVER_INLINE
// static QString qt_message(QtMsgType msgType, const QMessageLogContext &context, const char *msg, va_list ap)
// {
//     QString buf = QString::vasprintf(msg, ap);
//     qt_message_print(msgType, context, buf);
//     return buf;
// }
//
//     va_list ap;
//     va_start(ap, msg); // use variable arg list
//     const QString message = qt_message(QtInfoMsg, context, msg, ap);
//     va_end(ap);

void Logger::log(
    const char *msg,
    const char *fileName, int lineNumber, const QtMsgType &level) {
    if (level < level_) return;
    LogContext context(fileName, lineNumber, QDateTime::currentDateTime());
    qt_message_output(level,context,msg);
    emit message(msg,context,level);
}
