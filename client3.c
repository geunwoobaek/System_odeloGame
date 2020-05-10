#include<ncurses.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#define BUFSIZE 1024
static int sock;
static char message_sock[100];
static int str_len;
static struct sockaddr_in serv_addr;
static char* Buf1;
static char* Buf2;
static char* Buf3;
static int current=0;
static int player_number=2;
typedef struct information
{
char tempid[20];
char tempps[20];
char message[BUFSIZE];
char firstID[100];
char secondID[100];
char loginID[2][20];
int win[2];
int lose[2];
int sequence;
int row;
int colum;
}information;

information info;
void error_handling(char *message);
int moving(int current,int num,int limit);
int Keymoving(int current,int limit);
int* GameKeymoving(int *cordinate,int x_limit,int y_limit); 
int MAIN_Screen();
int SIGNIN_Screen();
int SIGNUP_Screen();
void delete(char *id,char *ps);
int LOGIN_Screen(char *id);
void WITHDRAWL_Screen(char *id);
void INFORMATION_Screen(char *id);
void GAME_Screen(char *firstid,char *secondid);
int CHECK(char *Buf_,char *personal);
int SIGNUP(char *ID,char *PS);
int SIGNIN(char *ID,char *PS);
int WITHDRAWAL(char *ID);
int main(int argc , char const **argv){
	if(argc!=3){
		printf("Usage :%s <IP> <PORT>\n",argv[0]);
		exit(1);
	}
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
	error_handling("socket() error");
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("connect() error!");	
	int fd1,fd2,fd3;
	read(sock,(void *)&info,sizeof(info));
	fd1=open("Personal_ID.txt",O_RDWR|O_CREAT,0644);
	fd2=open("Personal_PASSWORD.txt",O_RDWR|O_CREAT,0644);
	fd3=open("Personal_INFORMATION.txt",O_RDWR|O_CREAT,0644);
	Buf1=(char*)malloc((200)*sizeof(char));
	Buf2=(char*)malloc((200)*sizeof(char));
	Buf3=(char*)malloc((200)*sizeof(char));
	read(fd1,Buf1,200);
	read(fd2,Buf2,200);
	read(fd3,Buf3,200);
	close(fd1);
	close(fd2);
	close(fd3);
	MAIN_Screen();
	return 0;
}

