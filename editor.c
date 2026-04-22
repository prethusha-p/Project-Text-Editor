#include "editor.h"
#include "undo_redo.h"



static Node * head = NULL;
static Node * tail = NULL;
static Node * cursor = NULL;



void init(){
    head =  (Node *) malloc(sizeof(Node));
    tail = (Node *) malloc(sizeof(Node));
    head->next=tail;
    tail->prev = head;
    head->prev=NULL;
    tail->next=NULL;
    cursor=head;
}

Node * createNode(char * line){
    Node * newNode = malloc(sizeof(Node));
    newNode->line = malloc(sizeof(char)*strlen(line)+1);
    strcpy(newNode->line,line);    
    return newNode;
}

int numLines(){
    Node * curr = head->next;
    int num = 0;
    while(curr!=tail){
        num++;
        curr = curr->next;
    }
    return num;
}

Node * getprevLine(int lineNum){
    Node * head = editor_head();
    Node * tail = editor_tail();
    Node * curr = head;
    int counter=0;
    while(curr!=tail && counter<lineNum-1){
        curr=curr->next;
        counter++;
    }
    return curr;
}

int getCursor(){
	int val =0;
	Node *curr = head->next;
	while(curr!=cursor->next){
		val++;
		curr=curr->next;
	}
	return val;
}

void freeNodes(){
    Node * curr = head->next;
    Node * temp;
    while(curr!=tail){
        temp=curr;
        curr=curr->next;
        free(temp->line);
        free(temp);
    }
    free(head);
    free(tail);
    head = NULL;
    tail = NULL;
}

void editor_init(){
    init();
    init_stacks();
}

void editor_exit(){
    clear('u');
    clear('r');
    freeNodes();
}

Node * editor_head(){
	return head;
}

Node * editor_tail(){
	return tail;
}

Node * editor_cursor(){
	return cursor;
}

void editor_insert(char * line){
    clear('r');
    Node * newNode = createNode(line);
    Node * nextNode = cursor->next;
    newNode->prev=cursor;
    newNode->next=nextNode;

    cursor->next=newNode;
    if(nextNode!=NULL) nextNode->prev=newNode;
    cursor = newNode;
    State s;
    strcpy(s.op,"insert");
    s.line = malloc((strlen(newNode->line)+1)*sizeof(char));
    strcpy(s.line,newNode->line);
    s.lineNum = getCursor();
    pushUndo(s);
    return;
}

void editor_delete(){
    clear('r');
    Node * temp = cursor;
    State s;
    strcpy(s.op,"delete");
    if(temp==head){
        return;
    }
    if(temp->prev==head){
        if(temp->next!=tail){
            s.lineNum=getCursor();
            Node *next = cursor->next;
            head->next=next;
            next->prev = head;
            cursor = head;
            s.line=malloc(sizeof(char)*(strlen(temp->line)+1));
            strcpy(s.line,temp->line);
            free(temp->line);
            free(temp);
            pushUndo(s);
            return;
        }
        if(temp->next==tail){
            s.lineNum=getCursor();
            cursor=cursor->prev;
            head->next = temp->next;
            tail->prev =head;
            s.line=malloc(sizeof(char)*(strlen(temp->line)+1));
            strcpy(s.line,temp->line);
            free(temp->line);
            free(temp);
            pushUndo(s);
            return;
        }
    }
    int cur=getCursor();
    cursor = cursor->prev;
    cursor->next = temp->next;
    (temp->next)->prev = cursor;
    s.line=malloc(sizeof(char)*(strlen(temp->line)+1));
    strcpy(s.line,temp->line);
    s.lineNum=cur;
    pushUndo(s);
    free(temp->line);
    free(temp);
    return;
}

void editor_undo(){
if(isEmpty('u')) {
    return;
}
  State s = popUndo();
  if(s.lineNum==-1 ||strcmp(s.op,"")==0) return;
  if(strcmp(s.op,"insert")==0){
	 //pushing the undo operation into redo stack
    pushRedo(s);
    Node * prev ;
    prev = getprevLine(s.lineNum);
	Node * curr = prev->next;
    if(curr!=tail){
        prev->next = curr->next;
        (curr->next)->prev = prev;
        if(curr==cursor){
            cursor=cursor->prev;
        }
        free(curr->line);
        free(curr);
    }
	return;
  }
  if(strcmp(s.op,"delete")==0){
  	pushRedo(s);
    Node * prev = getprevLine(s.lineNum);
    Node * curr = prev->next;
    Node * newNode = malloc(sizeof(Node));
    newNode->line = malloc(sizeof(char)*(strlen(s.line)+1));
    strcpy(newNode->line,s.line);
    prev->next=newNode;
    newNode->prev=prev;
	newNode->next=curr;
    curr->prev = newNode;
    if(cursor==prev) cursor=newNode;
	return;	
  }
    return;
}

