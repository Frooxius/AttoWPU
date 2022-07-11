#include <iostream>
#include "Exception.h"
#include "Source.h"
#include "SourceFile.h"
#include "SourceString.h"
#include "AttocodeWriteMem.h"
#include "MessageDummy.h"
#include "MessageCout.h"
#include "Attoasm.h"
#include "MessageStrings.h"
#include <fstream>
#include <string>
using namespace std;

using namespace attoWPU::assembler;

void main(int argc, char *argv[])
{
	cout << "attoAssembler 0.8 alpha, written by Tomas \"Frooxius\" Mariancik" << endl;
	cout << "Please report any errors to tomas.mariancik@gmail.com (include the source)" << endl;
	cout << "Visit http://attowpu.solirax.org for more information" << endl;
	cout << "------------------------------------" << endl;
	try{
#if DEBUG == 0
		if(argc == 1)
			throw Exception("No input file.");
#endif
		AttocodeWriteMem mem = AttocodeWriteMem(16*1024*1024);
#if DEBUG == 1
		AttoASM a = AttoASM(new SourceFile("test.att"), &mem, new attoWPU::shared::MessageCout(DEBUG, msgstrings));
#else
		AttoASM a = AttoASM(new SourceFile(argv[1]), &mem, new attoWPU::shared::MessageCout(DEBUG, msgstrings));
#endif
		while(!a.Assembly(100000))
			cout << '.';

#if DEBUG == 0
		char hexfilename[255];
		strcpy(hexfilename, argv[1]);

		char *p = strstr(hexfilename, ".att");
		if(p)
			for(int i = 0; i < 4; i++)
				*(p++) = ".hex"[i];
		else
			strcat(hexfilename, ".hex");

#endif
#if DEBUG == 1
		char hexfilename[] = "test.hex";
#endif

		mem.SaveToFile(hexfilename);
		cout << " Done, saved as " << hexfilename;
	} catch(Exception e)
	{
		cout << e.GetMessage();
	}
	catch(...)
	{
		cout << "Unexpected error.";
	}

}