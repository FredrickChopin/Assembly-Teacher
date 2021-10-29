#include "User_Handling.h"
#define MIL_SECONDS_TO_WAIT 5000
#define EXERCISE_COUNT 2 //The amount of current exercises available + 1
#define OPERATION_COUNT 6 
#define MIL_SECONDS_TO_WAIT 2800

void EndProgram()
{
	/// <summary>
	/// This function must be called before closing the program
	/// It prints a thank you message and cleans garbage files
	/// </summary>
	CleanGarbageFiles();
	system("cls");
	printf("Thank you for using Assembly Teacher\n");
	Sleep(2000);
	exit(0);
}

int GetIntInRange(char* buff, int buffSize, int top)
{
	/// <summary>
	/// This function gets from the user an int on the interval [1, top]
	/// </summary>
	/// <param name="buff"> Buffer to hold the int as a string, must be allocated </param>
	/// <param name="buffSize"> The size of the buffer </param>
	/// <param name="top"> Maximal top integer </param>
	/// <returns> Boolean value determining if the user entered an int in that range </returns>
	if (buffSize <= 0)
	{
		ThrowError("At GetIntInRange, buffSize <= 0\n", 0);
	}
	rewind(stdin);
	int i;
	char c;
	for (i = 0; i < buffSize - 1; i++)
	{
		c = fgetc(stdin);
		if (c == '\n')
		{
			break;
		}
		buff[i] = c;
	}
	buff[i] = '\0';
	int num = atoi(buff);
	if (num < 1 || num > top)
	{
		printf("Invalid choise\n");
		system("pause");
		return 0;
	}
	return 1;
}

void MainMenu()
{
	/// <summary>
	/// The first menu the user sees
	/// </summary>
	system("cls");
	char exerciseNum[4];
	printf("Choose an exercise number:\n\n");
	printf("Exercise ---> (1)\n");
	printf("Exit ---> (2)\n\n");
	int success = GetIntInRange(exerciseNum, 4, EXERCISE_COUNT);
	if (atoi(exerciseNum) == EXERCISE_COUNT)
	{
		EndProgram();
	}
	if (!success)
	{
		MainMenu();
		return;
	}
	ExerciseMenu(exerciseNum);
	MainMenu();
}

void ExerciseMenu(char* exerciseNum)
{
	/// <summary>
	/// The menu the user sees after choosing an exercise number
	/// </summary>
	/// <param name="exerciseNum"> The exercise number he chose </param>
	system("cls");
	printf("Choose an operation:\n\n");
	printf("(1) ---> Edit code\n");
	printf("(2) ---> Check for code errors\n");
	printf("(3) ---> Test code\n");
	printf("(4) ---> Reset code file\n");
	printf("(5) ---> Go back\n\n");
	char input[3];
	int success = GetIntInRange(input, 3, OPERATION_COUNT);
	if (!success)
	{
		ExerciseMenu(exerciseNum);
		return;
	}
	int num = atoi(input);
	if (num == 1)
	{
		GetCodeFromUser(exerciseNum);
	}
	else if (num == 2)
	{
		AssembleCodeToUser(exerciseNum);
	}
	else if (num == 3)
	{
		TestCodeToUser(exerciseNum);
	}
	else if (num == 4)
	{
		ResetCodeFile(exerciseNum);
	}
	else //if (num == 5)
	{
		return;
	}
	ExerciseMenu(exerciseNum);
}

void ResetCodeFile(char* exerciseNum)
{
	/// <summary>
	/// Each exercise has a code back up file
	/// The user can choose to revert back any time he wants
	/// </summary>
	/// <param name="exerciseNum"> The exercise to reset </param>
	char* codePath = GetCodeFilePath(exerciseNum, "Code", "asm");
	char* backUpPath = GetCodeFilePath(exerciseNum, "Code_Backup", "asm");
	remove(codePath);
	MyCopyFileByPath(codePath, backUpPath);
	free(codePath);
	free(backUpPath);
	system("cls");
	printf("Reseting completed successfully\n");
	system("pause");
}

