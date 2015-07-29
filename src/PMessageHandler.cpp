// messagehandler.cpp

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdarg>

#include "pstd_util.h"
#include "PMessageHandler.h"

using namespace std;
using namespace PSTD;

#ifdef _MSC_VER
#define  snprintf	_snprintf_s
#define  vsnprintf	_vsnprintf_s
#endif


/*****************************************************************************
 * +CM+ MessageHandler(int maxchan, char *logname): Constructor which  allocates file
 *            channels and set up initial channel allocations ] 
 * Parameters:  maxchan (int): The maximum number of channels to allocate ]
 *              logname (char *): The name of the reserved logfile ]
 *****************************************************************************/
MessageHandler::MessageHandler(char *logName) {

   // initialize the reserved logging channel (for logger errors)
   ChannelConfig cConf(logName, NULL, MH_OUTPUT_NONE);
   _Channels.push_back(cConf);
   Redirect_Channel(0, MH_OUTPUT_NONE);
}


/******************************************************************************
 * +CM+ ~MessageHandler(void): Deconstructor to close open files and free memory ]
 *****************************************************************************/
MessageHandler::~MessageHandler(void) {
   
   // close all the open file channels and send them a shut down message
   size_t numChannel = _Channels.size();
   for (int cnt = 0; cnt < numChannel; cnt++) {
      std::string tempMess("\n---------------------- Message Logger Shut Down ----------------\n\n");
      Send_Message(tempMess, MH_MESSAGE, cnt);
      if (_Channels[cnt]._FileHandle) {
         fclose(_Channels[cnt]._FileHandle);
      }
   }
}

/***************************************************************************************
 * +CM+ void Redirect_Channel(int channel, MH_ChannelOutput output): Change the output of a given
 *               channel to a new type ]
 * Parameters:  channel (int): The channel to modify (1 to NumChannel) ]
 *              output (^MH_ChannelOutput): The output type ]
 * Returns: (bool): Success of redirect ]
 ***************************************************************************************/
bool MessageHandler::Redirect_Channel(unsigned int channel, MH_ChannelOutput output) {
   
   // handle parameter error conditions
   if (channel >= _Channels.size()) {
      std::string tempMess = "MessageHandler->Redirect_Channel: (Invalid Channel Number) - " + to_string(channel) + "\n";
      Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
      return false;
   }
   
   std::string tempMess("\n---------------------- Message Handler Redirected ----------------\n\n");
   Send_Message(tempMess, MH_MESSAGE, channel);
   
   // close the output file if needed
   if (_Channels[channel]._FileHandle != NULL) {
      fclose(_Channels[channel]._FileHandle);
      _Channels[channel]._FileHandle = NULL;
   }
   
   // start a new file
   if (output == MH_OUTPUT_FILE_NEW) {
      _Channels[channel]._FileHandle = fopen( _Channels[channel]._FileName.c_str(), "w");
      if (! _Channels[channel]._FileHandle) {
         tempMess = "MassageHandler->Redirect_Channel: (Can't open file for write): " + _Channels[channel]._FileName + "\n";
         Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
         return false;
      }
   }
 
   // append to an old file of create a new one if no old file exists
   else if (output == MH_OUTPUT_FILE_APPEND) {
      _Channels[channel]._FileHandle = fopen( _Channels[channel]._FileName.c_str(), "a+");
      if (!_Channels[channel]._FileHandle) {
         tempMess = "MassageHandler->Redirect_Channel: (Can't open file for append): " + _Channels[channel]._FileName + "\n";
         Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
         return false;
      }
   }
  
   // make a backup of the currently existing file an start a new one
   else if (output == MH_OUTPUT_FILE_BACKUP) {
      char tempChar;

      // check to see if the old file exists
      FILE *tempFile = fopen(_Channels[channel]._FileName.c_str(), "r");
    
      // it does so make a copy of it postfixed with .bak
      if (tempFile) {
         std::string backupFilename = _Channels[channel]._FileName + ".bak";
         FILE *tempFile2 = fopen(backupFilename.c_str(), "w");
         if (!tempFile2) {
            tempMess = "MassageHandler->Redirect_Channel: (Can't backup file): " + _Channels[channel]._FileName + "\n";
            Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
            return false;
         }
         else {
            do {
               tempChar = fgetc(tempFile);
               if (tempChar != EOF) {
	               fputc(tempChar, tempFile2);
               }
            } while (tempChar != EOF);
            fclose(tempFile);
            fclose(tempFile2);
         }
      }
      
      _Channels[channel]._FileHandle = fopen(_Channels[channel]._FileName.c_str(), "w");
      if (!_Channels[channel]._FileHandle) {
         tempMess = "MassageHandler->Redirect_Channel: (Can't open file for write): " + _Channels[channel]._FileName + "\n";
         Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
         return false;
      }
   }
  
   _Channels[channel]._OutputType = output;
   tempMess = "\n---------------------- Message Handling Started ----------------\n\n";
   Send_Message(tempMess, MH_MESSAGE, channel);
 
  
   return true;
}	      

