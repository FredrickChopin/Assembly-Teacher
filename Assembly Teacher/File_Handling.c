#include "File_Handling.h"

int CheckFilePointer(FILE* f, char* place)
{
	if (f)
	{
		return 1;
	}
	else
	{
		char* def = "Error when opening file at ";
		char* error = (char*)malloc((strlen(def) + strlen(place) + 1) * sizeof(char));
		strcpy(error, def);
		strcat(error, place);
		ThrowError(error, 1);
		return 0;
	}
}

void ThrowError(char* str, int freeStr)
{
	printf("%s\n", str);
	system("pause");
	if (freeStr) free(str);
	CleanGarbageFiles();
	exit(1);
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
	if (!CheckFilePointer(fileDest, "MyCopyFileByPath")) return;
	FILE* fileSrc = fopen(src, "r");
	if (!CheckFilePointer(fileSrc, "MyCopyFileByPath"))
	{
		fclose(fileDest);
		return;
	}
	MyCopyFile(fileDest, fileSrc);
	fclose(fileDest);
	fclose(fileSrc);
}

char* GetCodeFilePath(char* exerciseNum, char* name, char* ext)
{
	//This program returns dynamically allocated memory
	char* path = (char*)malloc(50 * sizeof(char));
	strcpy(path, "TASM\\Exers\\Exer");
	strcat(path, exerciseNum);
	strcat(path, "\\");
	strcat(path, name);
	strcat(path, ".");
	strcat(path, ext);
	return path;
}

int FindFileLength(char* path)
{
	FILE* f = fopen(path, "r");
	if (!CheckFilePointer(f, "FindFileLength")) return -1;
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
	if (!CheckFilePointer(original, "TruncateFromEnd")) return;
	FILE* copy = fopen("temp.txt", "w");
	if (!CheckFilePointer(copy, "TruncateFromEnd"))
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

void CleanGarbageFiles()
{
	remove("stderr.txt");
	remove("stdout.txt");
	remove("TASM\\OUTPUT.txt");
	remove("TASM\\TASM_OUT.txt");
	remove("Configuration.conf");
}

PROCESS_INFORMATION MyCreateProcess(char* exeName, char* args)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// create argument string
	INT size = strlen(exeName) + strlen(args) + 2;
	PCHAR param = (PCHAR)malloc(size * sizeof(CHAR));
	sprintf(param, "%s %s", exeName, args);
	// Start the child process.
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

double CalculateTimePassed(size_t start, size_t end)
{
	return ((double)end - (double)start) / CLOCKS_PER_SEC;
}