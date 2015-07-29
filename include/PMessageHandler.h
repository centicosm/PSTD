// messagehandler.h

/***************************************************************************************************************
 * +Class+ MessageHandler: A class used to handle error, debug, and general logging information in a flexible 
 *                         manner.  It allocates a specified number of logging channels to be used by other
 *                         components.  It reserves a single error channel for logging its own error messages ]
 **************************************************************************************************************/


#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <stdio.h>
#include <vector>
#include <string>


#define MAX_LOG_MESSAGE_SIZE		256

namespace PSTD {

   /*******************************************************************************************
    * +Type+  MH_MessageTypes:  Types of messages that can be distinguished by message handler ]
    *         +T+ MH_FATAL_ERROR: Fatal error resulting in program termination ]
    *         +T+ MH_NONFATAL_ERROR: Recoverable error ]
    *         +T+ MH_WARNING: Warning message ]
    *         +T+ MH_MESSAGE: General Message ]
    *******************************************************************************************/
   enum MH_MessageTypes { 
		MH_FATAL_ERROR = 16,
		MH_NONFATAL_ERROR = 8,
		MH_WARNING = 4,
		MH_MESSAGE = 2,
		MH_DEBUG = 1
   };
   
   /************************************************************************************
    * +Type+  MH_ChannelOutput:  Types of output available to a logging channel ]
    *         +T+ MH_OUTPUT_NONE: No Output ]
    *         +T+ MH_OUTPUT_CONSOLE: Output to user defined console ]
    *         +T+ MH_OUTPUT_FILE_NEW: Output to a file that overwrites previous log ]
    *         +T+ MH_OUTPUT_APPEND: Output to end of a log ]
    *         +T+ MH_OUTPUT_FILE_BACKUP: Output to a file, backing up old log ]
    *         +T+ MH_OUTPUT_TERMINAL: Output to stdout ]
    *         +T+ MH_OUTPUT_STDERR: Output to stderr ]
    ************************************************************************************/
   enum MH_ChannelOutput {
      MH_OUTPUT_NONE,
      MH_OUTPUT_CONSOLE,
      MH_OUTPUT_FILE_NEW,
      MH_OUTPUT_FILE_APPEND,
      MH_OUTPUT_FILE_BACKUP,
      MH_OUTPUT_TERMINAL,
      MH_OUTPUT_STDERR
   };
   

   struct ChannelConfig {
      ChannelConfig(const char *fileName, FILE *fh, MH_ChannelOutput outType) : _FileName(fileName), _FileHandle(fh), _OutputType(outType) {};
      ChannelConfig(void) : _FileName(""), _FileHandle(NULL), _OutputType(MH_OUTPUT_NONE) {};
      std::string _FileName;
      FILE *_FileHandle;
      MH_ChannelOutput _OutputType;
   };


   class MessageHandler {
   public:
      MessageHandler(char *logfile);
      ~MessageHandler(void);
      void Send_Message(std::string &message, MH_MessageTypes messageType, unsigned int channel);

	  void Send_Message(MH_MessageTypes messageType, unsigned int channel, const char *parent, const char *prefix, const char *format, va_list vargs);
	  void Send_Message(MH_MessageTypes messageType, unsigned int channel, const char *prefix, const char *format, va_list vargs);

      bool Redirect_Channel(unsigned int channel, MH_ChannelOutput output);
      MH_ChannelOutput Get_ChannelOutput(unsigned int channel);
      int Get_Channel(char *filename, MH_ChannelOutput co = MH_OUTPUT_NONE);
	  void Flush(unsigned int channel);
   
	   private:

      std::vector<ChannelConfig> _Channels;
   };
   

   struct MH_Config {
      MessageHandler *_MH;
      int _Channel;
      const char *_Prefix;
	  unsigned int _LogLevel;
	  bool _DebugFlag;
	  bool _LoggingOn;
   };
 
};



#endif
																		    
