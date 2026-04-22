#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include <stdio.h>
#include "editor.h"

typedef struct state{
	char op[20];
	char * line;
	int lineNum;
}State;


void init_stacks();
void pushUndo(State s);
void pushRedo(State s);
State popUndo();
State popRedo();
int isEmpty(char mode);
void clear(char mode);

#endif
