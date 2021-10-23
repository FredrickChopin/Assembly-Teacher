#include "File_Handling.h"
#include <Windows.h>
#include <direct.h>
#define MAX_ERROR_SIZE 80

typedef struct
{
	int lineNumber;
	char description[MAX_ERROR_SIZE];
}Error;

int CountAssemblingErrors();
Error* GetAssemblingErrors(int errorCount);
int TestCode(char* exerciseNum);
int CheckResult();
void DisplayErrors(int errorCount);
void AssembleCode(char* exerciseNum, char* type);