/*******************************************************************************************************
 * +CM+ void Send_Message(char *message, MH_MessageTypes message_type, int channel): Sends
 *               the given message to the appropriate location and does appropriate error handling ]
 * Parameters: message (char *): Message to handle ]
 *             message_type (^MH_MessageTypes): Type of message ]
 *             channel (int): The channel to send the message ]
 * *****************************************************************************************************/
void MessageHandler::Send_Message(std::string &message, MH_MessageTypes messageType, unsigned int channel) {

   std::string log_message("");

   if (channel >= _Channels.size()) {
      std::string tempMess("");
      tempMess = "MassageHandler->Send_Message: (Invalid channel number): " + to_string(channel) + "\n";
      Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
      return;
   }

   // setup the error message
   switch (messageType) {
   case MH_NONFATAL_ERROR:
      log_message = "ERROR - ";
      break;
    
   case MH_FATAL_ERROR:
      log_message = "FATAL ERROR - ";
      break;
    
   case MH_WARNING:
      log_message = "WARNING - ";
      break;
    
   case MH_MESSAGE:
      log_message = "";
      message += "\n";
      break;
    
   default:
      return;
   }	    

   log_message += message;
  
   // handle the output types
   switch (_Channels[channel]._OutputType) {
    
   case MH_OUTPUT_NONE:
      // NOTHING
      break;
    
   case MH_OUTPUT_CONSOLE:
      // NOTHING FOR NOW!  add a function pointer?
      break;
    
   case MH_OUTPUT_TERMINAL:
      printf("%s", log_message.c_str());
      break;

   case MH_OUTPUT_STDERR:
      fprintf(stderr, "%s", log_message.c_str());
      break;

   case MH_OUTPUT_FILE_NEW:
   case MH_OUTPUT_FILE_APPEND:
   case MH_OUTPUT_FILE_BACKUP:
      fprintf(_Channels[channel]._FileHandle, "%s", log_message.c_str());
      break;
   }
  
   if (messageType == MH_FATAL_ERROR) {
      exit(1);
   }
}

/*
void MessageHandler::Send_Message(MH_MessageTypes messageType, unsigned int channel, const char *parent, const char *prefix, const char *format, va_list vargs) {
	std::string log_message("");

	if (channel >= _Channels.size()) {
		std::string tempMess("");
		tempMess = "MassageHandler->Send_Message: (Invalid channel number): " + to_string(channel) + "\n";
		Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
		return;
	}

	const char *mess;

	// setup the error message
	switch (messageType) {
	case MH_NONFATAL_ERROR:
		mess = "ERROR - ";
		break;

	case MH_FATAL_ERROR:
		mess = "FATAL ERROR - ";
		break;

	case MH_WARNING:
		mess = "WARNING - ";
		break;

	case MH_MESSAGE:
		mess = "";
		break;

	case MH_DEBUG:
		mess = "DEBUG - ";
		break;

	default:
		return;
	}

	char buf[MAX_LOG_MESSAGE_SIZE];
	int bufUsed = snprintf(buf, MAX_LOG_MESSAGE_SIZE, "%s%s%s", mess, parent, prefix);
	if (bufUsed == -1) {
		snprintf(buf, MAX_LOG_MESSAGE_SIZE, _TRUNCATE, "Log message truncated");
	}
	else {
		vsnprintf(&buf[bufUsed], MAX_LOG_MESSAGE_SIZE - bufUsed, _TRUNCATE, format, vargs);
	}
	
	// handle the output types
	switch (_Channels[channel]._OutputType) {

	case MH_OUTPUT_NONE:
		// NOTHING
		break;

	case MH_OUTPUT_CONSOLE:
		// NOTHING FOR NOW!  add a function pointer?
		break;

	case MH_OUTPUT_TERMINAL:
		printf("%s\n", buf);
		break;

	case MH_OUTPUT_STDERR:
		fprintf(stderr, "%s\n", buf);
		break;

	case MH_OUTPUT_FILE_NEW:
	case MH_OUTPUT_FILE_APPEND:
	case MH_OUTPUT_FILE_BACKUP:
		fprintf(_Channels[channel]._FileHandle, "%s\n", buf);
		break;
	}

	if (messageType == MH_FATAL_ERROR) {
		exit(1);
	}
}
*/


