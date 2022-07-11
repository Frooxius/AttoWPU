#include "SourceQFile.h"
#include "attoWPUsrc/Exception.h"
#include <cstring>
#include <string>

namespace attoWPU
{
        namespace assembler
        {
                SourceQFile::SourceQFile(QString filename)
                {
                        // Load file, throw exception if it fails
                        file = new QFile(filename);

                        if(!file->open(QFile::ReadOnly))
                        {
                                char msg[300] = "Could not load ";
                                strcat(msg, filename.toLocal8Bit().data());
                                throw Exception(msg);
                        }

                        std::string name("File: ");
                        name += filename.toLocal8Bit().data();

                        file->read(&nextch, 1);
                }

                SourceQFile::~SourceQFile()
                {
                        file->close();
                        delete file;
                }

                char SourceQFile::__read()
                {
                        char ch = nextch;
                        file->read(&nextch, 1);
                        return ch;
                }

                bool SourceQFile::Empty()
                {
                        return file->atEnd();
                }
        }
}
