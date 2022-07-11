#pragma once
#include "attoWPUsrc/SourceRead.h"
#include <QFile>

//	This class allows reading a source code from a text file on the disk

namespace attoWPU
{
        namespace assembler
        {
                class SourceQFile : public SourceRead
                {
                private:
                        QFile *file;
                        char nextch;

                protected:
                        virtual char __read();		// reading a single character

                public:
                        virtual bool Empty();
                        SourceQFile(QString filename);	// constructor, which is also used to specify a file
                        ~SourceQFile();	// destructor used to close the file stream
                };
        }
}