int moving(int current,int num,int limit)
{
	int now=current;
	if(now+num<=limit||now+num>=0)
		now+=num;
	return now;
};
int Keymoving(int current,int limit)
{
	int now=current;
	keypad(stdscr,TRUE);
	switch (getch()){
		case KEY_LEFT:
			now=moving(now,-1,limit);
			break;
		case KEY_RIGHT:
			now=moving(now,1,limit);
			break;
		case 10:
			now+=10;
			break;
	}
	return now;
};
int* GamekeyMoving(int *cordinate,int x_limit,int y_limit)
{
	int row=cordinate[0];
	int colum=cordinate[1];
	int enter=cordinate[2];
	keypad(stdscr,TRUE);
	switch(getch()){
		case KEY_LEFT:
			row=moving(row,-1,x_limit);
			break;
		case KEY_RIGHT:
			row=moving(row,1,x_limit);
			break;
		case KEY_UP:
			colum=moving(colum,1,y_limit);
			break;		
		case KEY_DOWN:
			colum=moving(colum,-1,y_limit);
			break;
		case 10:
			if(enter==-1)
				enter=1;
		case 120:
			if(enter==0)
				enter=-1;
			else if(enter==-1)
				enter=0;	
			break;
	}
	cordinate[0]=row;
	cordinate[1]=colum;
	cordinate[2]=enter;
	return cordinate;

};
int MAIN_Screen()
{ 
	WINDOW *SIGN_IN;
	WINDOW *SIGN_UP;
	WINDOW *EXIT;
	WINDOW *window1;
	WINDOW *window2;
	initscr();
	noecho();
	curs_set(FALSE);
	if(has_colors()==FALSE){
		puts("Terminal does not support colors!");
		endwin();
		return 1;
	}else{
		start_color();
		init_pair(1,COLOR_BLUE,COLOR_WHITE);
		init_pair(2,COLOR_WHITE,COLOR_BLUE);
		init_pair(3,14,COLOR_WHITE);
	}
	refresh();
	window1=newwin(18,80,0,0);
	window2=newwin(6,80,18,0);
	SIGN_IN=newwin(1,8,20,10);
	SIGN_UP=newwin(1,8,20,35);
	EXIT=newwin(1,5,20,60);
	wbkgd(window1,COLOR_PAIR(1));
	wbkgd(window2,COLOR_PAIR(2));
	wbkgd(SIGN_IN,COLOR_PAIR(1));
	wbkgd(SIGN_UP,COLOR_PAIR(2));
	wbkgd(SIGN_UP,COLOR_PAIR(2));
	wbkgd(EXIT,COLOR_PAIR(2));
	wprintw(window1,"");
	wprintw(window2,"");
	wprintw(SIGN_IN,"SIGN IN");
	wprintw(SIGN_UP,"SIGN UP");
	wprintw(EXIT,"EXIT");
	attron(COLOR_PAIR(1));
	mvprintw(6,25,"System Software pratice");
	mvprintw(15,65,"2015726079");
	mvprintw(16,65,"Guenwoo Baek");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(3));
	mvprintw(8,33,"OTHELLO");
	attroff(COLOR_PAIR(3));
	wrefresh(window1);
	wrefresh(window2);
	wrefresh(SIGN_IN);
	wrefresh(SIGN_UP);
	wrefresh(EXIT);
	refresh();
	int now=0;	
	while(1)
	{	
		now=Keymoving(now,2);
		switch (now){
			case 0:
				wbkgd(SIGN_IN,COLOR_PAIR(1));
				wbkgd(SIGN_UP,COLOR_PAIR(2));
				wbkgd(EXIT,COLOR_PAIR(2));
				break;
			case 1:
				wbkgd(SIGN_IN,COLOR_PAIR(2));
				wbkgd(SIGN_UP,COLOR_PAIR(1));
				wbkgd(EXIT,COLOR_PAIR(2));
				break;
			case 2:
				wbkgd(SIGN_IN,COLOR_PAIR(2));
				wbkgd(SIGN_UP,COLOR_PAIR(2));
				wbkgd(EXIT,COLOR_PAIR(1));
				break;
			case 10:
				endwin();
				SIGNIN_Screen();
				break;
			case 11:
				endwin();
				SIGNUP_Screen();
				break;
			case 12:
				endwin();
				exit(1);
				break;	
		}
		wrefresh(EXIT);
		wrefresh(SIGN_IN);
		wrefresh(SIGN_UP);
		refresh();
	}  
	endwin();
	return 0;
};
int SIGNUP_Screen()
{
	int now=0;
	WINDOW *first;
	WINDOW *second;
	WINDOW *window1;
	WINDOW *window2;
	WINDOW *MESSAGE;
	WINDOW *ID;
	WINDOW *PASSWORD;
	WINDOW *SIGN_UP;
	WINDOW *BACK;
	// initscr();
	// noecho();
	curs_set(FALSE);
	if(has_colors()==FALSE){
		puts("Terminal does not support colors!");
		endwin();
		return 1;
	}else{
		start_color();
		init_pair(1,COLOR_BLUE,COLOR_WHITE);
		init_pair(2,COLOR_WHITE,COLOR_BLUE);
		init_pair(3,18,COLOR_WHITE);
	}
	refresh();
	MESSAGE=newwin(1,55,23,0);
	first=newwin(1,15,6,32);
	second=newwin(1,15,7,32);
	window1=newwin(18,80,0,0);
	window2=newwin(6,80,18,0);
	SIGN_UP=newwin(1,8,20,15);
	BACK=newwin(1,5,20,50);
	ID=newwin(1,25,6,23);
	PASSWORD=newwin(1,25,7,20);
	wbkgd(MESSAGE,COLOR_PAIR(2));
	wbkgd(first,COLOR_PAIR(1));
	wbkgd(second,COLOR_PAIR(1));
	wbkgd(window1,COLOR_PAIR(1));
	wbkgd(window2,COLOR_PAIR(2));
	wbkgd(SIGN_UP,COLOR_PAIR(2));
	wbkgd(BACK,COLOR_PAIR(2));
	wbkgd(ID,COLOR_PAIR(3));
	wbkgd(PASSWORD,COLOR_PAIR(3));
	wprintw(window1,"");
	wprintw(window2,"");
	wprintw(ID,"ID    :");
	wprintw(PASSWORD,"PASSWORD : ");
	wprintw(SIGN_UP,"SIGN UP");
	wprintw(BACK,"BACK");
	attron(COLOR_PAIR(1));
	mvprintw(5,35,"SIGN UP");
	wrefresh(window1);
	wrefresh(window2);
	wrefresh(BACK);
	wrefresh(PASSWORD);
	wrefresh(ID);
	wrefresh(SIGN_UP);
	char *id=(char*)malloc(20*sizeof(char));
	char *password=(char*)malloc(20*sizeof(char));
	char c;
	char ch;
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(3));
	int on=1;
	int k=0, p=32;
	while(1)
	{      
		werase(MESSAGE);
		wrefresh(MESSAGE);
		curs_set(1);
		echo();
		mvgetstr(6,32,id);
		curs_set(0);
		wprintw(first,id);
		wrefresh(first);
		curs_set(1);
		echo();
		while(1)
		{noecho();
			ch=mvgetch(7,p);
			if(ch==10)
			{
				p=32;
				k=0;
				break;
			}
			if(k!=0&&(ch==127||ch==8))
			{
				k-=1;
				p-=1;
				mvaddch(7,p,' ');
				password[k]=0x00;
				echo();
				continue;
			}
			password[k]=ch;
			echo();
			mvaddch(7,p,'*');
			p++;
			k++;
		}
		curs_set(0);
		on=1;
		wbkgd(SIGN_UP,COLOR_PAIR(1));
		wrefresh(SIGN_UP);
		now=0;
		while(on)
		{	
			now=Keymoving(now,1);
			switch (now){
				case 0:
					wbkgd(SIGN_UP,COLOR_PAIR(1));
					wbkgd(BACK,COLOR_PAIR(2));
					break;
				case 1:
					wbkgd(SIGN_UP,COLOR_PAIR(2));
					wbkgd(BACK,COLOR_PAIR(1));
					break;
				case 10:
					strcpy(info.message,"SIGNUP");
					strcpy(info.tempid,id);
					strcpy(info.tempps,password);
					write(sock,(void*)&info,sizeof(info));
					read(sock,(void*)&info,sizeof(info));
					if(strncmp(info.message,"success",7)==0)
					{
						werase(MESSAGE);
						wprintw(MESSAGE,">>>welcome to OTHELLO world (Press any key...)");
						wrefresh(MESSAGE);
						getchar();
						rewind(stdin);
						MAIN_Screen();
					}  
					else
					{
						id=(char*)calloc(20,sizeof(char));
						password=(char*)calloc(20,sizeof(char));
						werase(MESSAGE);
						wprintw(MESSAGE,id);
						wprintw(MESSAGE," has already exits in DB! (press and key)");
						wrefresh(MESSAGE);
						getchar();
						rewind(stdin);
						werase(first);
						werase(second);
						wrefresh(first);
						wrefresh(second);
						on=0;

					}
					break;
				case 11:
					MAIN_Screen();
			}
			refresh();
			wrefresh(SIGN_UP);
			wrefresh(BACK);
		}
	}
	return 0;
}
int SIGNIN_Screen()
{
	char c;
	int now=0;
	WINDOW *MESSAGE;
	WINDOW *window1;
	WINDOW *window2;
	WINDOW *ID;
	WINDOW *PASSWORD;
	WINDOW *SIGN_IN;
	WINDOW *BACK;
	WINDOW *first;
	WINDOW *second;
	initscr();
	// noecho();
	// curs_set(FALSE);
	if(has_colors()==FALSE){
		puts("Terminal does not support colors!");
		endwin();
		return 1;
	}else{
		start_color();
		init_pair(1,COLOR_BLUE,COLOR_WHITE);
		init_pair(2,COLOR_WHITE,COLOR_BLUE);
		init_pair(3,18,COLOR_WHITE);
	}
	refresh();
	MESSAGE=newwin(1,60,23,0);
	first=newwin(1,12,6,32);
	second=newwin(1,12,7,32);
	window1=newwin(18,80,0,0);
	window2=newwin(6,80,18,0);
	SIGN_IN=newwin(1,8,20,15);
	BACK=newwin(1,5,20,50);
	ID=newwin(1,25,6,23);
	PASSWORD=newwin(1,25,7,20);
	wbkgd(MESSAGE,COLOR_PAIR(2));
	wbkgd(window1,COLOR_PAIR(1));
	wbkgd(window2,COLOR_PAIR(2));
	wbkgd(SIGN_IN,COLOR_PAIR(2));
	wbkgd(BACK,COLOR_PAIR(2));
	wbkgd(ID,COLOR_PAIR(3));
	wbkgd(first,COLOR_PAIR(1));
	wbkgd(second,COLOR_PAIR(1));
	wbkgd(PASSWORD,COLOR_PAIR(3));	
	wprintw(window1,"");
	wprintw(window2,"");
	wprintw(ID,"ID    :");
	wprintw(PASSWORD,"PASSWORD : ");
	wprintw(SIGN_IN,"SIGN IN");
	wprintw(BACK,"BACK");
	attron(COLOR_PAIR(1));
	mvprintw(5,35,"SIGN IN");
	wrefresh(window1);
	wrefresh(window2);
	wrefresh(BACK);
	wrefresh(PASSWORD);
	wrefresh(ID);
	wrefresh(SIGN_IN);
	char *id=(char*)malloc(20*sizeof(char));
	char *password=(char*)malloc(20*sizeof(char));
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(3));
	int on=1;
	int k=0, p=32;
	char ch;
	while(1)
	{	
		werase(MESSAGE);
		wrefresh(MESSAGE);
		curs_set(1);
		echo();	
		mvgetstr(6,32,id);
		curs_set(0);
		wprintw(first,id);
		wrefresh(first);
		curs_set(1);
		echo();
		while(1){
			noecho();
			ch=mvgetch(7,p);
			if(ch==10)
			{
				p=32;
				k=0;
				break;
			}
			if(k!=0&&(ch==127||ch==8))
			{
				k-=1;
				p-=1;
				mvaddch(7,p,' ');
				password[k]=0x00;
				echo();
				continue;
			}	
			password[k]=ch;
			echo();
			mvaddch(7,p,'*');
			p++;
			k++;
		}
		curs_set(0);
		on=1;
		wbkgd(SIGN_IN,COLOR_PAIR(1));
		wrefresh(SIGN_IN);
		now=0;
		while(on)
		{
			now=Keymoving(now,1);
			switch (now){
				case 0:
					wbkgd(SIGN_IN,COLOR_PAIR(1));
					wbkgd(BACK,COLOR_PAIR(2));
					break;
				case 1:
					wbkgd(SIGN_IN,COLOR_PAIR(2));
					wbkgd(BACK,COLOR_PAIR(1));
					wrefresh(BACK);
					wrefresh(SIGN_IN);
					break;
				case 10:
					strcpy(info.message,"SIGNIN");
                                        strcpy(info.tempid,id);
                                        strcpy(info.tempps,password);
                                        write(sock,(void*)&info,sizeof(info));
                                        read(sock,(void*)&info,sizeof(info));
					if(strncmp(info.message,"success",7)==0)
						{
						LOGIN_Screen(id);
						}	
					else 
					{
						id=(char*)calloc(20,sizeof(char));
						password=(char*)calloc(20,sizeof(char));
						wprintw(MESSAGE,id);
						wprintw(MESSAGE,info.message);
						wrefresh(MESSAGE);
						getchar();
						rewind(stdin);
						werase(first);
						werase(second);
						wrefresh(first);
						wrefresh(second);
						on=0;

					}
					break;
				case 11:
					endwin();
					MAIN_Screen();
					break;
			}
			refresh();
			wrefresh(BACK);
			wrefresh(SIGN_IN);
		}
	}
	return 0;

};
int LOGIN_Screen(char *id)
{
	int now=0;
	WINDOW *window1;
	WINDOW *window2;
	WINDOW *PLAY;
	WINDOW *SIGN_OUT;
	WINDOW *ID;
	WINDOW *WITH_DRAWL;
	initscr();
	noecho();
	curs_set(FALSE);
	if(has_colors()==FALSE){
		puts("Terminal does not support colors!");
		endwin();
		return 1;
	}else{
		start_color();
		init_pair(1,COLOR_BLUE,COLOR_WHITE);
		init_pair(2,COLOR_WHITE,COLOR_BLUE);
		init_pair(3,18,COLOR_WHITE);
	}
	refresh();

	window1=newwin(18,80,0,0);
	window2=newwin(6,80,18,0);
	PLAY=newwin(1,5,20,10);
	SIGN_OUT=newwin(1,8,20,35);
	WITH_DRAWL=newwin(1,10,20,60);
	ID=newwin(1,10,6,23);
	wbkgd(window1,COLOR_PAIR(1));
	wbkgd(window2,COLOR_PAIR(2));
	wbkgd(PLAY,COLOR_PAIR(1));
	wbkgd(SIGN_OUT,COLOR_PAIR(2));
	wbkgd(WITH_DRAWL,COLOR_PAIR(2));
	wbkgd(ID,COLOR_PAIR(1));
	wprintw(window1,"");
	wprintw(window2,"");
	wprintw(PLAY,"PLAY");
	wprintw(SIGN_OUT,"SIGN OUT");
	wprintw(WITH_DRAWL,"WITHDRAWL");
	wprintw(ID,"player ID:");
	attron(COLOR_PAIR(1));
	mvaddstr(6,33,id);
	attroff(COLOR_PAIR(1));
	wrefresh(window1);
	wrefresh(window2);
	wrefresh(ID);
	wrefresh(PLAY);
	wrefresh(SIGN_OUT);
	wrefresh(WITH_DRAWL);
	refresh();
	while(1)
	{
		now=Keymoving(now,2);
		switch (now){
			case 0:
				wbkgd(PLAY,COLOR_PAIR(1));
				wbkgd(SIGN_OUT,COLOR_PAIR(2));
				wbkgd(WITH_DRAWL,COLOR_PAIR(2));
				break;
			case 1:
				wbkgd(PLAY,COLOR_PAIR(2));
				wbkgd(SIGN_OUT,COLOR_PAIR(1));
				wbkgd(WITH_DRAWL,COLOR_PAIR(2));
				break;
			case 2:
				wbkgd(PLAY,COLOR_PAIR(2));
				wbkgd(SIGN_OUT,COLOR_PAIR(2));
				wbkgd(WITH_DRAWL,COLOR_PAIR(1));
				break;
			case 10:
				strcpy(info.message,"LOGIN");
				strcpy(info.tempid,id);
				write(sock,(void*)&info,sizeof(info));
				INFORMATION_Screen(id);
				break;
			case 11:
				  strcpy(info.message,"MAIN_S");
				  strcpy(info.tempid,id);
                                  write(sock,(void*)&info,sizeof(info));
                                  read(sock,(void*)&info,sizeof(info));
				MAIN_Screen();
				break;
			case 12:	
				WITHDRAWL_Screen(id);
		}
		wrefresh(PLAY);
		wrefresh(SIGN_OUT);
		wrefresh(WITH_DRAWL);

	}
	return 0;

};
void INFORMATION_Screen(char* id)
{	WINDOW *window1;
	WINDOW *window2;
	WINDOW *player1;
	WINDOW *player2;
	WINDOW *O_K;
	WINDOW *WIN;
	WINDOW *WIN2;
	WINDOW *STATICS;
	WINDOW *STATICS2;
	//double percent=((double)a/((double)a+(double)b))*100;
	initscr();
	// noecho();
	curs_set(FALSE);
	if(has_colors()==FALSE){
		puts("Terminal does not support colors!");
		endwin();
		exit(1);
	}else{
		start_color();
		init_pair(1,COLOR_BLUE,COLOR_WHITE);
		init_pair(2,COLOR_WHITE,COLOR_BLUE);
		init_pair(3,18,COLOR_WHITE);
	}
	refresh();
	read(sock,(void *)&info,sizeof(info));
	write(sock,(void *)&info,sizeof(info));
	double percent1=((double)info.win[0]/((double)info.win[0]+(double)info.lose[0]))*100;
	 double percent2=((double)info.win[1]/((double)info.win[1]+(double)info.lose[1]))*100;

	window1=newwin(18,80,0,0);
	window2=newwin(6,80,18,0);
	player1=newwin(1,20,7,14);
	player2=newwin(1,20,7,51);
	STATICS=newwin(1,7,9,15);
	STATICS2=newwin(1,7,9,52);
	WIN=newwin(1,35,11,6);
	WIN2=newwin(1,35,11,45);
	O_K=newwin(1,7,22,39);
	wbkgd(window1,COLOR_PAIR(1));
	wbkgd(window2,COLOR_PAIR(2));
	wbkgd(O_K,COLOR_PAIR(1));
	wbkgd(player1,COLOR_PAIR(1));
	wbkgd(player2,COLOR_PAIR(1));
	wbkgd(WIN,COLOR_PAIR(1));
	wbkgd(STATICS,COLOR_PAIR(1));
	wbkgd(WIN2,COLOR_PAIR(1));
        wbkgd(STATICS2,COLOR_PAIR(1));
	wprintw(window1,"");
	wprintw(window2,"");
	wprintw(O_K,"waiting");
	wprintw(STATICS,"STATICS");
	wprintw(STATICS2,"STATICS");
	wprintw(player1,"player1: ");
	wprintw(player1,info.firstID);
	wprintw(player2,"player2: ");
	wprintw(WIN,"WIN: %d LOSE : %d percent:(%.3f)",info.win[0],info.lose[0],percent1);
	wprintw(WIN2,"WIN: %d LOSE : %d percent:(%.3f)",info.win[1],info.lose[1],percent2);
	wrefresh(window1);
	wrefresh(window2);
	wrefresh(O_K);
	wrefresh(player1);
	wrefresh(STATICS);
	wrefresh(WIN);
	wrefresh(player2);
        wrefresh(STATICS2);
        wrefresh(WIN2);
	int now=0;
	int count_num=0;
	while(strlen(info.secondID)==0)
	{	player_number=1;
		count_num++;
		write(sock,(void *)&info,sizeof(info));
		read(sock,(void *)&info,sizeof(info));
		strcpy(info.message,"LOGIN");
		if(count_num%2==0)
		sleep(1);	
	}
	percent2=((double)info.win[1]/((double)info.win[1]+(double)info.lose[1]))*100;
	werase(WIN2);
	wrefresh(WIN2);
	wprintw(WIN2,"WIN: %d LOSE : %d percent:(%.3f)",info.win[1],info.lose[1],percent2);
	wprintw(player2,info.secondID);
	werase(O_K);
	wrefresh(O_K);
	wprintw(O_K,"OK      ");
	wrefresh(O_K);	
	wrefresh(WIN2);
	wrefresh(player2);
	while(1)
	{
	now=Keymoving(now,0);
	switch (now){

		case 10:
			strcpy(info.message,"Start!!");
			write(sock,(void *)&info,sizeof(info));
			GAME_Screen(info.firstID,info.secondID);
	}
	}

};
void WITHDRAWL_Screen(char* id)
{ int now=0;
	int on=0;
	WINDOW *MESSAGE;
	WINDOW *window1;
	WINDOW *window2;
	WINDOW *ID;
	WINDOW *PASSWORD;
	WINDOW *WITH_DRAWL;
	WINDOW *BACK;
	WINDOW *WD;
	initscr();
	// noecho();
	curs_set(FALSE);
	if(has_colors()==FALSE){
		puts("Terminal does not support colors!");
		endwin();
		exit(1);
	}else{
		start_color();
		init_pair(1,COLOR_BLUE,COLOR_WHITE);
		init_pair(2,COLOR_WHITE,COLOR_BLUE);
		init_pair(3,18,COLOR_WHITE);
	}
	WD=newwin(1,9,3,37);
	MESSAGE=newwin(1,15,23,0);
	window1=newwin(18,80,0,0);
	window2=newwin(6,80,18,0);
	WITH_DRAWL=newwin(1,9,20,15);
	BACK=newwin(1,5,20,50);
	ID=newwin(1,25,6,23);
	PASSWORD=newwin(1,25,7,20);
	wbkgd(window1,COLOR_PAIR(1));
	wbkgd(window2,COLOR_PAIR(2));
	wbkgd(MESSAGE,COLOR_PAIR(2));
	wbkgd(WD,COLOR_PAIR(3));
	wbkgd(WITH_DRAWL,COLOR_PAIR(2));
	wbkgd(BACK,COLOR_PAIR(2));
	wbkgd(ID,COLOR_PAIR(1));
	wbkgd(PASSWORD,COLOR_PAIR(1));
	wprintw(window1,"");
	wprintw(window2,"");
	wprintw(MESSAGE,"");
	wprintw(ID,"ID    : ");
	wprintw(ID,id);
	wprintw(PASSWORD,"PASSWORD : ");
	wprintw(WITH_DRAWL,"WITHDRAWL");
	wprintw(BACK,"BACK");
	wprintw(WD,"WITHDRAWL");
	wrefresh(window1);
	wrefresh(window2);
	wrefresh(BACK);
	wrefresh(PASSWORD);
	wrefresh(ID);
	wrefresh(WD);
	wrefresh(WITH_DRAWL);
	attron(COLOR_PAIR(1));
	char *password=(char*)malloc(20*sizeof(char));
	while(1)
	{	noecho();
		curs_set(1);
		echo();
		mvgetstr(7,31,password);
		curs_set(0);
		on=1;
		wbkgd(WITH_DRAWL,COLOR_PAIR(1));
		wrefresh(WITH_DRAWL);
		now=0;
		while(on)
		{
			now=Keymoving(now,1);
			switch (now){
				case 0:
					wbkgd(WITH_DRAWL,COLOR_PAIR(1));
					wbkgd(BACK,COLOR_PAIR(2));
					break;
				case 1:
					wbkgd(WITH_DRAWL,COLOR_PAIR(2));
					wbkgd(BACK,COLOR_PAIR(1));
					break;
				case 10:
					strcpy(info.message,"DELETE");
                                        strcpy(info.tempid,id);
                                        strcpy(info.tempps,password);
                                        write(sock,(void*)&info,sizeof(info));
                                        read(sock,(void*)&info,sizeof(info));
                                        if(strncmp(info.message,"success",7)==0)
					{
						wprintw(MESSAGE," SUCCESS");
						wrefresh(MESSAGE);
						delete(id,password);
						getchar();
						getchar();
						rewind(stdin);
						MAIN_Screen();
					}
					else
					{
						werase(PASSWORD);
						wprintw(PASSWORD,"PASSWORD : ");
						wprintw(MESSAGE,"FAIL");
						wrefresh(MESSAGE);
						getchar();
						rewind(stdin);
						werase(MESSAGE);
						wrefresh(MESSAGE);
						wbkgd(WITH_DRAWL,COLOR_PAIR(2));
						on=0;
						password=0x00;
					}
					break;
				case 11:
					LOGIN_Screen(id);
					break;
			}
			wrefresh(WITH_DRAWL);
			wrefresh(BACK);
			wrefresh(PASSWORD);
		}
	}
};