void DisplayErrors(int errorCount)
{
	/// <summary>
	/// Finds out and displays the assembling errors 
	/// </summary>
	/// <param name="errorCount"> The number of assembling errors </param>
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

void GetCodeFromUser(char* exerciseNum)
{
	/// <summary>
	/// Prints general instructions and lets the user edit the code
	/// </summary>
	/// <param name="exerciseNum"> The exercise number </param>
	system("cls");
	printf("Write the code in the editor that is going to prompt\n");
	printf("Close the editor when you are done\n");
	printf("Make sure to follow the given commented instructions\n");
	system("pause");
	LetUserEdit(exerciseNum);
}

void AssembleCodeToUser(char* exerciseNum)
{
	/// <summary>
	/// Assembles the user's code. If there are any assembling errors it prints them out.
	/// </summary>
	/// <param name="exerciseNum"> The exercise number </param>
	system("cls");
	printf("Please wait . . . ");
	//Append end
	char* path = GetCodeFilePath(exerciseNum, "Code", "asm");
	FILE* codeFile = fopen(path, "a");
	if (!CheckFilePointer(codeFile, "AssembleCodeToUser")) return;
	fprintf(codeFile, "\nend");
	fclose(codeFile);
	//Start running
	time_t start = clock();
	HANDLE DOSBoxHandle = AssembleCode(exerciseNum, "Code", 0);
	DWORD waitType = WaitForSingleObject(DOSBoxHandle, MIL_SECONDS_TO_WAIT);
	time_t end = clock();
	double secondsTook = CalculateTimePassed(start, end);
	system("cls");
	if (waitType == WAIT_TIMEOUT)
	{
		printf("Took too long to assemble\n");
		printf("Perhaps your code has runtime errors or it stuck at an infinite loop\n");
		TerminateProcess(DOSBoxHandle, 1);
	}
	else
	{
		printf("Time took to assemble: %f\n", secondsTook);
		int errorCount = CountAssemblingErrors();
		if (errorCount)
		{
			printf("Your code has errors\n");
			DisplayErrors(errorCount);
		}
		else
		{
			printf("Code free of errors!\n");
		}
	}
	CloseHandle(DOSBoxHandle);
	TruncateFromEnd(path, 4);
	free(path);
	system("pause");
}

void LetUserEdit(char* exerciseNum)
{
	/// <summary>
	/// Lets the user edit the code for the exercise
	/// </summary>
	/// <param name="exerciseNum"> The exercise number </param>
	char* path = GetCodeFilePath(exerciseNum, "Code", "asm");
	system(path);
	free(path);
}

void TestCodeToUser(char* exercsieNum)
{
	/// <summary>
	/// Tests the user's code and presents the result
	/// </summary>
	/// <param name="exercsieNum"> The exercise number </param>
	system("cls");
	printf("Please wait . . . ");
	clock_t start = clock();
	HANDLE DOSBoxHandle = TestCode(exercsieNum);
	DWORD waitType = WaitForSingleObject(DOSBoxHandle, MIL_SECONDS_TO_WAIT);
	int errorCount = CountAssemblingErrors();
	clock_t end = clock();
	system("cls");
	if (waitType == WAIT_TIMEOUT)
	{
		printf("Took too long to test\n");
		printf("Perhaps your code has runtime errors or it stuck at an infinite loop\n");
		TerminateProcess(DOSBoxHandle, 1);
	}
	else
	{
		double seconds = CalculateTimePassed(start, end);
		printf("Time took to test: %f\n", seconds);
		if (errorCount)
		{
			printf("Errors when testing code\n");
			DisplayErrors(errorCount);
			system("pause");
			return;
		}
		int result = CheckResult();
		if (result == 1)
		{
			printf("Good job! All test cases passed!\n");
		}
		else if (result == 0)
		{
			printf("At least one test case failed\n");
		}
		else
		{
			ThrowError("Weird behaviour, neither 0 nor 1\n", 0);
		}
	}
	CloseHandle(DOSBoxHandle);
	system("pause");
}