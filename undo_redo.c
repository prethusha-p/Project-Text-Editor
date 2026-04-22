#include "undo_redo.h"


State undo_stack[50];
State redo_stack[50];


int undo_last=-1, redo_last=-1;

void init_stacks(){
        for(int i=0;i<50;i++){
                strcpy(undo_stack[i].op,"");
                undo_stack[i].lineNum=-1;
                undo_stack[i].line=NULL;

                strcpy(redo_stack[i].op,"");
                redo_stack[i].lineNum=-1;
                redo_stack[i].line=NULL;
        }
        return;
}

void push(State s, char mode){
        if(mode=='u'){
        undo_last++;
        strcpy(undo_stack[undo_last].op,s.op);
        undo_stack[undo_last].line = malloc(sizeof(char)*(strlen(s.line)+1));
        strcpy(undo_stack[undo_last].line,s.line);
        undo_stack[undo_last].lineNum=s.lineNum;
        return;
        }
        if(mode=='r'){
        redo_last++;
        strcpy(redo_stack[redo_last].op,s.op);
        redo_stack[redo_last].line = malloc(sizeof(char)*(strlen(s.line)+1));
        strcpy(redo_stack[redo_last].line,s.line);
        redo_stack[redo_last].lineNum=s.lineNum;
        return;
        }
}

void pushUndo(State s){
        push(s,'u');
}

void pushRedo(State s){
        push(s,'r');
}

State pop(char mode){
	State temp;
        strcpy(temp.op, "");
        temp.line = NULL;
        temp.lineNum = -1;
        if(mode=='u'){
                if(undo_last == -1){
                        // return empty state safely
                        strcpy(temp.op, "");
                        temp.line = NULL;
                        temp.lineNum = -1;
                        return temp;
                }
		strcpy(temp.op,undo_stack[undo_last].op);
		temp.line = malloc(sizeof(char)*(strlen(undo_stack[undo_last].line)+1));
                strcpy(temp.line,undo_stack[undo_last].line);
		temp.lineNum = undo_stack[undo_last].lineNum;
                strcpy(undo_stack[undo_last].op,"");
                free(undo_stack[undo_last].line);
		undo_stack[undo_last].line=NULL;
                undo_last--;
		return temp;

        }
        if(mode=='r'){
                if(redo_last == -1){
                        strcpy(temp.op, "");
                        temp.line = NULL;
                        temp.lineNum = -1;
                        return temp;
                }
		strcpy(temp.op,redo_stack[redo_last].op);
                temp.line = malloc(sizeof(char)*(strlen(redo_stack[redo_last].line)+1));
                strcpy(temp.line,redo_stack[redo_last].line);
                temp.lineNum = redo_stack[redo_last].lineNum;
                strcpy(redo_stack[redo_last].op,"");
                free(redo_stack[redo_last].line);
               	redo_stack[redo_last].line=NULL;
                redo_last--;
		return temp;
        }
	return temp;
}

State popUndo(){
       return pop('u');
}

State popRedo(){
        return pop('r');
}

int isEmpty(char mode){
        if(mode=='u'){
                return (undo_last==-1);
        }
        else if(mode=='r'){
                return (redo_last==-1);
        }
        return 0;
}

void clear(char mode){
        if(mode=='u'){
                for(int i=0;i<=undo_last;i++){
                        strcpy(undo_stack[i].op,"");
			free(undo_stack[i].line);
                        undo_stack[i].line=NULL;
                }
                undo_last=-1;
        }
        else if(mode=='r'){
                for(int i=0;i<=redo_last;i++){
                        strcpy(redo_stack[i].op,"");
                        free(redo_stack[i].line);
			redo_stack[i].line=NULL;
                }
                redo_last=-1;
        }
        return;
}

void freeState(State s){
    if(s.line){
        free(s.line);
    }
}

void shiftLeft(State stack[], int *top){
    freeState(stack[0]);
    for(int i = 1; i <= *top; i++){
        stack[i-1] = stack[i];
    }
    (*top)--;
}


