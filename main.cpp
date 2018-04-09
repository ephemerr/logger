#include "src/logger.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    Logger::init();
    for (;;) {
      logDebug("Hey debug");
      logInfo("Hey info");
      logWarning("Hey warning");
      logCritical("Hey critical");
      QThread::sleep(2);
    }
    return app.exec();
}
