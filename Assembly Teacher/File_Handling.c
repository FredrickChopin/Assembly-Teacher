#include "File_Handling.h"

int CheckFilePointer(FILE* f)
{
	if (f)
	{
		return 1;
	}
	else
	{
		ThrowError("Error when opening file\n");
		return 0;
	}
}

void ThrowError(char* str)
{
	printf("%s", str);
	rewind(stdin);
	_getch();
}

int GoToNextLine(FILE* f)
{
	char c = 0;
	while (1)
	{
		char c = fgetc(f);
		if (c == EOF) return 0;
		if (c == '\n') return 1;
	}
}

int ScanIntFromFile(FILE* f)
{
	char c = 0;
	int num = 0;
	while (isdigit(c = fgetc(f)))
	{
		num *= 10;
		num += c - '0';
	}
	//fseek(f, -1, SEEK_CUR);
	return num;
}

void ResetMode(FILE* f)
{
	fseek(f, 0, SEEK_CUR);
}

void MyCopyFile(FILE* dest, FILE* src)
{
	ResetMode(dest);
	ResetMode(src);
	char c = 0;
	while ((c = fgetc(src)) != EOF)
	{
		fputc(c, dest);
	}
	ResetMode(dest);
	ResetMode(src);
}

void MyCopyFileByPath(char* dest, char* src)
{
	FILE* fileDest = fopen(dest, "w");
	if (!CheckFilePointer(fileDest)) return;
	FILE* fileSrc = fopen(src, "r");
	if (!CheckFilePointer(fileSrc))
	{
		fclose(fileDest);
		return;
	}
	MyCopyFile(fileDest, fileSrc);
	fclose(fileDest);
	fclose(fileSrc);
}


void CopyToNewFile(char* fileNameToCopy, char* newName)
{
	FILE* src = fopen(fileNameToCopy, "r");
	if (!CheckFilePointer(src)) return;
	FILE* newCopy = fopen(newName, "w");
	if (!CheckFilePointer(newCopy))
	{
		fclose(src);
		return;
	}
	MyCopyFile(newCopy, src);
	fclose(src);
	fclose(newCopy);
}

char* GetCodeFilePath(char* exerciseNum, char* type, char* end)
{
	char* path = (char*)malloc(50 * sizeof(char));
	strcpy(path, "TASM\\Exers\\Exer");
	strcat(path, exerciseNum);
	strcat(path, "\\");
	strcat(path, type);
	strcat(path, ".");
	strcat(path, end);
	return path;
}

int FindFileLength(char* path)
{
	FILE* f = fopen(path, "r");
	if (!CheckFilePointer(f)) return -1;
	int length = 0;
	char c = 0;
	while ((c = fgetc(f)) != EOF)
	{
		length++;
	}
	fclose(f);
	return length;
}

void TruncateFromEnd(char* path, int n)
{
	int fileLength = FindFileLength(path);
	FILE* original = fopen(path, "r");
	if (!CheckFilePointer(original)) return;
	FILE* copy = fopen("temp.txt", "w");
	if (!CheckFilePointer(copy))
	{
		fclose(original);
		return;
	}
	int i;
	for (i = 0; i < fileLength - n; i++)
	{
		char c = fgetc(original);
		fputc(c, copy);
	}
	fclose(copy);
	fclose(original);
	remove(path);
	rename("temp.txt", path);
}

double CalculateTimePassed(size_t start, size_t end)
{
	return ((double)end - (double)start) / CLOCKS_PER_SEC;
}