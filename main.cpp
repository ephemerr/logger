#include "src/logger.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    Logger::init();
    Logger::init();
    for (;;) {
      qDebug("Hey debug");
      qInfo("Hey info");
      qWarning("Hey warning");
      qCritical("Hey critical");
      QThread::sleep(2);
    }
    return app.exec();
}
