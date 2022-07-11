#include "SourceFile.h"
#include "Exception.h"
#include <cstring>

namespace attoWPU
{
	namespace assembler
	{
		SourceFile::SourceFile(const char *filename)
		{
			// Load file, throw exception if it fails
			file = new std::ifstream(filename, std::ios::in | std::ios::binary);

			if(!*file)
			{
				char msg[300] = "Could not load ";
				strcat(msg, filename);
				throw Exception(msg);
			}

			std::string name("File: ");
			name += filename;

			file->get(nextch);
		}

		SourceFile::~SourceFile()
		{
			file->close();
			delete file;
		}

		char SourceFile::__read()
		{
			char ch = nextch;
			file->get(nextch);
			return ch;
		}

		bool SourceFile::Empty()
		{
			return file->eof();
		}
	}
}