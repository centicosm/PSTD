#include <string>
#include <iostream>
#include "mixin/logger.h"
#include <stdio.h>
#include <cstdio>
#include <cstdarg>
#include <stdlib.h>  
#include <Windows.h>

#ifdef _MSC_VER
#define  snprintf	_snprintf_s
#endif


using namespace std;
using namespace PSTD;

Logger::Logger(void) :
_LogPrefix(NULL)
{
	//_LogPrefix = NULL;
	Config_Logger(NULL, NULL);
}

Logger::Logger(MH_Config *mhConf, const char *prefix) {
  Config_Logger(mhConf, prefix);
}

Logger::~Logger(void) {
	delete[]_LogPrefix;
}

void Logger::Config_Logger(MH_Config *mhc, const char *prefix) {
  
  if (mhc != NULL) {
    _LoggingHandler = mhc->_MH;
    _LogChannel = mhc->_Channel;
	_DebugFlag = mhc->_DebugFlag;
	_LogLevel = mhc->_LogLevel;
	_LoggingOn = mhc->_LoggingOn;

	// child prefix will be -  :Parents:Self:
	size_t prefixSize = strlen(prefix) + strlen(mhc->_Prefix) + 1;
	_LogPrefix = new char[prefixSize];
	snprintf((char *)_LogPrefix, prefixSize, _TRUNCATE, "%s%s", mhc->_Prefix, prefix);
  }
  else {
    _LoggingHandler = NULL;
    _LogChannel = -1;
	_DebugFlag = false;
	_LoggingOn = false;
	_LogLevel = 0xFFFF;
	_LogPrefix = NULL;
  }


  _ChildMHC._MH = _LoggingHandler;
  _ChildMHC._Channel = _LogChannel;
  _ChildMHC._Prefix = _LogPrefix;
  _ChildMHC._LogLevel = _LogLevel;
  _ChildMHC._DebugFlag = _DebugFlag;
  _ChildMHC._LoggingOn = _LoggingOn;

}

void Logger::Toggle_Logging(bool flag) {
  
  if (flag) {
    if (_LoggingHandler != NULL) {
      _LoggingOn = true;
    }
  }
  else {
    _LoggingOn = false;
  }
}

void Logger::Set_DebugState(bool debugState) {
  _DebugFlag = debugState;
}



void Logger::Handle_LogMessage(MH_MessageTypes type, const char *format, ...) const {

	if ((_LoggingOn) && (type & _LogLevel)) {

		// Message display - ":LogPrefix:" + Log message
		va_list vargs;
		va_start(vargs, format);
		_LoggingHandler->Send_Message(type, _LogChannel, _LogPrefix, format, vargs);
	}
	else {

		// and bail if it is a fatal error
		if (type == MH_FATAL_ERROR) {
			va_list vargs;
			va_start(vargs, format);
			vfprintf(stderr, format, vargs);
			va_end(vargs);
			exit(1);
		}
	}
}

void Logger::Handle_LogMessage(MH_MessageTypes messageType, const char *format, va_list vargs) {
	if ((_LoggingOn) && (messageType & _LogLevel)) {

		// Message display - ":LogPrefix:" + Log message
		_LoggingHandler->Send_Message(messageType, _LogChannel, _LogPrefix, format, vargs);
	}
	else {

		// and bail if it is a fatal error
		if (messageType == MH_FATAL_ERROR) {
			vfprintf(stderr, format, vargs);
			va_end(vargs);
			exit(1);
		}
	}
}


void Logger::Flush(void) {
	_LoggingHandler->Flush(_LogChannel);
}
