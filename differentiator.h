#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdlib.h>

#include <assert.h>

//*********************************************************

#include "filefunctions.h"
#include <math.h>

//*********************************************************

#define UNKNOWNTYPE 177

enum side {LEFT, RIGHT, SAME};

enum  type {UNKNOWN, NUMBER, VARIABLE, OPERATOR};

enum operators {UNKNOWN_OP, PLUS = '+'    , MINUS = '-'  , 
                            DIV = '/'     , MULT = '*'   , 
                            SIN = 's'     , COS = 'c'    ,
                            POW = '^'};

//*********************************************************

struct Type
{
    int number;
    char var;
    int oper;
};

struct Node 
{
    struct Type val;
    int type;

    Node* left;
    Node* right;
};

struct Tree 
{
    Node* head;

    int size;
};

void treeGraph (Tree* tree);

//**********************************

Tree* treeCstr (Node* head);

void treeDstr (Tree * tree);

void fullTreeDstr (Node* node);

//**********************************

Node* nodeCstrNumber (int val);

Node* nodeCstrVar (char val);

Node* nodeCstrOper (int val, Node* left, Node* right);

//**********************************

void nodeDstr (Node* node);

//**********************************

Tree* treeCstr (Node* head)
{
    assert (head != nullptr);
    
    Tree* newTree = (Tree*) calloc (1, sizeof (*newTree));
    newTree->head = head;

    newTree->size = 1;

    return newTree;
}

//**********************************

void treeDstr (Tree * tree)
{
    assert (tree != nullptr);

    free (tree);
}

//**********************************

void fullTreeDstr (Node* node)
{
    assert (node != nullptr);

    if (node->left)
        fullTreeDstr (node->left);
    
    if (node->right)
        fullTreeDstr (node->right);
    
    free (node);
}

//**********************************

void printNodeInf (Node* node);


