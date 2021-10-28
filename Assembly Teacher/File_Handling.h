#include "Libaries.h"

//This file contains general and specific functions that deal with files
//And also other general functions

int CheckFilePointer(FILE* f, char* place);
int GoToNextLine(FILE* f);
int ScanIntFromFile(FILE* f);
void ResetMode(FILE* f);
void MyCopyFile(FILE* dest, FILE* src);
char* GetCodeFilePath(char* exerciseNum, char* name, char* ext);
void MyCopyFileByPath(char* dest, char* src);
int FindFileLength(char* path);
void TruncateFromEnd(char* path, int n);
void ThrowError(char* str, int freeStr);
void CleanGarbageFiles();
PROCESS_INFORMATION MyCreateProcess(char* exeName, char* args, int hidden);