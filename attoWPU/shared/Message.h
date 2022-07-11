#pragma once
#include <string>

/*
	This class provides abstract interface for sending messages to
	unspecified output. The nature of the output is implemented by
	the derived classes.
*/

namespace attoWPU
{
	namespace shared
	{
		enum MessageType
		{
			msgInfo, msgWarning, msgError
		};

		class Message
		{
			bool debugver;	// determines whether to show debug messages or not
			std::string	infoPrefix, warningPrefix, errorPrefix, debugPrefix;
			Message *_forward;

			char **_messages;	// pointer to an array of built-in messages

			virtual void _msg(std::string text, MessageType type) = 0;

		public:
			Message(std::string infoPref, std::string warningPref,
				std::string errorPref, std::string debugPref,
				bool debugVersion, char **messages = 0, Message *forward = 0);

			virtual ~Message() { }

			void Send(std::string msg, MessageType type, bool debug = false);
			void Send(unsigned int msg, MessageType type, bool debug = false);
		};
	}
}