Node* nodeCstrNumber (int val)
{
    Node* newNode = (Node* ) calloc (1, sizeof(*newNode));

    newNode->type = NUMBER;

    newNode->val.oper = UNKNOWNTYPE;
    newNode->val.var =  UNKNOWNTYPE;
    newNode->val.number = val;

    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

Node* nodeCstrVar (char val)
{
    Node* newNode = (Node* ) calloc (1, sizeof(*newNode));

    newNode->type = VARIABLE;

    newNode->val.var = val;
    newNode->val.number = UNKNOWNTYPE;
    newNode->val.oper = UNKNOWNTYPE;

    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

Node* nodeCstrOper (int val, Node* left, Node* right)
{
    Node* newNode = (Node* ) calloc (1, sizeof(*newNode));

    newNode->type = OPERATOR;

    if (val == PLUS || val == MINUS || val == DIV || val == MULT ||
        val == SIN  || val == COS   || val == POW)
    {
        newNode->val.oper = val;
    }

    else 
        newNode->val.oper = UNKNOWNTYPE;

    newNode->val.number = UNKNOWNTYPE;
    newNode->val.number = UNKNOWNTYPE;

    newNode->left = left;
    newNode->right = right;

    return newNode;
}

//**********************************

void nodeDstr (Node* node)
{
    assert (node != nullptr);

    if (node->left)
        nodeDstr (node->left);

    if (node->right)
        nodeDstr (node->right);

    free (node);
}

//**********************************

void nodeOneDstr (Node* node)
{
    assert (node != nullptr);

    free (node);
}

//**********************************

Node* copy (Node* node)
{
    if (!node)
        return nullptr;

    Node* nodeLeft  = copy (node->left);
    Node* nodeRight = copy (node->right);

    Node* newNode = (Node* ) calloc (1, sizeof(*newNode));

    newNode->left  = nodeLeft;
    newNode->right = nodeRight;

    newNode->type = node->type;

    newNode->val.number = node->val.number;
    newNode->val.oper   = node->val.oper;
    newNode->val.var    = node->val.var;

    return newNode;
}

//**********************************

#define NUM (value)                           \
     nodeCstrNumber (node->val.number, value);

Node* diff (Node* node)
{
    assert (node);

    switch (node->type)
    {
        case NUMBER: 
            return nodeCstrNumber (0);
        
        case VARIABLE:
            return nodeCstrNumber (1);

        case OPERATOR:
            switch (node->val.oper)
            {
                case PLUS:
                    return nodeCstrOper (PLUS, diff (node->left), diff (node->right));

                case MINUS:
                    return nodeCstrOper (MINUS, diff (node->left), diff (node->right));

                case MULT:
                    return nodeCstrOper (PLUS, nodeCstrOper (MULT, diff (node->left), copy (node->right)),
                                               nodeCstrOper (MULT, copy (node->left), diff (node->right)));

                case DIV: 
                    return nodeCstrOper (DIV, nodeCstrOper (MINUS, 
                                              nodeCstrOper (MULT, diff (node->left), copy (node->right)), 
                                              nodeCstrOper (MULT, copy (node->left), diff (node->right))),

                                              nodeCstrOper (MULT,
                                              copy (node->right), copy (node->right)));

                case COS:
                    return nodeCstrOper (MULT, nodeCstrOper (MULT, nodeCstrOper (SIN, nullptr, copy (node->right)), nodeCstrNumber (-1)),
                                               diff (node->right));

                case SIN:
                    return nodeCstrOper (MULT, nodeCstrOper (COS, nullptr, copy(node->right)),
                                               diff (node->right));
                
                case POW:
                {
                    if (node->left->type == VARIABLE && node->right->type == NUMBER)
                        return nodeCstrOper (MULT, copy(node->right), 
                                            nodeCstrOper (POW, copy(node->left), nodeCstrNumber (node->right->val.number -1)));

                }

                default:
                    {
                        printf ("Unknown operator");
                        printNodeInf (node);
                        return nullptr;
                    }
            }
        
        printf ("Unknown type");
        return nullptr;
    }

    printf ("Erroooor");
    return nullptr;
}

//**********************************

void inOrderPrintToFile (FILE* file, const Node* node)
{
    assert (node != nullptr);

    fprintf (file, "%s", "(");

    if (node->left)
        inOrderPrintToFile (file, node->left);

    if (node->type == OPERATOR)
        fprintf (file, "%c", node->val.oper);

    else if (node->type == NUMBER)
        fprintf (file, "%d", node->val.number);
    
    else if (node->type == VARIABLE)
        fprintf (file, "%c", node->val.var);

    if (node->right)
        inOrderPrintToFile (file, node->right);
   
   fprintf (file, "%s", ")");
}

//**********************************

void printNodeInf (Node* node)
{
    if (node == nullptr)
        printf ("Nullptr node\n");

    else    
    {
        if (node->type == NUMBER)
            printf ("Node type is number, val: <%d>\n", node->val.number);
        
        else if (node->type == VARIABLE)
            printf ("Node type is var, it is var: <%c>\n", node->val.var);

        else if (node->type == OPERATOR)
            printf ("Node type is operator, it is operator: <%c>\n", node->val.oper);
    }

    if (node->left != nullptr)
        printNodeInf (node->left);

    if (node->right != nullptr)
        printNodeInf (node->right);

}

//**********************************

void optimisationNumbers (Node* node)
{
    if (node == nullptr)
        return;

    if (node->left != nullptr && node->left->type == OPERATOR)
        optimisationNumbers (node->left);

    
    if (node->right != nullptr && node->right->type == OPERATOR)
        optimisationNumbers (node->right);

    if (node->type == OPERATOR && node->left != nullptr && node->right != nullptr && 
        node->left->type == NUMBER && node->right->type == NUMBER)
    {
        int success = 1;

        if (node->val.oper == PLUS)
            node->val.number = node->left->val.number + node->right->val.number;

        else if (node->val.oper == MINUS)
            node->val.number = node->left->val.number - node->right->val.number;

        else if (node->val.oper == DIV)
            node->val.number = node->left->val.number / node->right->val.number;

        else if (node->val.oper == MULT)
            node->val.number = node->left->val.number * node->right->val.number;

        else if (node->val.oper == SIN)
            node->val.number = (int) sin((double) node->right->val.number);

        else if (node->val.oper == COS)
            node->val.number = (int) cos((double) node->right->val.number);

        else
        {
            printf ("Error. Unknown operation");
            success = 0;
        }

        if (success)
        {
            node->val.oper = UNKNOWN_OP;

            node->type = NUMBER;

            nodeDstr (node->left);
            nodeDstr (node->right);

            node->left = nullptr;
            node->right = nullptr;
        }
    }
}

//**********************************

int isZeroDesc (Node* node)
{
    assert (node != nullptr);

    if ((node->left->type  == NUMBER && node->left->val.number == 0) || 
        (node->right->type == NUMBER && node->right->val.number == 0))
        return YES;

    return 0;
}

//**********************************

void optimisationMultOnZero (Node* node)
{
    if (node == nullptr)
        return;

    if (node->type == OPERATOR && node->val.oper == MULT && isZeroDesc (node))
    {
        fullTreeDstr (node->left);
        fullTreeDstr (node->right); 
        
        node->left  = nullptr;
        node->right = nullptr;

        node->type = NUMBER;
        node->val.number = 0;
    }

    else
    {
        if (node->left != nullptr && node->left->type == OPERATOR)
            optimisationMultOnZero (node->left);
        
        if (node->right != nullptr && node->right->type == OPERATOR)
            optimisationMultOnZero (node->right);
    }

}

//**********************************

int equalExpression (Node* fNode, Node* sNode)
{
    assert (fNode != nullptr);
    assert (sNode != nullptr);

    if (fNode->type == VARIABLE && sNode->type == VARIABLE &&
        fNode->val.var == sNode->val.var)
        return YES;
    
    else if (fNode->type == NUMBER && sNode->type == NUMBER &&
             fNode->val.number == sNode->val.number)
        return YES;

    return NO;
}

//**********************************

void optimisationEquals (Node* node)
{
    assert (node != nullptr);

    if (node->type == OPERATOR && node->val.oper == MINUS && equalExpression (node->left, node->right))
    {
        fullTreeDstr (node->left);
        fullTreeDstr (node->right);

        node->left = nullptr;
        node->right = nullptr;

        node->type = NUMBER;
        node->val.number = 0;
    }

    else if (node->left != nullptr && node->left->type == OPERATOR)
        optimisationEquals (node->left);
    
    else if (node->right != nullptr && node->right->type == OPERATOR)
        optimisationEquals (node->right);
}

//**********************************

int allDescNullPtr (Node* node)
{
    assert (node != nullptr);

    if (node->left == nullptr || node->right == nullptr)
        return YES;

    return NO; 
}

//**********************************

int multVarOnOne (Node* node)
{
    assert (node != nullptr);

    if (node->type == OPERATOR && node->val.oper == MULT && !allDescNullPtr (node) &&
        ((node->left->type == VARIABLE && node->right->type == NUMBER && node->right->val.number == 1) ||
        (node->right->type == VARIABLE && node->left->type == NUMBER && node->left->val.number == 1)))
        return YES;
    
    return NO;
}

//**********************************

void optimisationMultOnOne (Node* node)
{
    assert (node != nullptr);

    if (node->left != nullptr && node->left->type == OPERATOR)
        optimisationMultOnOne (node->left);
        
    if (node->right != nullptr && node->right->type == OPERATOR)
        optimisationMultOnOne (node->right);

    if (multVarOnOne (node))
    {
        if (node->left->type == VARIABLE)
            node->val.var = node->left->val.var;
        
        if (node->right->type == VARIABLE)
            node->val.var = node->right->val.var;

        node->type = VARIABLE;

        fullTreeDstr (node->left);
        fullTreeDstr (node->right);

        node->left = nullptr;
        node->right = nullptr;
    }
}

//**********************************

int multOpOnOne (Node* node)
{
    assert (node != nullptr);

    if (node->type == OPERATOR && node->val.oper == MULT && !allDescNullPtr (node) &&
        ((node->left->type == OPERATOR && node->right->type == NUMBER && node->right->val.number == 1) ||
        (node->right->type == OPERATOR && node->left->type == NUMBER && node->left->val.number == 1)))
        return YES;
    
    return NO;
}

//**********************************

void delFullSideTree (Node* node, int side)
{
    assert (node != nullptr);

    if (side == LEFT)
        fullTreeDstr (node->left); 
    
    else if (side == RIGHT)
        fullTreeDstr (node->right);
    
    else 
        printf ("Unknown side\n");

    free (node);
}

//**********************************

void optimisationMultOpOnOne  (Node* node, Node* prevNode, int side)
{
    assert (node != nullptr);

   if (node->left != nullptr && node->left->type == OPERATOR)
        optimisationMultOpOnOne (node->left, node, LEFT);

    if (node->right != nullptr && node->right->type == OPERATOR)
        optimisationMultOpOnOne (node->right, node, RIGHT);
    
    if (multOpOnOne (node))
    {
        if (node->left->type == OPERATOR)
        {
            if (side == LEFT)
            {
                if (prevNode == nullptr)
                {
                    Node* ptrOnNode = node;
                    node = node->left;
                    delFullSideTree (ptrOnNode, RIGHT);
                }

                else
                {
                    prevNode->left = node->left;
                    delFullSideTree (node, RIGHT);
                }
            }

            else if (side == RIGHT)
            {
                if (prevNode == nullptr)
                {
                    Node* ptrOnNode = node;
                    node = node->left;
                    delFullSideTree (ptrOnNode, RIGHT);
                }

                else
                {
                    prevNode->right = node->left;
                    delFullSideTree (node, RIGHT);
                }
            }
            
            else if (side == SAME)
            {
                // Node* ptrOnNode = node;
                // node = node->left;
                
                // delFullSideTree (ptrOnNode, RIGHT);
            }

            else
                printf ("Unknown side\n");
        }

        else if (node->right->type == OPERATOR)
        {
            if (side == LEFT)
            {
                if (prevNode == nullptr)
                {
                    Node* ptrOnNode = node;
                    node = node->right;
                    delFullSideTree (ptrOnNode, LEFT);
                }

                else
                {
                    prevNode->left = node->right;
                    delFullSideTree (node, LEFT);
                }
            }

            else if (side == RIGHT)
            {
                if (prevNode == nullptr)
                {
                    Node* ptrOnNode = node;
                    node = node->right;
                    delFullSideTree (ptrOnNode, LEFT);
                }

                else
                { 
                    prevNode->right = node->right;
                    delFullSideTree (node, LEFT);
                }
            }
            
            else if (side == SAME)
            {
                // Node* ptrOnNode = node;
                // node = node->right;
                // delFullSideTree (ptrOnNode, LEFT);
            }

            else
                printf ("Unknown side!\n");
        }
    }
}

//**********************************

int powVarInOne (Node* node)
{
    assert (node != nullptr);

    if (node->type == OPERATOR && node->val.oper == POW && !allDescNullPtr (node) &&
        ((node->left->type == VARIABLE && node->right->type == NUMBER && node->right->val.number == 1) ||
         (node->right->type == VARIABLE && node->left->type == NUMBER && node->left->val.number == 1)))
         return YES;
        
    return NO;
}

//**********************************

void optimisationPowOne (Node* node)
{
    assert (node != nullptr);

    if (node->left != nullptr && node->left->type == OPERATOR)
        optimisationPowOne (node->left);
        
    if (node->right != nullptr && node->right->type == OPERATOR)
        optimisationPowOne (node->right);

    if (!allDescNullPtr (node) && powVarInOne (node))
    {
        if (node->left->type == VARIABLE)
            node->val.var = node->left->val.var;
        
        else if (node->right->type == VARIABLE)
            node->val.var = node->right->val.var;

        node->type = VARIABLE;

        fullTreeDstr (node->left);
        fullTreeDstr (node->right);

        node->left = nullptr;
        node->right = nullptr;
   }
}

//**********************************

int plusZero (Node* node)
{
    if (node->type == OPERATOR && node->val.oper == PLUS &&
       ((node->left->type == NUMBER  && node->left->val.number == 0) ||
       (node->right->type == NUMBER && node->right->val.number == 0)))
        return YES;

    return NO;                  
}

//**********************************

void assignValue (Node* node1, Node* node2)
{
    assert (node1 != nullptr);
    assert (node2 != nullptr);

    if (node1->type == OPERATOR)
        node1->val.oper = node2->val.oper;
    
    if (node1->type == NUMBER) 
        node1->val.number = node2->val.number;
    
    if (node1->type == VARIABLE)
        node1->val.var = node2->val.var;
}

//**********************************

void optimizationPlusZero (Node* node)
{
    assert (node != nullptr);

    if (node->left != nullptr && node->left->type == OPERATOR)
        optimizationPlusZero (node->left);
        
    if (node->right != nullptr && node->right->type == OPERATOR)
        optimizationPlusZero (node->right);

    if (!allDescNullPtr (node) && plusZero (node))
    {
        if (node->left->type == NUMBER)
        {
            Node* copyLeftNode  = node->left;
            Node* copyRightNode = node->right;

            node->left = copyRightNode->left;
            node->right = copyRightNode->right;

            node->type = copyRightNode->type;
            assignValue (node, copyRightNode);
            
            nodeOneDstr (copyLeftNode);
            nodeOneDstr (copyRightNode);
        }
        
        else if (node->right->type == NUMBER)
        {
            Node* copyLeftNode  = node->left;
            Node* copyRightNode  = node->right;

            node->left = copyLeftNode->left;
            node->right = copyLeftNode->right;

            node->type = copyLeftNode->type;
            assignValue (node, copyLeftNode);
            
            nodeOneDstr (copyLeftNode);
            nodeOneDstr (copyRightNode);
        }
   }
}


//**********************************

void optimisation (Node* node)
{
    assert (node != nullptr);

    optimisationNumbers       (node);
    optimisationMultOnZero    (node);
    optimisationEquals        (node);
    optimisationMultOnOne     (node);
    optimizationPlusZero      (node);
    optimisationPowOne        (node);
    optimisationMultOpOnOne   (node, nullptr, SAME);
}

//**********************************

void writeArithmetic (FILE* file, Node* node)
{
    assert (file != nullptr);
    assert (node != nullptr);

    if (node->val.oper == MINUS)
        fprintf (file, "-");
    
    else if (node->val.oper == PLUS)
        fprintf (file, "+");
    
    else if (node->val.oper == MULT)
        fprintf (file, "\\cdot");
}

//**********************************

void printTreeToTex (FILE* file, Node* node)
{
    assert (file != nullptr);
    assert (node != nullptr);

    if (node->type == OPERATOR && (node->val.oper == SIN || node->val.oper == COS || node->val.oper == POW ||
                                   node->val.oper == DIV))
    {
        if (node->val.oper == SIN)
        {
            fprintf (file, " sin{(");
            printTreeToTex (file, node->right);
            fprintf (file, ")}");
        }

        else if (node->val.oper == COS)
        {
            fprintf (file, " cos{(");
            printTreeToTex (file, node->right);
            fprintf (file, ")}");
        }
        
        else if (node->val.oper == POW)
        {
            fprintf (file, "{");
            printTreeToTex (file, node->left);
            fprintf (file, "}");
            fprintf (file, "^");
            printTreeToTex (file, node->right);
        }

        else if (node->val.oper == DIV)
        {
            fprintf (file, "\\frac{");
            printTreeToTex (file, node->left);
            fprintf (file, "}{");
            printTreeToTex (file, node->right);
            fprintf (file, "}");
        }
    }

    if (node->type == OPERATOR && !(node->val.oper == SIN || node->val.oper == COS || node->val.oper == POW ||
                                    node->val.oper == DIV))
    {
        printTreeToTex (file, node->left);
        writeArithmetic (file, node);
        printTreeToTex (file, node->right);
    }

    if (node->type == VARIABLE)
        fprintf (file, "%c", node->val.var);

    if (node->type == NUMBER)
    {
        if (node->val.number < 0)
            fprintf (file, "(%d)", node->val.number);
        
        else
            fprintf (file, "%d", node->val.number);
    }
}

//**********************************

void printToLatex (Node* node)
{
    assert (node != nullptr);

    FILE* texFile = fopen("texFile.txt", "w");

    fprintf (texFile,   "\\documentclass{article}\n"

                        "\\usepackage[english]{babel}\n"
                        "\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\n"

                        "\\usepackage{amsmath}\n"
                        "\\usepackage{graphicx}\n"
                        "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n"

                        "\\title{Your Paper}\n"
                        "\\author{You}\n");
    
    fprintf (texFile, "\\begin{document}\n");
    fprintf (texFile, "$");

    printTreeToTex (texFile, node);

    fprintf (texFile, "$\n");
    fprintf (texFile, "\\end{document}\n");
}

//**********************************


#endif