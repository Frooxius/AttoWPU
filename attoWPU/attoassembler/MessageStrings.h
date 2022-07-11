#pragma once

namespace attoWPU
{
	namespace assembler
	{
		extern char *msgstrings[]; 

		enum msgCode
		{
			msgAttoassembling,

			msgErrNumberEmpty,
			msgErrNumberUnexpectedSymbol,
			msgErrNumberSmallBase,
			msgErrNumberTooLarge,
			msgErrPrematureStatement,
			msgErrUndefSymbol,

			msgDbgStatementEnd
		};
	}
}