void GAME_Screen(char *firstid,char *secondid)
{
	int now=0;
	int on=0;
	int *cursor= (int*)malloc(2*sizeof(int));
	cursor[0]=0;
	cursor[1]=0;
	cursor[2]=0;
	WINDOW *MESSAGE;
	WINDOW *window1;
	WINDOW *window2;
	WINDOW *EXIT;
	WINDOW *____;
	WINDOW *N_;
	WINDOW *R_;
	WINDOW *FLATFORM;
	WINDOW *FIRST;
	WINDOW *SECOND;
	WINDOW *NEXT_TURN;
	WINDOW *REGAME;
	WINDOW **BALL=(WINDOW**)malloc(20*sizeof(WINDOW));
	initscr();
	// noecho();
	curs_set(FALSE);
	if(has_colors()==FALSE){
		puts("Terminal does not support colors!");
		endwin();
		exit(1);
	}else{
		start_color();
		init_pair(1,COLOR_BLUE,COLOR_WHITE);
		init_pair(2,COLOR_WHITE,COLOR_BLUE);
		init_pair(3,18,COLOR_WHITE);
	}
	window1=newwin(24,60,0,0);
	window2=newwin(24,20,0,60);
	BALL[0]=newwin(1,3,10,24);
	BALL[1]=newwin(1,3,10,28);
	BALL[2]=newwin(1,3,12,24);
	BALL[3]=newwin(1,3,12,28);
	EXIT=newwin(1,5,17,68);
	____=newwin(1,1,17,69);
	FLATFORM=newwin(18,30,5,15);
	FIRST=newwin(1,17,9,62);
	SECOND=newwin(1,17,10,62);
	NEXT_TURN=newwin(1,9,13,66);
	N_=newwin(1,1,13,66);
	REGAME=newwin(1,6,15,68);
	R_=newwin(1,1,15,68);
	wbkgd(window1,COLOR_PAIR(1));
	wbkgd(window2,COLOR_PAIR(2));
	wbkgd(BALL[0],COLOR_PAIR(3));
	wbkgd(BALL[1],COLOR_PAIR(3));
	wbkgd(BALL[2],COLOR_PAIR(3));
	wbkgd(BALL[3],COLOR_PAIR(3));
	wbkgd(FLATFORM,COLOR_PAIR(1));
	wbkgd(FIRST,COLOR_PAIR(1));
        wbkgd(SECOND,COLOR_PAIR(2));
	wbkgd(EXIT,COLOR_PAIR(2));
	wbkgd(____,COLOR_PAIR(2)|A_UNDERLINE);
	wbkgd(NEXT_TURN,COLOR_PAIR(2));
        wbkgd(N_,COLOR_PAIR(2)|A_UNDERLINE);
	wbkgd(REGAME,COLOR_PAIR(2));
        wbkgd(R_,COLOR_PAIR(2)|A_UNDERLINE);
	wprintw(FIRST,firstid);
	wprintw(SECOND,secondid);
	wprintw(FIRST,"(O) : %d",2);
	wprintw(SECOND,"(X) : %d",2);
	wprintw(NEXT_TURN,"NEXT TURN");
	wprintw(N_,"N");
	wprintw(REGAME,"REGAME");
	wprintw(R_,"R");
	wprintw(BALL[0]," O ");
	wprintw(BALL[1]," X ");
	wprintw(BALL[2]," O ");
	wprintw(BALL[3]," X ");
	wprintw(EXIT,"EXIT");
	wprintw(____,"X");
	wprintw(window1,"");
	wprintw(window2,"");
	for(int i=0;i<=12;i++)
	{
		if(i%2==0)
		{
			wprintw(FLATFORM,"+---+");
			for(int j=0;j<5;j++)
			{
				wprintw(FLATFORM,"---+");
			}
			wprintw(FLATFORM,"\n");
		}
		else
		{
			for(int j=0;j<7;j++)
			{
				wprintw(FLATFORM,"|   ");
			}
			wprintw(FLATFORM,"\n");
		}
	}
	wrefresh(window1);
	wrefresh(window2);
	wrefresh(FLATFORM);
	wrefresh(EXIT);
	wrefresh(____);	
	wrefresh(BALL[0]);
	wrefresh(BALL[1]);
	wrefresh(BALL[2]);
	wrefresh(BALL[3]);
	wrefresh(FIRST);
	wrefresh(SECOND);
	wrefresh(NEXT_TURN);
	wrefresh(N_);
	wrefresh(REGAME);
	wrefresh(R_);
	on=1;
	WINDOW* current;
	current=newwin(1,3,10,20);
	wbkgd(current,COLOR_PAIR(2));
	wrefresh(current);
	wprintw(current," O ");
	now=1;
	while(on)
	{	
		cursor=GamekeyMoving(cursor,5,5);
		if(cursor[2]==1)
		{   MAIN_Screen();}
		if((cursor[2]==-1))
		{    wbkgd(EXIT,COLOR_PAIR(1));
			wbkgd(____,COLOR_PAIR(1)|A_UNDERLINE);	
		}
		if((cursor[2]==0))
		{    wbkgd(EXIT,COLOR_PAIR(2));
			wbkgd(____,COLOR_PAIR(2)|A_UNDERLINE);
		}
		wrefresh(EXIT);
		wrefresh(____);
		if(cursor[2]==-1)
		{continue;}
		if(cursor[0]==5)
		{cursor[0]-=2;}
		else if(cursor[0]==-2)
		{cursor[0]+=2;}
		if(cursor[1]==3)
		{cursor[1]-=2;}
		else if(cursor[1]==-4)
		{cursor[1]+=2;}
		mvwin(current,10-2*cursor[1],20+4*cursor[0]);
		wrefresh(FLATFORM);
		wbkgd(FLATFORM,COLOR_PAIR(1));
		wrefresh(FLATFORM);
		for(int i=0;i<4;i++)
		{wbkgd(BALL[i],COLOR_PAIR(3));
			wrefresh(BALL[i]);
		}
		wrefresh(current);

	}
}	

