#include "User_Handling.h"
#define MIL_SECONDS_TO_WAIT 2800
#define EXERCISE_COUNT 2 //The amount of current exercises available

typedef struct Option
{
	void (*function)(char*);
	char name[20];
}Option;

void EndProgram()
{
	/// <summary>
	/// This function must be called before closing the program
	/// It prints a thank you message and cleans garbage files
	/// </summary>
	CleanGarbageFiles();
	system("cls");
	printf("Thank you for using Assembly Teacher\n");
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
	while (1)
	{
		system("cls");
		char exerciseNum[4];
		printf("Choose an exercise number:\n\n");
		int i;
		for (i = 1; i <= EXERCISE_COUNT; i++)
		{
			printf("Exercise %d ---> (%d)\n", i, i);
		}
		printf("Exit ---> (%d)\n\n", EXERCISE_COUNT + 1);
		int success = GetIntInRange(exerciseNum, 4, EXERCISE_COUNT + 1);
		if (atoi(exerciseNum) == EXERCISE_COUNT + 1)
		{
			EndProgram();
		}
		if (success)
		{
			ExerciseMenu(exerciseNum);
		}
	}
}

void ExerciseMenu(char* exerciseNum)
{
	/// <summary>
	/// The menu the user sees after choosing an exercise number
	/// </summary>
	/// <param name="exerciseNum"> The exercise number he chose </param>
	Option options[] = 
	{
		{GetCodeFromUser, "Edit code"},
		{TestCodeToUser, "Test code" },
		{ShowSolutionToUser, "Show solution"},
		{ResetCodeFile, "Reset code file"},
		{NULL, "Go back"},
		{EndProgram, "Exit"}
	};
	int optionCount = sizeof(options) / sizeof(Option);
	while (1)
	{
		system("cls");
		printf("Choose an operation:\n\n");
		int i;
		for (i = 0; i < optionCount; i++)
		{
			printf("(%d) --> %s\n", i + 1, options[i].name);
		}
		putchar('\n');
		char input[3];
		int success = GetIntInRange(input, 3, optionCount);
		if (success)
		{
			int choice = atoi(input);
			void (*functionChoice)(char*) = options[choice - 1].function;
			if (functionChoice == NULL)
			{
				return;
			}
			functionChoice(exerciseNum);
		}
	}
}

void ShowSolutionToUser(char* exerciseNum)
{
	/// <summary>
	/// Opens the solution code file for the exercise
	/// </summary>
	/// <param name="exerciseNum"> The exercise number</param>
	OpenFileToUser(exerciseNum, "Solution", "asm");
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

void DisplayErrors(int errorCount, char* exerciseNum, FILE* stream, int commentErrors)
{
	/// <summary>
	/// Finds out and displays the assembling errors 
	/// </summary>
	/// <param name="errorCount"> The number of assembling errors </param>
	if (commentErrors)
	{
		fprintf(stream, ";");
	}
	fprintf(stream, "Error count: %d\n", errorCount);
	if (commentErrors)
	{
		fprintf(stream, ";");
	}
	fprintf(stream, "Error list: \n");
	Error* errors = GetAssemblingErrors(errorCount, exerciseNum);
	for (int i = 0; i < errorCount; i++)
	{
		if (commentErrors)
		{
			fprintf(stream, ";");
		}
		fprintf(stream, "(%d): %s", errors[i].lineNumber, errors[i].description);
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

void LetUserEdit(char* exerciseNum)
{
	/// <summary>
	/// Lets the user edit the code for the exercise
	/// </summary>
	/// <param name="exerciseNum"> The exercise number </param>
	OpenFileToUser(exerciseNum, "Code", "asm");
}

void TestCodeToUser(char* exerciseNum)
{
	/// <summary>
	/// Tests the user's code and presents the result
	/// </summary>
	/// <param name="exercsieNum"> The exercise number </param>
	system("cls");
	printf("Please wait . . . ");
	clock_t start = clock();
	HANDLE DOSBoxHandle = TestCode(exerciseNum);
	HANDLE DOSBoxJob = CreateJobAssignProcess(DOSBoxHandle);
	DWORD waitType = WaitForSingleObject(DOSBoxHandle, MIL_SECONDS_TO_WAIT);
	int errorCount = CountAssemblingErrors();
	clock_t end = clock();
	system("cls");
	if (waitType == WAIT_TIMEOUT)
	{
		printf("Took too long to test\n");
		printf("Perhaps your code has runtime errors or is stuck at an infinite loop\n");
		TerminateJobObject(DOSBoxJob, 1);
	}
	else
	{
		double seconds = CalculateTimePassed(start, end);
		printf("Time took to test: %.2f\n", seconds);
		if (errorCount)
		{
			printf("There are assembling errors in the code\n");
			DisplayErrors(errorCount, exerciseNum, stdout, 0);
		}
		else
		{
			printf("No assembling errors\n");
			int result = CheckResult();
			if (result == 1)
			{
				printf("Good job! All test cases passed!\n");
			}
			else if (result == 0)
			{
				printf("Your code has failed the test\n");
			}
			else
			{
				ThrowError("Weird behaviour, neither 0 nor 1\n", 0);
			}
		}
	}
	remove("temp.txt"); //sometimes unecessary, doesn't hurt when not necessary
	CloseHandle(DOSBoxHandle);
	CloseHandle(DOSBoxJob);
	system("pause");
}

//void WriteAssemblingErrorsInCodeFile(int exerciseNum, int errorCount)
//{
//	FILE* temp = fopen("temp.txt", "w");
//	if (!CheckFilePointer(temp, "TestCodeToUser"))
//	{
//		return;
//	}
//	fputs(";Last errors:\n", temp);
//	DisplayErrors(errorCount, exerciseNum, temp, 1);
//	char* codeFilePath = GetCodeFilePath(exerciseNum, "code", "asm");
//	FILE* codeFile = fopen(codeFilePath, "r");
//	if (!CheckFilePointer(temp, "TestCodeToUser"))
//	{
//		fclose(temp);
//		free(codeFilePath);
//		return;
//	}
//	fputc('\n', temp);
//	MyCopyFile(temp, codeFile);
//	fclose(codeFile);
//	fclose(temp);
//	remove(codeFilePath);
//	rename("temp.txt", codeFilePath);
//	free(codeFilePath);
//}

/*void AssembleCodeToUser(char* exerciseNum)
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
	size_t start = clock();
	HANDLE DOSBoxHandle = AssembleCode(exerciseNum, "Code", 0);
	HANDLE DOSBoxJob = CreateJobAssignProcess(DOSBoxHandle);
	DWORD waitType = WaitForSingleObject(DOSBoxHandle, MIL_SECONDS_TO_WAIT);
	size_t end = clock();
	double secondsTook = CalculateTimePassed(start, end);
	system("cls");
	if (waitType == WAIT_TIMEOUT)
	{
		printf("Took too long to assemble\n");
		printf("This shouldn't happen\n");
		TerminateJobObject(DOSBoxJob, 1);
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
	CloseHandle(DOSBoxJob);
	TruncateFromEnd(path, 4);
	free(path);
	system("pause");
}*/