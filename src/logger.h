#include <QObject>
#include <QFile>
#include <QFileSystemWatcher>
#include <qlogging.h>

class Logger : public QObject {
  Q_OBJECT
public:
  static void init();
  static void setLogPath(const QString &log_path);
  static void messageHandler(const QtMsgType type,
                             const QMessageLogContext &context,
                             const QString &msg);

  static void logRulesChanged(const QString &file_name);
private:
  static QFile log_;
};

#define logDebug    QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).debug
#define logInfo     QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).info
#define logWarning  QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).warning
#define logCritical QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).critical
#define logFatal    QMessageLogger(__FILE__,__LINE__, QT_MESSAGELOG_FUNC).fatal
