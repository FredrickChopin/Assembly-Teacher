#include "Code_Input.h"
#define EXERCISE_COUNT 1

void main()
{
	system("cls");
	char exerciseNum[3];
	printf("Choose an exercise num --- (1)\n");
	scanf("%s", exerciseNum);
	int num = atoi(exerciseNum);
	if (num < 1 || num > EXERCISE_COUNT)
	{
		printf("Invalid exercise number\n");
		system("pause");
		main();
		return;
	}
	system("cls");
	GetCodeFromUser("1"); //Starting point
	printf("Press any key to test the code . . . ");
	rewind(stdin);
	_getch();
	system("cls");
	clock_t start = clock();
	printf("Please wait . . . ");
	int passed = TestCode("1");
	system("cls");
	clock_t end = clock();
	double seconds = CalculateTimePassed(start, end);
	//Remove these files that are created because we are running
	//DOSBox noconsole
	remove("stderr.txt");
	remove("stdout.txt");
	remove("TASM\\OUTPUT.txt");
	printf("Time took to assemble: %f\n", seconds);
	if (passed == 1)
	{
		printf("Good job! All cases passed!\n");
		printf("Would you like to do another exercise? y / (n)\n");
		rewind(stdin);
		char c = 0;
		scanf("%c", &c);
		if (c == 'y') main();
	}
	else if (passed == 0)
	{
		printf("Failed at least 1 test case\n");
		printf("Would you like to try again? y / (n)\n");
		char c = 0;
		rewind(stdin);
		scanf("%c", &c);
		if (c == 'y')
		{
			main();
		}
	}
	else
	{
		printf("Error when testing\n");
	}
}