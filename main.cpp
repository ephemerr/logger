#include "src/logger.h"

int main() {
  Logger::instance().log("Hello",__FILE__,__LINE__,QtDebugMsg);
}
