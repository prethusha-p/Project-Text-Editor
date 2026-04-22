#include "editor.h"
#include <ncurses.h>
#define maxLength 2048

int display(Node * start, Node * end, Node * cursor){
	Node * curr = start->next;
    	int lineNum = 1;
    	while(curr!=end){
            if(curr==cursor) attron(A_REVERSE);
            mvprintw(lineNum-1,1,"%3d | %s",lineNum,curr->line);
            if(curr==cursor) attroff(A_REVERSE);
            lineNum++;
            curr = curr->next;
    	}
	return lineNum;
}

void search_display(Node * curr, Linematches search, int lineNo, char * word){
		if(search.count==0){
			mvprintw(lineNo,1,"%3d | %s",lineNo+1,curr->line);
			return;
		}
		mvprintw(lineNo,1,"%3d | ",lineNo+1);
		int lineLength = strlen(curr->line);
		char line[lineLength+1];
		strcpy(line,curr->line);
		int word_len=strlen(word);
		int pos = 0;
		for(int i=0;i<lineLength;i++){
			if(pos < search.count && i==search.positions[pos]) attron(A_REVERSE);
			mvprintw(lineNo,7+i,"%c",line[i]);
			if(pos < search.count && i==search.positions[pos]+word_len-1){
				attroff(A_REVERSE);
				pos++;
			}
		}
		return;

}

int get_nav_cursor(Node * start, Node * end, Node * cursor){
	Node * curr = start->next;
	int pos=0;
	while(curr!=end){
		if(curr==cursor) return pos;
		pos++;
		curr = curr->next;
	}
	return pos;
}

