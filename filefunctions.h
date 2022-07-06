#include <string.h>
#include <assert.h>
#include <stdio.h>

enum correct {NO = 0, YES = 1};

void scanInputName(char* str);
void scanOutputName(char* resultSortedFileName);
int correctFileName(char* fileName);

void printTextToFile(const char* const* const arrayOfptrOnStrings, const size_t linesNumber, FILE* resultSortedFile);
char* readTextFromFile(FILE* text, char* str, const size_t fileSize);

size_t sizeOfFile(FILE* text);

int symbolAmount(const char* const str, const size_t fileSize, const char symbol);
int strIsGood(const char* str);
int isGoodSymbol (const char symbol);

void fillArrayOfPtrOnStrings(char** arrayOfptrOnStrings, char* str, const size_t fileSize);

int symbolAmount(char* str, const size_t fileSize, const char symbol) 
{
    assert(str != nullptr);

    int nSymbols = 0;

    for(size_t i = 0; i < fileSize; ++i) 
    {
        if(*str++ == symbol) 
            ++nSymbols;
    }

    return nSymbols;
}

void fillArrayOfPtrOnStrings(char** arrayOfptrOnStrings, char* str, const size_t fileSize) {
    arrayOfptrOnStrings[0] = str;

    int currentLine = 1;

    for(size_t j = 0; j < fileSize; ++j) {
        if (str[j] == '\n') {
            str[j] = '\0';

            if (j + 1 != fileSize) {
                arrayOfptrOnStrings[currentLine] = str + j + 1;
                ++currentLine;
            }
        }
    }
}

void scanInputName (char* str) {
    printf("Please, enter input file name: \n");
    scanf("%s", str);

    if (!correctFileName(str)) {
        printf("File name is not correct.\n");
        scanInputName(str);
    }
}

void scanOutputName (char* resultSortedFileName) {
    printf("Please, enter the name of output file: \n");
    scanf("%s", resultSortedFileName);

    if (!correctFileName(resultSortedFileName)) {
        printf("File name is not correct.\n");
        scanOutputName(resultSortedFileName);
    }
}

int correctFileName (char* fileName) {
    assert(fileName != nullptr);

    size_t sizeOfFileName = strlen(fileName);

    if (sizeOfFileName <= 4) {
        return NO;
    }

    if (strcmp(fileName + strlen(fileName) - 4, ".txt") != 0) {
        return 0;
    }

    return YES;
}

void printTextToFile(const char* const* const arrayOfptrOnStrings, const size_t linesNumber, FILE* resultSortedFile) {
    assert(arrayOfptrOnStrings != nullptr);
    assert(linesNumber >= 0);

    const char* emptyString = "\n";
    for(size_t Index = 0; Index < linesNumber; ++Index) 
    {
        if (*(arrayOfptrOnStrings + Index) != nullptr && strIsGood(*(arrayOfptrOnStrings + Index)) &&
            strlen(*(arrayOfptrOnStrings + Index)) != 0) 
        {
            printf("<%s>\n", *(arrayOfptrOnStrings + Index));
            
            fputs(*(arrayOfptrOnStrings + Index), resultSortedFile);
            fputs(emptyString, resultSortedFile);
        }
    }

    fputs(emptyString, resultSortedFile);
}

char* readTextFromFile(FILE* text, char* str, const size_t fileSize) {
    assert(text != nullptr);
    assert(str != nullptr);

    fread(str, sizeof(*str), fileSize, text);

    return str;
}

size_t sizeOfFile (FILE* text) {
    assert(text != nullptr);

    fseek(text, 0 , SEEK_END);
    size_t fileSize = ftell(text);
    rewind(text);

    return fileSize;
}

int strIsGood(const char* str) { //is str is garbage
    int index = 0;

    while((!isGoodSymbol(*(str + index)) && *(str + index) != '\0')) 
    {
        ++index;
    }

    if (*(str + index) == '\0') {
        return 0;
    }

    return 1;
}

int isGoodSymbol (const char symbol) {
    return (('A' <= symbol && symbol  <= 'Z') || ('a' <= symbol && symbol <= 'z'));
}