void editor_redo(){
    if(isEmpty('r')) return;
    State s = popRedo();
    Node * prev;
    if(s.lineNum==-1) return;
    if(s.lineNum!=-1) prev=getprevLine(s.lineNum);
    Node * curr = prev->next;
    if(strcmp(s.op,"insert")==0){
        pushUndo(s);
        Node * newNode = malloc(sizeof(Node));
        newNode->line = malloc(sizeof(char)*(strlen(s.line)+1));
        strcpy(newNode->line,s.line);
        prev->next=newNode;
        newNode->prev=prev;
        newNode->next=curr;
        curr->prev=newNode;
        if(cursor==prev) cursor = cursor->next;
        return;
  }
  if(strcmp(s.op,"delete")==0){
    if(curr!=tail){
        prev->next = curr->next;
        (curr->next)->prev=prev;
        
        if(cursor==curr) cursor=prev;
        free(curr->line);
        free(curr);
        pushUndo(s);
    }
    return;
  }
    return;
}

void editor_move_cursor(char dir){
    if(dir=='u'){
        if(cursor!=NULL && cursor->prev!=head){
			cursor = cursor->prev;
        }
    }
    if(dir=='d'){
        if(cursor!=NULL && cursor->next!=tail){
			cursor = cursor->next;
        }
    }   
}

int editor_num_lines(){
    return numLines();
}

Linematches* editor_search_highlight(char * word){
    Node * curr = head->next;
    if(curr==tail){
        return NULL;
    }
    if(word==NULL || strlen(word)==0) return NULL;
    int lines = numLines();
    Linematches * highlights = malloc(sizeof(Linematches)*lines);
    for(int i=0;i<lines;i++){
        highlights[i].count=0;
    }
    int lineNum = 0;
    char * ptr;
    while(curr!=tail){
        int count  = 0;
        ptr = strstr(curr->line,word);
        while(ptr!=NULL){
            count++;
            ptr = ptr + strlen(word);
            ptr = strstr(ptr,word);
        }
        if(count==0){
            highlights[lineNum].positions=NULL;
            lineNum++;
            curr = curr->next;
            continue;
        }
        highlights[lineNum].count = count;
        highlights[lineNum].positions = malloc(sizeof(int)*count);
        int pos  = 0;
        ptr = strstr(curr->line,word);
        int matches = 0;
        while(ptr!=NULL){
            pos = ptr - curr->line;
            highlights[lineNum].positions[matches++] = pos;
            ptr = ptr + strlen(word);
            ptr = strstr(ptr,word);
        }
        lineNum++;
        curr=curr->next;
    }
    return highlights;
}

void editor_saveToFile(char * filename){
    FILE * fptr;
    fptr = fopen(filename,"w");
    if(fptr==NULL){
        perror("Error opening file");
        return;
    }
    for(Node *curr = head->next; curr!=tail ; curr=curr->next){
        fprintf(fptr,"%s\n",curr->line);
    }
    fclose(fptr);
    return;
}

void editor_loadFromFile(char * filename){
    FILE * fptr;
    fptr = fopen(filename,"r");
    if(fptr==NULL){
        perror("Error opening file");
        return;
    }
    if(head->next!=tail){
        Node * curr = head->next;
        Node * temp;
        while(curr!=tail){
            temp=curr;
            curr=curr->next;
            free(temp->line);
            free(temp);
        }
    }
    head->next=tail;
    tail->prev=head;
    cursor = head;
    char buffer[2048]; 
    while(fgets(buffer,2048,fptr)!=NULL){
        buffer[strcspn(buffer,"\n")]='\0'; //gives first occurance of \n and thereby replace with \0
        editor_insert(buffer);
    }
    fclose(fptr);
}




