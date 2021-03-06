#include "File_Handling.h"

int CheckFilePointer(FILE* f, char* place)
{
	/// <summary>
	/// Checks if the file has opened successfully, throws an erorr if not
	/// </summary>
	/// <param name="f"> FILE pointer which was recently opened</param>
	/// <param name="place"> function name where the file was opened</param>
	/// <returns> Boolean value determining if the file has opened successfully </returns>
	if (f)
	{
		return 1;
	}
	else
	{
		printf("\nError when opening file at %s", place);
		return 0;
	}
}

void ThrowError(char* str, int freeStr)
{
	/// <summary>
	/// Throws an error, i.e prints erorr message exits the program
	/// </summary>
	/// <param name="str"> The error message</param>
	/// <param name="freeStr"> Wether the error message must be freed or not </param>
	printf("\n%s\n", str);
	system("pause");
	if (freeStr) free(str);
	CleanGarbageFiles();
	exit(1);
}

int GoToNextLine(FILE* f)
{
	/// <summary>
	/// Advances the file pointer to the next line
	/// </summary>
	/// <param name="f"> </param>
	/// <returns></returns>
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
	/// <summary>
	/// Scans an int from a file
	/// </summary>
	/// <param name="f"> file pointer</param>
	/// <returns> The scanned int </returns>
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
	/// <summary>
	/// Resets the file mode (to switch from read to write and vice versa)
	/// </summary>
	/// <param name="f"> File pointer </param>
	fseek(f, 0, SEEK_CUR);
}

void MyCopyFile(FILE* dest, FILE* src)
{
	/// <summary>
	/// Copies the destenation file into source file
	/// dest must be readable and src must be writable
	/// </summary>
	/// <param name="dest"> Destination file pointer </param>
	/// <param name="src"> Source file pointer </param>
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
	/// <summary>
	/// Writes what ever is in the file with path dest to a file (new or old) with path src
	/// </summary>
	/// <param name="dest"> Destination file pointer </param>
	/// <param name="src"> Source file pointer </param>
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
	/// <summary>
	/// </summary>
	/// <param name="exerciseNum"></param>
	/// <param name="name"></param>
	/// <param name="ext"></param>
	/// <returns> Dynamically allocated string which hold the path to the wanted code file</returns>
	char* path = (char*)malloc(50 * sizeof(char));
	if (!path)
	{
		ThrowError("Unable to allocate dynamic memory in GetCodeFilePath", 0);
		return NULL;
	}
	sprintf(path, "TASM\\Exers\\Exer%s\\%s.%s", exerciseNum, name, ext);
	return path;
}

int FindFileLength(char* path)
{
	/// <summary>
	/// Finds the file length at a specified path
	/// </summary>
	/// <param name="path"> Path</param>
	/// <returns> File length</returns>
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

//This function is unused
void TruncateFromEnd(char* path, int n)
{
	/// <summary>
	/// Truncates n chars from the end of the file with the specified path
	/// </summary>
	/// <param name="path"> File path</param>
	/// <param name="n"> Number of characters to truncate</param>
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
	/// <summary>
	/// Cleans "garbage" files created during the program's life
	/// This function must be called before exiting the program
	/// </summary>
	remove("stderr.txt");
	remove("stdout.txt");
	remove("TASM\\OUTPUT.txt");
	remove("TASM\\TASM_OUT.txt");
	remove("Configuration.conf");
}

double CalculateTimePassed(size_t start, size_t end)
{
	/// <summary>
	/// Calculates the seconds passed from start to end, where start and end
	///  were initialized using the function clock()
	/// </summary>
	/// <param name="start"> Start</param>
	/// <param name="end"> End </param>
	/// <returns> Time passed in seconds </returns>
	return ((double)end - (double)start) / CLOCKS_PER_SEC;
}

void OpenFileToUser(char* exerciseNum, char* nameOfFile, char* ext)
{
	/// <summary>
	/// 
	/// </summary>
	/// <param name="exerciseNum"> The exercise number</param>
	/// <param name="nameOfFile"> The name of the file, e.g Code</param>
	/// <param name="ext"> The extension of the file</param>
	char* path = GetCodeFilePath(exerciseNum, nameOfFile, ext);
	system(path);
	free(path);
}