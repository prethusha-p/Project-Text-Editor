#ifndef EDITOR_H
#define EDITOR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node{
    char * line;
    struct node * next;
    struct node * prev;
}Node;

typedef struct matches{
    int * positions;
    int count;
}Linematches;

void editor_init();
Node * editor_head();
Node * editor_tail();
Node * editor_cursor();
void editor_move_cursor(char dir);
void editor_insert(char * line);
void editor_delete();
void editor_undo();
void editor_redo();
void editor_display();
int editor_num_lines();
Linematches* editor_search_highlight();
void editor_saveToFile();
void editor_loadFromFile(char * filename);
void editor_exit();
#endif
