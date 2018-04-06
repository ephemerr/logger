#include <QObject>
#include <QFile>
#include <QFileSystemWatcher>

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


