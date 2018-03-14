// strokefi.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <ios>
#include <string>
#include <iostream>
#include <cstdlib>


using namespace std;


int main()
{
	WCHAR cchFiName[MAX_PATH]; // file name buffer
	char cchBuffLayer[MAX_PATH]; // user input buffer
	DWORD* pNumBytesWritten = new DWORD();
	BOOL bWriteError = FALSE;
	

	ZeroMemory(cchBuffLayer, MAX_PATH); // clear memory blocks for the buffer cchBuffLayer
	FillMemory(cchBuffLayer, MAX_PATH, 32);  // initiating blank spaces on memory blocks taken by cchBuffLayer
	std::string mstr(cchBuffLayer);  // construct a std::string object initialized with cchBuffLayer for its internal string buffer 


	printf_s("%ls", L"Enter file name: ");
	wscanf_s(L"%ls", cchFiName, _countof(cchFiName));

	printf_s("\n\n");

	printf_s("%ls", L"Type any text below here then press [Return/Enter] to save it.\n");
	
	
	cin.clear(); // clear stdin buffer to get rid of any left behind trace-junks
	cin.ignore(1, '\0'); // mark the beginning input of stdin negating every possible null-terminated character found in the first place
	
	cin.getline((char*)mstr.c_str(), mstr.size(), cin.widen('\n')); //acquiring user input data plunged in the stdin
	
	// create a new file to be write to
	HANDLE hFile = CreateFile(cchFiName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);

	printf_s("\n\n");

	if (INVALID_HANDLE_VALUE == hFile) {
		printf_s("%ls", L"Some Errors happen while program attempts open accesses to the file !!\n");
		CloseHandle(hFile);
		return 0;
	}

	// issue WriteFile API to dump any user entered data to the file
	bWriteError = WriteFile(hFile, mstr.data(), (DWORD)mstr.length(), (LPDWORD)pNumBytesWritten, NULL);

	if (bWriteError)
		printf_s("%ld %ls", (long)pNumBytesWritten, L" bytes saved to the file !\n\n");
	else
		printf_s("%ls", L"Error writing unknown format of data, corrupted or damaged file !!\n\n");

	// if number of bytes written to the file differed slightly with number of bytes input from the user, then there might be things went wrong
	if (mstr.length() != *pNumBytesWritten) printf_s("%ls" "%ld", L"Buffer Overrun with invalid data length. Current specified data length:\n\n",(long)*pNumBytesWritten);

	CloseHandle(hFile);


	// Codes for Reading data from the file written above
	DWORD* pNBytesRead = new DWORD();
	char* cchReadBuff = new char[MAX_PATH]; // buffer for data read
	DWORD nNumBytesRead = strlen(cchReadBuff); // bytes size for the read buffer
	BOOL bReadFlag = FALSE;

	// Open the just written file in general read mode
	HANDLE hFileRead = CreateFile(cchFiName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	// Data would be read from the file to cchReadBuff, ReadFile reads up number of bytes of the data entered by user in cchBuffLayer
	bReadFlag = ReadFile(hFileRead, cchReadBuff, (DWORD)strlen(cchReadBuff), (LPDWORD)pNBytesRead, NULL);

	if (!bReadFlag) {
		printf_s("%ls", L"Read Error!! Unknown data format, corrupted or damaged file\n\n"); CloseHandle(hFileRead);
		return 0;
	}

	printf_s("\n\n");

	printf_s("%ls", L"Outputting previous saved data..\n ");
	printf_s("%ls", L"----------------------------------------\n");
	
	cout << cchReadBuff << endl;



	printf_s("\n\n");
	CloseHandle(hFileRead);
	printf_s("\n\n");

	printf_s("%ls", L"Any key to continue.. \n");
	system("PAUSE");
	return -1;
}

