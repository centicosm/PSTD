#pragma once

#ifndef GLOBAL_LOGGER_H
#define GLOBAL_LOGGER_H

#include <string>
#include <cstdarg>
#include "mixin/logger.h"

namespace PSTD {

	class GlobalLogger : public Logger {
	public:
		~GlobalLogger() {};

		static void Log(MH_MessageTypes type, const char *format, ...) {
			if (!_Instance) {
				_Instance = new GlobalLogger();
			}

			va_list vargs;
			va_start(vargs, format);
			_Instance->Handle_LogMessage(type, format, vargs);
		}
			
		static void Redirect_Global(MH_ChannelOutput output) {
			_Handler->Redirect_Channel(_GlobChannel, output);
		}

		static void FlushGlobal(void) {
			if (!_Instance) {
				_Instance = new GlobalLogger();
			}
			_Instance->Flush();
		}

		static MessageHandler *Get_MessageHandler(void) {
			if (!_Handler) {
				_Handler = _Handler = new MessageHandler("Handler.log");
			}
			return _Handler;
		}
		

	private:

		const char *_Name = ":Global:";

		GlobalLogger()  {
			_LogChannel = Get_MessageHandler()->Get_Channel("Global.log", MH_OUTPUT_TERMINAL);
			_GlobChannel = _LogChannel;
			_LogPrefix = _Name;
			_LoggingOn = true;
			_LogLevel = 0xFFFF;
			_LoggingHandler = Get_MessageHandler();
			_GlobChannel = _LogChannel;
		};

		static unsigned int _GlobChannel;
		static GlobalLogger *_Instance;
		static MessageHandler *_Handler;
	};

};




#define GLDEBUG(...) PSTD::GlobalLogger::Log(MH_DEBUG, __VA_ARGS__)
#define GLMESS(...) PSTD::GlobalLogger::Log(MH_MESSAGE, __VA_ARGS__)
#define GLWARN(...) PSTD::GlobalLogger::Log(MH_WARNING, __VA_ARGS__)
#define GLNFERR(...) PSTD::GlobalLogger::Log(MH_NONFATAL_ERROR, __VA_ARGS__)
#define GLFATERR(...) PSTD::GlobalLogger::Log(MH_FATAL_ERROR, __VA_ARGS__)
#define GLFLUSHLOG PSTD::GlobalLogger::FlushGlobal

#endif

