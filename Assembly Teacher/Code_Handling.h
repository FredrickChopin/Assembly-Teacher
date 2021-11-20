#include "Libaries.h"
#pragma once
#include "File_Handling.h"

//This file handles the work with the code.
//Assembling, running, and testing the code.

//Struct for storing an assembling error
#define MAX_ERROR_SIZE 80
typedef struct
{
	int lineNumber;
	char description[MAX_ERROR_SIZE];
}Error;

int CountAssemblingErrors();
Error* GetAssemblingErrors(int errorCount, char* exerciseNum);
HANDLE TestCode(char* exerciseNum);
int CheckResult();
HANDLE AssembleCode(char* exerciseNum, char* name, int run);
PROCESS_INFORMATION MyCreateProcess(char* exeName, char* args);
HANDLE CreateJobAssignProcess(HANDLE process);