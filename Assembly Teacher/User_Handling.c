#include "User_Handling.h"
#define MIL_SECONDS_TO_WAIT 5000
#define EXERCISE_COUNT 2 //The amount of current exercises available + 1
#define OPERATION_COUNT 6 
#define MIL_SECONDS_TO_WAIT 3000

void EndProgram()
{
	system("cls");
	printf("Thank you for using Assembly Teacher\n");
	CleanGarbageFiles();
	exit(0);
}

int GetIntInRange(char* buff, int buffSize, int top)
{
	//including top number
	//Returns boolean value which determines wether the user entered an int
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
	//Returns ANOTHER_EXERCISE if user wants to do another exercise after this exercise
	//Returns DIFFERENT_EXERCISE otherwise
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
		RunCodeToUser(exerciseNum);
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
	char* codePath = GetCodeFilePath(exerciseNum, "Code", "asm");
	char* backUpPath = GetCodeFilePath(exerciseNum, "Code_Backup", "asm");
	remove(codePath);
	MyCopyFileByPath(codePath, backUpPath);
	free(codePath);
	free(backUpPath);
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

void GetCodeFromUser(char* exerciseNum)
{
	system("cls");
	printf("Write the code in the editor that is going to prompt\n");
	printf("Close the editor when you are done\n");
	printf("Make sure to follow the given commented instructions\n");
	system("pause");
	LetUserEdit(exerciseNum);
}

void RunCodeToUser(char* exerciseNum)
{
	system("cls");
	printf("Please wait . . . ");
	//Append end
	char* path = GetCodeFilePath(exerciseNum, "Code", "asm");
	FILE* codeFile = fopen(path, "a");
	if (!CheckFilePointer(codeFile, "RunCodeToUser")) return;
	fprintf(codeFile, "\nend");
	fclose(codeFile);
	//Start running
	time_t start = clock();
	system("set sdl_videodriver=dummy");
	HANDLE DOSBoxHandle = AssembleCode(exerciseNum, "Code", 0);
	DWORD waitType = WaitForSingleObject(DOSBoxHandle, MIL_SECONDS_TO_WAIT);
	time_t end = clock();
	double secondsTook = CalculateTimePassed(start, end);
	system("cls");
	printf("Wait type = %lu\n", waitType);
	if (waitType == WAIT_TIMEOUT)
	{
		printf("Took too long to test\n");
		printf("Perhaps your code has runtime errors or it stuck at an infinite loop\n");
		TerminateProcess(DOSBoxHandle, 1);
	}
	else
	{
		printf("Time took to test: %f\n", secondsTook);
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
	char* path = GetCodeFilePath(exerciseNum, "Code", "asm");
	system(path);
	free(path);
}

void TestCodeToUser(char* exercsieNum)
{
	system("cls");
	printf("Please wait . . . ");
	clock_t start = clock();
	TestCode(exercsieNum);
	int errorCount = CountAssemblingErrors();
	clock_t end = clock();
	double seconds = CalculateTimePassed(start, end);
	system("cls");
	printf("Time took to assemble: %f\n", seconds);
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
		ThrowError("Weird behaviour\n", 0);
	}
	system("pause");
}

double CalculateTimePassed(size_t start, size_t end)
{
	return ((double)end - (double)start) / CLOCKS_PER_SEC;
}