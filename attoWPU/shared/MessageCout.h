#pragma once
#include "Message.h"
#include <iostream>

namespace attoWPU
{
	namespace shared
	{
		class MessageCout : public Message
		{
			void _msg(std::string text, MessageType type)
			{
				std::cout << text << std::endl;
			}
		public:
			MessageCout(bool debugVersion, char **messages, Message *forward = 0) :
			  Message("", "WARNING: ", "ERROR: ", "(DEBUG) ", debugVersion, messages, forward)
			  {
			  }

			MessageCout(std::string infoPref, std::string warningPref,
				std::string errorPref, std::string debugPref,
				bool debugVersion, char **messages, Message *forward = 0) :
				Message(infoPref, warningPref, errorPref, debugPref, debugVersion, messages, forward)
				{
				}
			
		};
	}
}