#include <string>
#include "GlobalLogger.h"

using namespace PSTD;

GlobalLogger *GlobalLogger::_Instance = NULL;
MessageHandler *GlobalLogger::_Handler = NULL;
unsigned int GlobalLogger::_GlobChannel = 0;