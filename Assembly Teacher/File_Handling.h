#include "Libaries.h"

int CheckFilePointer(FILE* f);
int GoToNextLine(FILE* f);
int ScanIntFromFile(FILE* f);
void ResetMode(FILE* f);
void MyCopyFile(FILE* dest, FILE* src);
void CopyToNewFile(char* fileNameToCopy, char* newName);
char* GetCodeFilePath(char* exerciseNum, char* type, char* end);
void MyCopyFileByPath(char* dest, char* src);
int FindFileLength(char* path);
void TruncateFromEnd(char* path, int n);
void ThrowError(char* str);
void CleanGarbageFiles();