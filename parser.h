#ifndef PARSER_H
#define PARSER_H

void syntaxError (const char* file, size_t line, const char* function, char* strError)
{
    assert (file != nullptr);
    assert (function != nullptr);

    printf ("Filename: %s, line of program: %ld, function: <%s>\n", file, line, function);
    printf ("str mistake: <%s>\n", strError);

    exit (1);
}

#ifndef SYNTAXERROR
#define SYNTAXERROR(str) syntaxError (__FILE__, __LINE__, __PRETTY_FUNCTION__, str)
#endif 

size_t fileSize (FILE* text) 
{
    assert(text != nullptr);

    fseek(text, 0 , SEEK_END);
    size_t fSize = ftell(text);
    rewind(text);

    return fSize;
}

char* readTextFromFile(FILE* text, char* str) 
{
    assert(text != nullptr);
    assert(str != nullptr);
    
    size_t fSize = fileSize (text);

    fread(str, sizeof(*str), fSize, text);

    return str;
}

Node* getG (char* str);
Node* getE (char* str, int* currPointer);
Node* getT (char* str, int* currPointer);
Node* getP (char* str, int* currPointer);
Node* getN (char* str, int* currPointer);
Node* getV (char* str, int* currPointer);
Node* getTrigFunc (char* str, int* currPointer);

Node* getG (char* str)
{
    assert (str != nullptr);
    
    int currPointer = 0;

    Node* val = getE (str, &currPointer);

    if (*(str + currPointer) != '$')
        SYNTAXERROR (str);
    
    return val;
}

Node* getE (char* str, int* currPointer)
{
    assert (str != nullptr);
    assert (currPointer != nullptr);

    Node* val = getT (str, currPointer);

    while (*(str + *currPointer) == '+' || *(str + *currPointer) == '-')
    {
        int op = *(str + *currPointer);
        ++*currPointer;
        Node* secondVal = getT (str, currPointer);

        switch (op)
        {
            case '+':
                val = nodeCstrOper (op, val, secondVal);
                break;
            
            case '-':
                val = nodeCstrOper (op, val, secondVal);
                break;
            
            default:
                SYNTAXERROR (str);
                break;
        }
    }

    return val;
}

Node* getT (char* str, int* currPointer)
{
    assert (str != nullptr);
    assert (currPointer != nullptr);

    Node* val = getP (str, currPointer);

    while (*(str + *currPointer) == '*' || *(str + *currPointer) == '/')
    {
        int op = *(str + *currPointer);
        ++*currPointer;
        Node* secondVal = getP (str, currPointer);

        switch (op)
        {
            case '*':
                val = nodeCstrOper (op, val, secondVal);
                break;
            
            case '/':
                val = nodeCstrOper (op, val, secondVal);
                break;
            
            default:
                SYNTAXERROR (str);
                break;
        }
    }

    return val;
}

Node* getP (char* str, int* currPointer)
{
    assert (str != nullptr);
    assert (currPointer != nullptr);

    Node* val = nullptr;

    if (*(str + *currPointer) == '(')
    {
        ++*currPointer;

        Node* val = getE (str, currPointer);

        if (*(str + *currPointer) != ')')
            SYNTAXERROR (str);
        
        ++*currPointer;
        return val;
    }

    else if ((val = getV (str, currPointer)) != nullptr)
        return val;
    
    else if ((val = getTrigFunc (str, currPointer)) != nullptr)
        return val;
    
    return getN (str, currPointer);
}

int isLetter (char symbol)
{
    if (('a' <= symbol && symbol <= 'z') ||
        ('A' <= symbol && symbol <= 'Z'))
         return 1;
        
    return 0;
}

Node* getV (char* str, int* currPointer)
{
    assert (str != nullptr);
    assert (currPointer != nullptr);

    if (isLetter (*(str + *currPointer)) && !isLetter (*(str + *currPointer + 1)))
    {
        Node* val = nodeCstrVar (*(str + *currPointer));
        ++*currPointer;

        if (*(str + *currPointer) == '^')
        {
            *currPointer += 1;
            Node* powVal = getN (str, currPointer);
            val = nodeCstrOper ('^', val, powVal);
        }

        return val;
    }
    
    return nullptr;
}

Node* getTrigFunc (char* str, int* currPointer)
{
    assert (str != nullptr);
    assert (currPointer != nullptr);

    if ((*(str + *currPointer)) == 's' && *(str + *currPointer + 1) == 'i' &&
        *(str + *currPointer + 2) == 'n')
    {
        *currPointer += 3;
        
        Node* var = getP (str, currPointer);

        Node* val = nodeCstrOper('s', nullptr, var);
        return val;
    }

    else if ((*(str + *currPointer)) == 'c' && *(str + *currPointer + 1) == 'o' &&
        *(str + *currPointer + 2) == 's')
    {
        *currPointer += 3;

        Node* var = getP (str, currPointer);

        Node* val = nodeCstrOper('c', nullptr, var);
        return val;
    }

    return nullptr;
}

Node* getN (char* str, int* currPointer)
{
    assert (str != nullptr);
    assert (currPointer != nullptr);

    int val = 0;

    int bPointer = *currPointer;

    while ('0' <= *(str + *currPointer) && *(str + *currPointer) <= '9')
    {
        val = val * 10 + (*(str + *currPointer) - '0');
        ++*currPointer;
    }

    if (*currPointer == bPointer)
        SYNTAXERROR (str);

    return nodeCstrNumber (val);
}

int skipSpaces (char * str)
{
    assert (str != nullptr);

    int len = strlen (str);
    int i = 0;

    for (; i < len && ( *(str + i) == '\t' ||*(str + i) == ' '); ++i)
    {
        ;// emptybody
    }

    return i;
}

#endif  