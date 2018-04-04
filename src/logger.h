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
private:
  QtMsgType level_{QtInfoMsg};
};

#define logDebug(msg) Logger::instance().log(msg,__FILE__,__LINE__,QtDebugMsg)
#define logInfo(msg)  Logger::instance().log(msg,__FILE__,__LINE__,QtInfoMsg)
#define logWarn(msg)  Logger::instance().log(msg,__FILE__,__LINE__,QtWarningMsg)
#define logFatal(msg) Logger::instance().log(msg,__FILE__,__LINE__,QtFatalMsg)

#define LOGGER_NO_DEBUG_MACRO

#if defined(LOGGER_NO_DEBUG_OUTPUT)
#  undef logDebug
#  define logDebug LOGGER_NO_DEBUG_MACRO
#endif
#if defined(LOGGER_NO_INFO_OUTPUT)
#  undef logInfo
#  define logInfo LOGGER_NO_DEBUG_MACRO
#endif
#if defined(LOGGER_NO_WARNING_OUTPUT)
#  undef logWarning
#  define logWarning LOGGER_NO_DEBUG_MACRO
#endif
