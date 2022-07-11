#include "Message.h"

namespace attoWPU
{
	namespace shared
	{
		Message::Message(std::string infoPref, std::string warningPref,
				std::string errorPref, std::string debugPref,
				bool debugVersion, char **messages, Message *forward)
		{
			infoPrefix = infoPref;
			warningPrefix = warningPref;
			errorPrefix = errorPref;
			debugPrefix = debugPref;
			debugver = debugVersion;
			_forward = forward;
			_messages = messages;
		}

		void Message::Send(unsigned int msg, MessageType type, bool debug)
		{
            if(_messages)
				Send(std::string(_messages[msg]), type, debug);
		}

		void Message::Send(std::string msg, MessageType type, bool debug)
		{
			// ignore debug messages in nondebug version
			if(debug && !debugver)
				return;

			std::string pref;
			if(debugver)
				pref += debugPrefix;
			switch(type)
			{
			case msgInfo:
				pref += infoPrefix;
				break;
			case msgWarning:
				pref += warningPrefix;
				break;
			case msgError:
				pref += errorPrefix;
				break;
			default:
				pref += " UNKNOWN MESSAGE TYPE ";
			}

			this->_msg(pref + msg, type);

			if(_forward)
				_forward->Send(msg, type, debug);
		}
	}
}