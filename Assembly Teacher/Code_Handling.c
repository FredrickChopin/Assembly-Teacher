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
		fgets(buff, 23, TASMOut); //skip  _Exers\Exer1\Code.asm(
		errors[i].lineNumber = ScanIntFromFile(TASMOut); //scan line number
		fgetc(TASMOut); //Skip space
		fgets(errors[i].description, MAX_ERROR_SIZE, TASMOut);
	}
	fclose(TASMOut);
	return errors;
}

PROCESS_INFORMATION MyCreateProcess(char* exeName, char* args)
{
	/// <summary>
	/// Starts a process with a name exeName and args args
	/// </summary>
	/// <param name="exeName"> The name of the executable file</param>
	/// <param name="args"> The command-line arguments to create the process with </param>
	/// <returns> Process-Information struct </returns>
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	// create argument string
	INT size = strlen(exeName) + strlen(args) + 2;
	PCHAR param = (PCHAR)malloc(size * sizeof(CHAR));
	if (!param)
	{
		ThrowError("Couldn't allocate memory for param in MyCreateProcess", 0);
	}
	sprintf(param, "%s %s", exeName, args);
	// Start the child process.
	system("set sdl_videodriver = dummy");
	BOOL processCreated = CreateProcessA
	(
		NULL, //Application name
		param, // Command line
		NULL, // Process handle not inheritable
		NULL, // Thread handle not inheritable
		FALSE, // Set handle inheritance to FALSE
		0, // No creation flags
		NULL, // Use parent's environment block
		NULL, // Use parent's starting directory
		&si, // Pointer to STARTUPINFO structure
		&pi // Pointer to PROCESS_INFORMATION structure
	);
	free(param);
	if (!processCreated)
	{
		ThrowError("Failed to create process\n", 0);
	}
	return pi;
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
	/// <summary>
	/// Assembles code to user.
	/// </summary>
	/// <param name="exerciseNum"> The number of exercise</param>
	/// <param name="name"> The name of the file, e.g "Test", "Code"</param>
	/// <param name="run"> Boolean value determining wether to also run the code</param>
	/// <returns> HANDLE to the DOSBox created process </returns>
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
	//PROCESS_INFORMATION pi = MyCreateProcess("DOSBox-0.74-3\\DOSBox.exe", "-conf Configuration.conf -noconsole");
	PROCESS_INFORMATION pi = MyCreateProcess("Assemble_Code.bat", "");
	CloseHandle(pi.hThread);
	return pi.hProcess;
}

HANDLE CreateJobAssignProcess(HANDLE process)
{
	/// <summary>
	/// Creates a new job object and assigns it the given process
	/// We use this function after creating the Assemble_Code.bat process
	/// in order to assign it to a job
	/// Assigning it to a job allows us to then terminate the DOSBox which the
	/// Assemble_Code.bat creates
	/// </summary>
	/// <param name="process"> HANDLE to a process </param>
	/// <returns> HANDLE to the newly created job object</returns>
	HANDLE job = CreateJobObjectA(NULL, NULL);
	if (job == NULL)
	{
		ThrowError("Couldn't create job object at AssignProcessToJob\n", 0);
		return NULL;
	}
	AssignProcessToJobObject(job, process);
	return job;
}

int CheckResult()
{
	/// <summary>
	/// Opens the TASM\Output.txt file, which is the ouput the Test.asm program created
	/// Checks if there's a 1 or a 0 there
	/// </summary>
	/// <returns> Wether user's code successfully completed the task</returns>
	FILE* fileOutput = fopen("TASM\\Output.txt", "r");
	if (!CheckFilePointer(fileOutput, "CheckResult")) return -1;
	int result = 0;
	fscanf(fileOutput, "%d", &result);
	fclose(fileOutput);
	return result;
}