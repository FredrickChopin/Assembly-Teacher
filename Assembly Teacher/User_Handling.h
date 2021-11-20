#pragma once
#include "Libaries.h"
//#include "File_Handling.h"
#include "Code_Handling.h"

//This file handles the work with the user
//Input, output, menus and options

void LetUserEdit(char* exerciseNum);
int CountAssemblingErrors();
void AssembleCodeToUser(char* exerciseNum);
void GetCodeFromUser(char* exerciseNum);
void TestCodeToUser(char* exercsieNum);
double CalculateTimePassed(size_t start, size_t end);
int GetIntInRange(char* buff, int buffSize, int top);
void EndProgram();
void MainMenu();
void ExerciseMenu(char* exerciseNum);
void DisplayErrors(int errorCount, char* exerciseNum);
void ResetCodeFile(char* exerciseNum);