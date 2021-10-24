#include "Test_Code.h"
#define MAX_ADDRESS_LENGTH 40

int CountAssemblingErrors()
{ //Counts the number of errors in the compilation process of TASM
	FILE* TASMOut = fopen("TASM\\Tasm_Out.txt", "r");
	if (!CheckFilePointer(TASMOut)) return -1;
	char buff[20] = "\0";
	while (!feof(TASMOut))
	{
		fscanf(TASMOut, "%s", buff);
		if (strcmp(buff, "Error") == 0)
		{
			fscanf(TASMOut, "%s", buff);
			int errorCount = 0;
			fscanf(TASMOut, "%d", &errorCount);
			fclose(TASMOut);
			return errorCount;
		}
		else
		{
			GoToNextLine(TASMOut);
		}
	}
	fclose(TASMOut);
	return -1;
}

Error* GetAssemblingErrors(int errorCount)
{
	if (errorCount == 0) return NULL;
	FILE* TASMOut = fopen("TASM\\Tasm_Out.txt", "r");
	if (!CheckFilePointer(TASMOut)) return NULL;
	for (int i = 0; i < 3; i++)
	{
		GoToNextLine(TASMOut);
	}
	char buff[40];
	buff[0] = '\0';
	Error* errors = (Error*)malloc(errorCount * sizeof(Error));
	int i = 0;
	for (i = 0; i < errorCount; i++)
	{
		fscanf(TASMOut, "%s", buff); //Skip **error**
		fgets(buff, 23, TASMOut); //skip  Exers\Exer1\Code.asm(
		errors[i].lineNumber = ScanIntFromFile(TASMOut); //scan line number
		fgetc(TASMOut); //Skip space
		fgets(errors[i].description, MAX_ERROR_SIZE, TASMOut);
	}
	return errors;
}

void DisplayErrors(int errorCount)
{
	printf("Error count: %d\n", errorCount);
	printf("Error list: \n\n");
	Error* errors = GetAssemblingErrors(errorCount);
	for (int i = 0; i < errorCount; i++)
	{
		printf("(%d): ", errors[i].lineNumber);
		puts(errors[i].description);
	}
	free(errors);
}

int TestCode(char* exerciseNum)
{
	AssembleCode(exerciseNum, "test");
	int errorCount = CountAssemblingErrors();
	if (errorCount)
	{
		printf("Errors when testing code\n");
		DisplayErrors(errorCount);
		system("pause");
		return -1;
	}
	return CheckResult();
}

void AssembleCode(char* exerciseNum, char* type)
{
	//type is either code or test
	MyCopyFileByPath("Configuration.conf", "Configuration_Template.conf");
	FILE* confFile = fopen("Configuration.conf", "a");
	if (!confFile) return;
	char path[100]; //will hold current directory
	_getcwd(path, 100);
	fprintf(confFile, "mount c %s\\TASM\n", path);
	fprintf(confFile, "c:\n");
	char* codePath = GetCodeFilePath(exerciseNum, type, "asm"); //will hold path inside project to the code file
	fprintf(confFile, "tasm /zi %s > TASM_Out.txt\n", codePath + 5);
	fprintf(confFile, "tlink /v %s.obj\n", type);
	fprintf(confFile, "%s.exe > OUTPUT.txt\n", type);
	fprintf(confFile, "exit\n");
	fclose(confFile);
	system("Assemble_Code.bat");
	remove("Configuration.conf");
	free(codePath);
}

int CheckResult()
{
	FILE* fileOutput = fopen("TASM\\Output.txt", "r");
	if (!CheckFilePointer(fileOutput)) return -1;
	int result = 0;
	fscanf(fileOutput, "%d", &result);
	fclose(fileOutput);
	return result;
}