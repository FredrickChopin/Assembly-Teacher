#include "Code_Handling.h"
#define MAX_ADDRESS_LENGTH 40

int CountAssemblingErrors()
{   
	/// <summary>
	/// Counts the number of assembling errors in the recent assembling
	/// </summary>
	/// <returns> The count of assembling erorrs </returns>
	FILE* TASMOut = fopen("TASM\\Tasm_Out.txt", "r");
	if (!CheckFilePointer(TASMOut, "CountAssemblingErrors")) return -1;
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
	/// <summary>
	/// Allocates an error array dynamically with length of errorCount
	/// Fills the error array with the recent assembling erorrs
	/// Memory must be freed
	/// </summary>
	/// <param name="errorCount"> The count of assembling errors </param>
	/// <returns> Dynamic error array filled with the last assembling errors </returns>
	if (errorCount == 0) return NULL;
	FILE* TASMOut = fopen("TASM\\Tasm_Out.txt", "r");
	if (!CheckFilePointer(TASMOut, "GetAssemblingErrors")) return NULL;
	for (int i = 0; i < 3; i++)
	{
		GoToNextLine(TASMOut);
	}
	char buff[40];
	buff[0] = '\0';
	Error* errors = (Error*)malloc(errorCount * sizeof(Error));
	if (!errors)
	{
		ThrowError("Unable to allocate dynamic memory in GetAssemblingErrors\n", 0);
		return NULL;
	}
	int i = 0;
	for (i = 0; i < errorCount; i++)
	{
		fscanf(TASMOut, "%s", buff); //Skip **error**
		fgets(buff, 23, TASMOut); //skip  Exers\Exer1\Code.asm(
		errors[i].lineNumber = ScanIntFromFile(TASMOut); //scan line number
		fgetc(TASMOut); //Skip space
		fgets(errors[i].description, MAX_ERROR_SIZE, TASMOut);
	}
	fclose(TASMOut);
	return errors;
}

HANDLE TestCode(char* exerciseNum)
{
	/// <summary>
	/// Assembles and runs the test file
	/// Returns handle to the DOSBox
	/// The handle must be closed afterwards
	/// </summary>
	/// <param name="exerciseNum"> The exercise number</param>
	/// <returns> Returns handle to the DOSBox </returns>
	return AssembleCode(exerciseNum, "test", 1);
}

HANDLE AssembleCode(char* exerciseNum, char* name, int run)
{
	//type is either code or test
	MyCopyFileByPath("Configuration.conf", "Configuration_Template.conf");
	FILE* confFile = fopen("Configuration.conf", "a");
	if (!CheckFilePointer(confFile, "AssembleCode")) return NULL;
	char path[100]; //will hold current directory
	_getcwd(path, 100);
	fprintf(confFile, "mount c %s\\TASM\n", path);
	fprintf(confFile, "c:\n");
	char* codePath = GetCodeFilePath(exerciseNum, name, "asm"); //will hold path inside project to the code file
	fprintf(confFile, "tasm /zi %s > TASM_Out.txt\n", codePath + 5);
	fprintf(confFile, "tlink /v %s.obj\n", name);
	if(run) fprintf(confFile, "%s.exe > OUTPUT.txt\n", name);
	fprintf(confFile, "exit\n");
	fclose(confFile);
	free(codePath);
	//Old way was to use a batch file
	//system("Assemble_Code.bat");
	//Now we create a process and return its handle
	PROCESS_INFORMATION pi = MyCreateProcess("Assemble_Code.bat", "");
	CloseHandle(pi.hThread);
	return pi.hProcess;
}

int CheckResult()
{
	FILE* fileOutput = fopen("TASM\\Output.txt", "r");
	if (!CheckFilePointer(fileOutput, "CheckResult")) return -1;
	int result = 0;
	fscanf(fileOutput, "%d", &result);
	fclose(fileOutput);
	return result;
}