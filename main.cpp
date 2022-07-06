#include <stdio.h>

#include "differentiator.h"
#include "graphvizTree.h"
#include "parser.h"

void launchProgram (Tree* tree);

int main()
{
    FILE* expr = fopen ("expression.txt", "r");
  
    char* str = (char*) calloc (fileSize (expr) + 1, sizeof (*str));
    readTextFromFile (expr, str);

    Node* head = getG (str);
    Tree* tree = treeCstr (head);

    launchProgram (tree);
    
    fclose (expr);
    free (str);

    return 0;
}

void launchProgram (Tree* tree)
{
    assert (tree != nullptr);

    Node* newNode = diff (tree->head);

    Tree* newTree = treeCstr (newNode);

    //inOrderPrintToFile (file, tree->head);

    optimisation (newNode);
    //optimisation (newTree->head);

    treeGraph (newTree);
    printToLatex (newNode);
    
    fullTreeDstr (newNode);
    treeDstr (newTree);

    fullTreeDstr (tree->head);
    treeDstr (tree);
}