#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int CheckFilePointer(FILE* f);
int GoToNextLine(FILE* f);
int ScanIntFromFile(FILE* f);
void ResetMode(FILE* f);
void MyCopyFile(FILE* dest, FILE* src);
void CopyToNewFile(char* fileNameToCopy, char* newName);
char* GetCodeFilePath(char* exerciseNum, char* type, char* end);
void MyCopyFileByPath(char* dest, char* src);
FindFileLength(char* path);
void TruncateFromEnd(char* path, int n);