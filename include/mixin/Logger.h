#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "PMessageHandler.h"

// A mix-in class intended for types that require logging functionality
namespace PSTD {

     class Logger {
     public:
		 Logger(void);
         Logger(MH_Config *mhconf, const char *prefix);
        virtual ~Logger(void);
        
        void Config_Logger(MH_Config *mhc, const char *prefix);
        void Toggle_Logging(bool flag);
		void Flush(void);
        void Set_DebugState(bool debugstate);
        
     protected:
		void Handle_LogMessage(MH_MessageTypes type, const char *format, ...) const;
		void Handle_LogMessage(MH_MessageTypes messageType, const char *format, va_list vargs);

		// used to filter log messages by severity
		unsigned int _LogLevel;

        bool _DebugFlag;
        bool _LoggingOn;

        unsigned int _LogChannel;

		// prefix appended to all log messages using this logger
        const char *_LogPrefix;
       
		int _StartMessageIndex;
        
		//  configuration packed for passing default config to nested logging objects
        MH_Config _ChildMHC;
        
		// Actually handles the appropriate routing of the log messages
        MessageHandler *_LoggingHandler;
  };

#define DEBUG(...) Handle_LogMessage(MH_DEBUG, __VA_ARGS__)
#define MESS(...) Handle_LogMessage(MH_MESSAGE, __VA_ARGS__)
#define WARN(...) Handle_LogMessage(MH_WARNING, __VA_ARGS__)
#define NFERR(...) Handle_LogMessage(MH_NONFATAL_ERROR, __VA_ARGS__)
#define FATERR(...) Handle_LogMessage(MH_FATAL_ERROR, __VA_ARGS__)


};


#endif
