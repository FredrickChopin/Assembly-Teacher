#include "User_Handling.h"
#define MIL_SECONDS_TO_WAIT 5000

void GetCodeFromUser(char* exerciseNum)
{
	printf("Would you like to edit the code? y / (n)\n");
	rewind(stdin);
	char c = 0;
	scanf("%c", &c);
	if (c == 'y')
	{
		system("cls");
		printf("Write the code in the editor that is going to prompt\n");
		printf("Close the editor when you are done\n");
		printf("Make sure to follow the given commented instructions\n");
		system("pause");
		LetUserEdit(exerciseNum);
	}
	system("cls");
	AssembleCodeToUser(exerciseNum);
	int errorCount = 0;
	while ((errorCount = CountAssemblingErrors()) != 0)
	{
		DisplayErrors(errorCount);
		printf("Your code has errors\n");
		printf("Would you like to try again?\n");
		system("pause");
		system("cls");
		LetUserEdit(exerciseNum);
		AssembleCodeToUser(exerciseNum);
	};
	printf("Code free of errors!\n");
}


void AssembleCodeToUser(char* exerciseNum)
{
	printf("Press any key to assemble . . . ");
	rewind(stdin);
	_getch();
	system("cls");
	printf("Please wait . . . ");
	size_t start = clock();
	//Now append end
	char* path = GetCodeFilePath(exerciseNum, "Code", "asm");
	FILE* codeFile = fopen(path, "a");
	if (!CheckFilePointer(codeFile)) return;
	fprintf(codeFile, "\nend");
	fclose(codeFile);
	AssembleCode(exerciseNum, "Code");
	TruncateFromEnd(path, 4);
	free(path);
	size_t end = clock();
	double seconds = CalculateTimePassed(start, end);
	system("cls");
	printf("Time took to test: %f\n", seconds);
}

void LetUserEdit(char* exerciseNum)
{
	char* path = GetCodeFilePath(exerciseNum, "Code", "asm");
	system(path);
	free(path);
}

int TestCodeToUser(char* exercsieNum)
{
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
		return -1;
	}
	return CheckResult();
}

double CalculateTimePassed(size_t start, size_t end)
{
	return ((double)end - (double)start) / CLOCKS_PER_SEC;
}