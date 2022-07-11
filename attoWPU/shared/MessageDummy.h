#pragma once
#include "Message.h"
#include <iostream>

namespace attoWPU
{
	namespace shared
	{
		class MessageDummy : public Message
		{
			void _msg(std::string text, MessageType type)
			{
				
			}
		public:
			MessageDummy(bool debugVersion, char **messages = 0, Message *forward = 0) :
			  Message("", "WARNING: ", "ERROR: ", "(DEBUG) ", debugVersion, messages, forward)
			  {
			  }

			MessageDummy(std::string infoPref, std::string warningPref,
				std::string errorPref, std::string debugPref,
				bool debugVersion, char **messages = 0, Message *forward = 0) :
				Message(infoPref, warningPref, errorPref, debugPref, debugVersion, messages, forward)
				{
				}
			
		};
	}
}