int main(){
    initscr(); //initialize the screen
    cbreak(); 
    noecho();
    keypad(stdscr,TRUE);
    set_escdelay(15);
	int rows,cols;
	getmaxyx(stdscr,rows,cols); 
	WINDOW *ui_win = newwin(3, cols, rows - 3, 0);
	scrollok(ui_win,FALSE);
    editor_init();
    Node * head = editor_head();
    Node * tail = editor_tail();
    Node * nav_cursor = editor_cursor();
    char file[100];
	strcpy(file,"Untitled");
    int ch;
	wclear(ui_win);
	clear();
    wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);
	
    wrefresh(ui_win);
    while((ch=getch())!='q'){
		if(ch!='i'){
        clear();
        display(head,tail,nav_cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);
	
    
        refresh();
		wrefresh(ui_win);
		}
        if(ch=='i'){
		int len = 0;
        char buffer[maxLength];
        buffer[0]='\0';
        int c;
            while(1){
				erase();
			nav_cursor = editor_cursor();
			Node * curr = head->next;
			int currentRow = 0;
			int bufferRow = -1;
			int offset=0;
					if (nav_cursor==head) {
						bufferRow = 0;
						offset = 1; // Start drawing existing lines from Row 1
					}
			// Draw everything and find the gap for the buffer
        while(curr!=tail && curr != NULL) {
            if(curr==nav_cursor) {
                // Draw the line the cursor is currently on
                attron(A_REVERSE);
                mvprintw(currentRow, 1, "%3d | %s", currentRow + 1, curr->line);
                attroff(A_REVERSE);
                // The typing buffer belongs on the row immediately after the cursor
                bufferRow = currentRow + 1;
                offset=1; 
            }else{
                mvprintw(currentRow+offset, 1, "%3d | %s", currentRow +offset+ 1, curr->line);
            }
            curr = curr->next;
            currentRow++;
        }
        // If we are at the very beginning (head), bufferRow might still be -1
        if(bufferRow == -1)bufferRow = 0;

        // Draw the "Live" typing buffer in the gap
        mvprintw(bufferRow, 1, "%3d | %s", bufferRow + 1, buffer);
        wmove(stdscr,bufferRow,7+len);
			wclear(ui_win);
			wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--INSERT MODE--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
		wmove(stdscr,bufferRow,7+len);
		wnoutrefresh(stdscr);
        doupdate();
                c = getch();
                if(c==27){
					clear();
        display(head,tail,nav_cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);
	
    
        refresh();
		wrefresh(ui_win);
                    break; //ESC to break out of the input mode
                }
                else if(c=='\n'){
                    buffer[len]='\0';
                    editor_insert(buffer); //ENTER to complete one line of input
                    nav_cursor = editor_cursor();
                    buffer[0]='\0';
                    len=0;
                }
                else if(c==KEY_BACKSPACE || c==127 || c==8 || c==7){
                    if(len>0){ //handling BACKSPACE
                        len--;
                        buffer[len] = '\0';
                    }
                }
                else{
                    buffer[len]=c; //writing char into buffer
                    len++;
                    buffer[len]='\0';
                }
        }
	}

	if(ch == 'd'){
		editor_delete();
		nav_cursor = editor_cursor();
		wclear(ui_win);
		clear();
		display(head,tail,nav_cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--DELETE MODE--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
	}

	if(ch == 'u'){
		editor_undo();
		nav_cursor = editor_cursor();
		wclear(ui_win);
		clear();
		display(head,tail,nav_cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE);
	mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--UNDO MODE--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
	}

	if(ch == 'r'){
		editor_redo();
		nav_cursor = editor_cursor();
		clear();
		wclear(ui_win);
		display(head,tail,nav_cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--REDO MODE--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();	
	}
	if(ch=='h'){
		nav_cursor=editor_cursor();
		refresh();
		int ht=3, wd=40;
		int starty=(LINES-ht)/2;
		int startx = (COLS-wd)/2;

		WINDOW * win = newwin(ht,wd,starty,startx);
		box(win,0,0);

		mvwprintw(win,1,2,"Search: ");
	       	wrefresh(win);	
		char word[2048];
		word[0]='\0'; //a change here
		int w_len = 0;
		int w;
		while((w=wgetch(win))!='\n'){
			wclear(win);
			mvwprintw(win,1,2,"Search: %s",word);
			wrefresh(win);
			if(w==27){
                 	   break; //ESC to break out of the search & highlight mode
                	}
			if(w==KEY_BACKSPACE || w==127 || w==8 || w==7){
				if(w_len>0){
				w_len--;
				word[w_len]='\0';
				}
			}
			else{
				word[w_len]=w;
				w_len++;
				word[w_len]='\0';
			}
			wclear(win);
			mvwprintw(win,1,2,"Search: %s",word);
			wmove(win,1,w_len+10);
			wrefresh(win);
		}
		delwin(win);
		wclear(ui_win);
		clear();
		if(word[0]=='\0'){
			clear();
			Node * head = editor_head();
			Node * tail = editor_tail();
			Node * cursor = editor_cursor();
			display(head,tail,cursor);
			wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--SERACH HIGHLIGHT MODE--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
			continue;
		}
		Linematches * searches = editor_search_highlight(word);
		int lineNo=0;
		Node * curr =  head->next;
		while(curr!=tail){
			search_display(curr,searches[lineNo],lineNo,word);
			wclear(ui_win);
			wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--SEARCH HIGHLIGHT MODE--");
			curr=curr->next;
			lineNo++;
		}
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
	}
	if(ch=='s'){
		char filename[100];
		filename[0]='\0';
		int ht=3, wd=40;
                int starty=(LINES-ht)/2;
                int startx = (COLS-wd)/2;

                WINDOW * win = newwin(ht,wd,starty,startx);
                box(win,0,0);
		mvwprintw(win,1,2,"Enter filename: ");
		wrefresh(win);
		int w;
		int f_len=0;
		while((w=getch())!='\n'){
			wclear(win);
			mvwprintw(win,1,2,"Enter filename: %s",filename);
			wrefresh(win);
			 if(w==KEY_BACKSPACE || w==127 || w==8 || w==7){
                                if(f_len>0){
                                f_len--;
                                filename[f_len]='\0';
                                }
                         }
			 else{
			 	filename[f_len]=w;
				f_len++;
				filename[f_len]='\0';
			 }
			 wclear(win);
			 mvwprintw(win,1,2,"Enter filename: %s",filename);
			 wmove(win,1,f_len+18);
			 wrefresh(win);
		}
		wclear(win);
		wrefresh(win);
		delwin(win);
		if(filename[0]!='\0') strncpy(file,filename,100);
		editor_saveToFile(file);
		Node * nav_cursor = editor_cursor();
		wclear(ui_win);
		display(head,tail,nav_cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--File Saved Successfully!!--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
	}
	if(ch=='l'){
		char filename[100];
                filename[0]='\0';
                int ht=3, wd=40;
                int starty=(LINES-ht)/2;
                int startx = (COLS-wd)/2;

                WINDOW * win = newwin(ht,wd,starty,startx);
                box(win,0,0);
                mvwprintw(win,1,2,"Enter filename: ");
                wrefresh(win);
                int w;
                int f_len=0;
                while((w=getch())!='\n'){
                        wclear(win);
                        mvwprintw(win,1,2,"Enter filename: %s",filename);
                        wrefresh(win);
                         if(w==KEY_BACKSPACE || w==127 || w==8 || w==7){
                                if(f_len>0){
                                f_len--;
                                filename[f_len]='\0';
                                }
                         }
                         else{
                                filename[f_len]=w;
                                f_len++;
                                filename[f_len]='\0';
                         }
                         wclear(win);
                         mvwprintw(win,1,2,"Enter filename: %s",filename);
                         wmove(win,1,f_len+18);
			 wrefresh(win);
		}
		wclear(win);
		delwin(win);
		editor_loadFromFile(filename);
		strcpy(file,filename);
		Node * head = editor_head();
		Node * tail = editor_tail();
		Node * cursor = editor_cursor();
		wclear(ui_win);
		display(head,tail,cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--File loaded successfully!!--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
	}
	if(ch==KEY_UP){
		editor_move_cursor('u');
		Node * head = editor_head();
		Node * tail = editor_tail();
		Node *cursor = editor_cursor();
		int y = get_nav_cursor(head,tail,cursor);
		int line_len = strlen(cursor->line);
		move(y,7+line_len);
		display(head,tail,cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--MOVING UP--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
		
	}
	if(ch==KEY_DOWN){
			editor_move_cursor('d');
			Node * head = editor_head();
		Node * tail = editor_tail();
		Node *cursor = editor_cursor();
			int y = get_nav_cursor(head,tail,cursor);
			int line_len = strlen(cursor->line);
			move(y,7+line_len);
		display(head,tail,cursor);
		wattron(ui_win,A_REVERSE);
    mvwprintw(ui_win,0,0,"Menu:  i:insert | ESC:back to menu || d:delete || h:search & highlight || s:save || l:load || KEY_UP move cursor up ||KEY_DOWN: move cursor down|| q:quit \n");
    wattroff(ui_win,A_REVERSE);

    wattron(ui_win,A_REVERSE); // Invert colors for the status bar
    mvwprintw(ui_win,1, 0, " File: %s", file);
    wattroff(ui_win,A_REVERSE);

		mvwprintw(ui_win,2,0,"--MOVING DOWN--");
		wnoutrefresh(stdscr);
        wnoutrefresh(ui_win);
        doupdate();
		}
	
    } 
    refresh();
	delwin(ui_win);
    endwin();
    editor_exit();
    return 0;
}
