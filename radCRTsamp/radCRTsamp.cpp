
/*START OF SPECIFICATIONS***************************************************
*
* Projec Name:  radCRTsamp - Sample code showing use of C runtime library
*                            functions. 
*
* File Name: radCRTsamp.cpp
*
* DESCRIPTION: This file contains code to call some of the commonly used 
*              C runtime library functions.  This is just practicer for me
*              after a long lay off from code writing.  Starting simple.
*
* Copyright (C) <2017> <Robert A. Davis>
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the RAD license.  See the LICENSE,TXT file for details.
*
* FUNCTIONS/ACTIONS IN
* THIS FILE:
*
* TestThread              - simple threadfunction.
* LaunchThreads           - Function to launch some threads
* TestSpawn		          - Function to spawn this executable and wait for it to end.
* TestSystemFunction      - Executes the dir system command 
* PlayWithFiles           - Function to test the use of the file related functions.
* PlayWithStrings         - Funtion to test the use of string processing functions.
*
* CHANGE LOG:
*
* DATE       FLAG	       Ref. No.	    CHANGE DESCRIPTION
* ----       ----------    --------	    -------------------------------------
* 02/09/17   V10RAD00 	                Initial coding
*
*END OF SPECIFICATIONS*******************************************************/


#include "stdafx.h"
#include <stdlib.h>
#include <process.h>
#include <tchar.h> 
#include <string.h>

/*
** TestThread - Thread function that prints its id number.
**
**   Parameters: 
**      argp - pointer to thread argument.
**
** - Return value: none.
*/
void TestThread(void * argp) {
	int myID = (int)argp;

	_tprintf(_T("Thread ID: %d\n"), myID);
}

/*
** LaunchThreads - Function to launch some threads
**
**   Parameters: None.
**
** - Return value: none.
*/
void LaunchThreads() {
	int i;
	
	/* launch 5 threads */
	for (i = 0; i < 5; i++) {
		_beginthread(TestThread,0, (void *)(i+1));

	}
}

/*
** TestSpawn - Function to test the use of _spawn and _cwait functions.
**
**   Parameters:
**    ProgName - Null terminated string filename to execute.
**    
** - Return value: none.
*/
void TestSpawn(_TCHAR *ProgName) {
	int nPID;

	/* First spwan and wait for completion*/
	_tspawnl(_P_WAIT, ProgName, _T("Test_of_waiting_"), _T("for_completion_"), _T("of_the_test_program \n"), NULL);
	/* now spawn asynchrousnly and then wait for completion*/
	nPID = _tspawnl(_P_NOWAIT, ProgName, _T("Test_of_asynchronous"), _T("spawned_program \n"), NULL);
	/* Check for error and report it or wait on completion if successful */
	if (nPID == -1) { _tprintf(_T("error launching myself")); }
	else {
		int TermCode;
		_cwait(&TermCode, nPID, _WAIT_CHILD);
		_tprintf(_T("process termination code was %d\n"), TermCode);
	}
}

/*
** TestSystemFunction - Function to test the use of the system command.
**
**   Parameters: None.
**
** - Return value: none.
*/
void TestSystemFunction() {
	_TCHAR FileName[] = _T("radcrtsamp.txt");
	_TCHAR Command[200];
	/*
	* Perform the dir command sending the output to a file
	*/
	_stprintf_s(Command, _T("dir > %s"), FileName);
	_tsystem(Command);
	/* 
	*Now open the file and print the data to the screen
	*/
	_TCHAR Line[500];
	FILE *fp;

	_tfopen_s(&fp, FileName, _T("r"));
	if (fp) {
		while (_fgetts(Line, sizeof(Line), fp)) {
			_tprintf(Line);
		}
		fclose(fp);
		_tremove(FileName);
	}
}

