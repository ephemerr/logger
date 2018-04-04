#include <QObject>
#include <QDateTime>
#include <qlogging.h>

class LogContext : public QMessageLogContext  {
  public:
    LogContext(const char *fileName, int lineNumber, const QtMsgType& );
    void log(const char *msg, ...) const Q_ATTRIBUTE_FORMAT_PRINTF(2, 3);
  private:
    QDateTime datetime_;
    QtMsgType level_{QtInfoMsg};
};

class Logger : public QObject {
  Q_OBJECT
public:
  static Logger& instance() {
          static Logger theSingleInstance;
          return theSingleInstance;
  }
  void setLevel(const QtMsgType &level) {level_ = level;};
  const QtMsgType& getLevel() {return level_;};
  void setFormat(const QString &);
signals:
  void message(const QString &msg, const LogContext *);
private:
  QtMsgType level_{QtInfoMsg};
};

#define logDebug LogContext(__FILE__,__LINE__,QtDebugMsg).log
#define logInfo  LogContext(__FILE__,__LINE__,QtInfoMsg).log
#define logWarn  LogContext(__FILE__,__LINE__,QtWarningMsg).log
#define logFatal LogContext(__FILE__,__LINE__,QtFatalMsg).log

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