void MessageHandler::Send_Message(MH_MessageTypes messageType, unsigned int channel, const char *prefix, const char *format, va_list vargs) {

	if (channel >= _Channels.size()) {
	//	tempMess = "MassageHandler->Send_Message: (Invalid channel number): " + to_string(channel) + "\n";
		Send_Message(string("MassageHandler->Send_Message: (Invalid channel number): ") + to_string(channel) + "\n", MH_NONFATAL_ERROR, 0);
		return;
	}

	const char *mess;

	// setup the error message
	switch (messageType) {
	case MH_NONFATAL_ERROR:
		mess = "ERROR - ";
		break;

	case MH_FATAL_ERROR:
		mess = "FATAL ERROR - ";
		break;

	case MH_WARNING:
		mess = "WARNING - ";
		break;

	case MH_MESSAGE:
		mess = "MESS";
		break;

	case MH_DEBUG:
		mess = "DEBUG - ";
		break;

	default:
		return;
	}

	char buf[MAX_LOG_MESSAGE_SIZE];
	
	int bufUsed = snprintf(buf, MAX_LOG_MESSAGE_SIZE, "%s%s", mess, prefix);

	if (bufUsed == -1) {
		snprintf(buf, MAX_LOG_MESSAGE_SIZE, _TRUNCATE, "Log message truncated");
	}
	else {
		vsnprintf(&buf[bufUsed], MAX_LOG_MESSAGE_SIZE - bufUsed, _TRUNCATE, format, vargs);
	}

	// handle the output types
	switch (_Channels[channel]._OutputType) {

	case MH_OUTPUT_NONE:
		// NOTHING
		break;

	case MH_OUTPUT_CONSOLE:
		// NOTHING FOR NOW!  add a function pointer?
		break;

	case MH_OUTPUT_TERMINAL:
		printf("%s\n", buf);
		break;

	case MH_OUTPUT_STDERR:
		fprintf(stderr, "%s\n", buf);
		break;

	case MH_OUTPUT_FILE_NEW:
	case MH_OUTPUT_FILE_APPEND:
	case MH_OUTPUT_FILE_BACKUP:
		fprintf(_Channels[channel]._FileHandle, "%s\n", buf);
		break;
	}

	if (messageType == MH_FATAL_ERROR) {
		exit(1);
	}
}




/*******************************************************************************************
 * +CM+ MH_ChannelOutput Get_ChannelOutput(int channel): Returns the type of output used by a channel ]
 * Parameters: channel (int): Channel of inquiry ]
 * Returns: (^MH_ChannelOutput): The type of output used by the channel ]
 ******************************************************************************************/
MH_ChannelOutput MessageHandler::Get_ChannelOutput(unsigned int channel){
   
   if (_Channels.size() <= channel) {
      return MH_OUTPUT_NONE;
   }

   return _Channels[channel]._OutputType;
}


/*****************************************************************************************************
 * +CM+ int Get_Channel(char *filename, MH_ChannelOutput co): Allocates a channel a channel with a log filename ]
 *    Parameters:  filename (char *): Filename to use for a particular channel ]
 *                 co (^MH_ChannelOutput): Type of output to use for channel ]
 *    Returns: (int): The channel number allocated ]
 *****************************************************************************************************/
int MessageHandler::Get_Channel(char *fileName, MH_ChannelOutput co) {

   // make sure filename is valid
   if (fileName == NULL) {
      std::string tempMess = "MessageHandler->Add_Channel: (Invalid filename) - NULL";
      Send_Message(tempMess, MH_NONFATAL_ERROR, 0);
      return -1;
   }
  
   ChannelConfig cConf(fileName, NULL, MH_OUTPUT_NONE);
   _Channels.push_back(cConf);

   size_t channelNum = _Channels.size() - 1;
   Redirect_Channel((unsigned int)channelNum, co);
   return (int)channelNum;
}

void MessageHandler::Flush(unsigned int channel) {
	if ((_Channels[channel]._OutputType == MH_OUTPUT_FILE_APPEND) || (_Channels[channel]._OutputType == MH_OUTPUT_FILE_BACKUP) || (_Channels[channel]._OutputType == MH_OUTPUT_FILE_NEW)) {
		fflush(_Channels[channel]._FileHandle);
	}
}