/*
** PlayWithFiles - Function to test the use of the file related functions.
**
**   Parameters: None.
**
** - Return value: none.
*/
void PlayWithFiles() {
	FILE *fp;
	_TCHAR FileName[] = _T("radcrtsamp.txt");
	_TCHAR Buffer[200];
	int i;

	/* 
	* Open a text file, write a few lines, close file.
	*/
	_tfopen_s(&fp, FileName, _T("w"));
	if (fp) {
		for (i = 0; i < 10; i++) {
			_stprintf_s(Buffer, _T("this is line number %d text\n"), i);
			_fputts(Buffer, fp);
		}
		fclose(fp);
		/*
		* Open a text file for reading, read the data and then close and delete the file.
		*/
		_tfopen_s(&fp, FileName, _T("r"));
		if (fp) {
			while (_fgetts(Buffer, sizeof(Buffer), fp)) {
					_tprintf(Buffer);
			}
			fclose(fp);
			_tremove(FileName);
		}
	}
	/*
	* Open a binary file, write a few lines of text in binary format, close file.
	*/
	_tcscpy_s(FileName, _T("radcrtsamp.bin"));
	_tfopen_s(&fp, FileName, _T("wb"));
	if (fp) {
		for (i = 0; i < 10; i++) {
			_stprintf_s(Buffer, _T("this is line number %d in binary\n"), i);
			fwrite(Buffer, sizeof(Buffer), 1, fp);
		}
		fclose(fp);
		/*
		* Now read the data back in binary
		*/
		_tfopen_s(&fp, FileName, _T("rb"));
		if (fp) {
			/* sequentially read everything */
			while (fread(Buffer, sizeof(Buffer), 1, fp) == 1)
				_tprintf(Buffer);
			/* read a few lines randomly*/
			fseek(fp, sizeof(Buffer) * 2, 0);
			fread(Buffer, sizeof(Buffer), 1, fp);
			_tprintf(Buffer);
			fseek(fp, sizeof(Buffer) * 4, 0);
			fread(Buffer, sizeof(Buffer), 1, fp);
			_tprintf(Buffer);
			fseek(fp, sizeof(Buffer) * 6, 0);
			fread(Buffer, sizeof(Buffer), 1, fp);
			_tprintf(Buffer);
			fclose(fp);
		}
		_tremove(FileName);
	}
}

/*
**  PlayWithStrings - Funtion to test the use of string processing functions.
**
**   Parameters: None.
**
** - Return value: none.
*/
void PlayWithStrings() {
	_TCHAR Buffer1[200], Buffer2[200], *Temp;
	_TCHAR *Pos;

	/* Create a string to test on, copy part of strng into another buffer */
	_tcscpy_s(Buffer1, _T("This is a test string\n"));
	_tprintf(Buffer1);
	_tprintf(_T("string length is %d\n"), _tcslen(Buffer1));
	_tcsncpy_s(Buffer2, Buffer1, 15);
	_tcscat_s(Buffer2, _T("<- first 15 characters\n"));
	_tprintf(Buffer2); 
	/* get a substring of original string */
	Temp = _tcsstr(Buffer1, _T("test"));
	_tprintf(Temp);
	/* create a delimited string and parse out the tokens */
	_tcscpy_s(Buffer1, _T("This,string/is,delimited/by,more,then,one/delimiter\n"));
	_tprintf(Buffer1);
	Temp = _tcstok_s(Buffer1, _T(",/"), &Pos);
	while (Temp) {
		_tprintf(_T("%s\n"),Temp);
		Temp = _tcstok_s(NULL, _T(",/"), &Pos);
	}
}
/* 
* standard main entry point
*/
int main(int argc, _TCHAR **argv)
{
	_tprintf(_T("Process id: %d\n"), _getpid()); // who are we shows the multiple processes.
	/* 
	* This program is normally executed without command line parameters from the user.
	* However, the same program is executed again to show use of the process functions
	* The arguments determine the path to take.
	*/
	if(argc > 1) { 
		/* simply print each argument out and exit */
		for (int i = 0; i < argc; i++) {
			_tprintf(argv[i]);
		}
		/* use different exit code when invoked by ourself */
		return 1;
	}
	/*
	* No command line arguments so proceed with test code
	*/
	TestSpawn(argv[0]);
	LaunchThreads();
	TestSystemFunction();
	PlayWithFiles();
	PlayWithStrings();
    return 0;
}

