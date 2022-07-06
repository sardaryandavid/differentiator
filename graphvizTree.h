#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include <stdio.h>
#include <assert.h>
#include "differentiator.h"

void  treeBegin      (FILE* graphfile);
void treeEnd        (FILE* graphfile);
void addNodeGr      (FILE* graphfile, const Node* node);
void treeRel        (FILE* graphfile, const Node* currNode);
void addAllNodes    (FILE* graphfile, const Node* currNode);

void treeBegin (FILE* graphfile)
{
    assert (graphfile != nullptr);

    fprintf (graphfile, "digraph G{\n");
}

void addNodeGr (FILE* graphfile, const Node* node)
{
    assert (graphfile != nullptr);
    assert (node != nullptr);

    if (node->type == NUMBER)
    {
        fprintf(graphfile, "   Node%p  [shape = \"ellipse\", style=\"filled\", fillcolor = \"goldenrod1\"," 
                           "label = \" %d \"];\n", 
                           node, node->val.number);
    }

    else if (node->type == VARIABLE)
    {
            fprintf(graphfile, "   Node%p  [shape = \"octagon\", style=\"filled\", fillcolor = \"pink2\","
                               "label = \" %c \"];\n", 
                               node, node->val.var);
    }

    else if (node->type == OPERATOR)
    {
            if (node->val.oper == PLUS || node->val.oper == MINUS || node->val.oper == DIV || node->val.oper == MULT)
                fprintf(graphfile,  "   Node%p  [shape = \"pentagon\", style=\"filled\", fillcolor = \"lightcyan2\","  
                                    "label = \" %c \"];\n", 
                                    node, node->val.oper);

            else if (node->val.oper == SIN)
                fprintf(graphfile,  "   Node%p  [shape = \"hexagon\", style=\"filled\", fillcolor = \"#8f12d9\","  
                                    "label = \" sin \"];\n", node);

            else if (node->val.oper == COS)
                fprintf(graphfile,  "   Node%p  [shape = \"hexagon\", style=\"filled\", fillcolor = \"#8f12d9\","  
                                    "label = \" cos \"];\n", node);

            else 
                fprintf(graphfile,  "   Node%p  [shape = \"circle\", style=\"filled\", fillcolor = \"#12abd9\","  
                                    "label = \" %c \"];\n", 
                                    node, node->val.oper);
    }
}

void treeRel (FILE* graphfile, const Node* currNode)
{
    assert (graphfile != nullptr);
    assert (currNode != nullptr);

    if (currNode->left)
    {
        fprintf (graphfile, "   edge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n");
        fprintf (graphfile, "   Node%p -> Node%p;\n", 
                                currNode, currNode->left);
        treeRel (graphfile, currNode->left);
    }

    if (currNode->right)
    {
        fprintf (graphfile, "   edge[color=\"darkblue\",fontcolor=\"blue\",fontsize=12];\n");
        fprintf (graphfile, "   Node%p -> Node%p;\n", 
                                currNode, currNode->right);
        treeRel (graphfile, currNode->right);
    }
}

void addAllNodes (FILE* graphfile, const Node* currNode)
{
    assert (graphfile != nullptr);
    assert (currNode != nullptr);

    addNodeGr (graphfile, currNode);

    if (currNode->left)
        addAllNodes (graphfile, currNode->left);

    if (currNode->right)
        addAllNodes (graphfile, currNode->right);
}

void treeGraph (Tree* tree)
{
    assert (tree != nullptr);

    FILE * graphfile = fopen ("treeGr.txt", "w");

    treeBegin (graphfile);

    addAllNodes (graphfile, tree->head);

    system ("dot -Tjpg treeGr.txt > treeGr.jpg");

    treeRel (graphfile, tree->head);
    treeEnd  (graphfile);

    fclose (graphfile);
}

void treeEnd (FILE* graphfile) 
{
    assert (graphfile != nullptr);

    fprintf (graphfile, "}");
}

#endif 