int CHECK(char *Buf_,char *personal)
{
	for(int i=0;i<strlen(Buf_);)
	{
		if(Buf_[i]=='@')
		{
			if(strlen(personal)==(int)Buf_[i+2])
			{
				for(int j=0;j<strlen(personal);j++)
				{
					if(Buf_[i+3+j]!=personal[j])
					{
						i+=Buf_[i+2]+4;	
						break;
					}
					if(j==strlen(personal)-1)
						return i+1;
				}
			}
			else
				i+=Buf_[i+2]+4;
		}
		else
			i+=Buf_[i+2]+4;
	}
	return 0;
};
int SIGNIN(char *ID,char *PS)
{	
	if(CHECK(Buf1,ID)!=0&&CHECK(Buf2,PS)!=0)
		return CHECK(Buf1,ID);
	else
		return 0;

};
int SIGNUP(char *ID,char *PS)
{
	int fd1,fd2,fd3;
	int num;
	int x,y;
	x=strlen(Buf1);
	y=strlen(Buf2);
	Buf1[x]='@';
	Buf2[y]='@';
	if(x==0)
	{
		num=1;
	}
	else
	{
		num=(int)Buf1[x-(int)Buf1[x-1]-3]+1;
	}
	Buf3[(num-1)*3]=(char)num;
	Buf3[(num-1)*3+1]=(char)11;
	Buf3[(num-1)*3+2]=(char)12;
	Buf1[x+1]=(char)num;
	Buf2[y+1]=(char)num;
	Buf1[x+2]=(char)strlen(ID);
	Buf2[y+2]=(char)strlen(PS);
	for(int i=0;i<strlen(ID);i++)
	{
		Buf1[x+3+i]=ID[i];
	}
	for(int i=0;i<strlen(PS);i++)
	{
		Buf2[y+3+i]=PS[i];
	}
	Buf1[x+3+strlen(ID)]=(char)strlen(ID);
	Buf2[y+3+strlen(PS)]=(char)strlen(PS);
	fd1=open("Personal_ID.txt",O_RDWR|O_CREAT,0644);
	fd2=open("Personal_PASSWORD.txt",O_RDWR|O_CREAT,0644);
	fd3=open("Personal_INFORMATION.txt",O_RDWR|O_CREAT,0644);
	lseek(fd1,0,SEEK_SET);
	lseek(fd2,0,SEEK_SET);
	lseek(fd3,0,SEEK_SET);
	write(fd1,Buf1,strlen(Buf1));
	write(fd2,Buf2,strlen(Buf2));
	write(fd3,Buf3,strlen(Buf3));
	close(fd1);
	close(fd2);
	close(fd3);
	return num;
};
void delete(char *id,char *ps)
{
	int fd1,fd2;
	int num=CHECK(Buf1,id);
	Buf1[num-1]='!';
	num=CHECK(Buf2,ps);
	Buf2[num-1]='!';
	fd1=open("Personal_ID.txt",O_RDWR|O_CREAT,0644);
	fd2=open("Personal_PASSWORD.txt",O_RDWR|O_CREAT,0644);
	write(fd1,Buf1,strlen(Buf1));
	write(fd2,Buf2,strlen(Buf2));
	close(fd1);
	close(fd2);
};
void error_handling(char *message)
{
        fputs(message,stderr);
        fputs("\n",stderr);
        exit(1);
}


