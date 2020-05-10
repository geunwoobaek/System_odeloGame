#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/stat.h>
#define BUFSIZE 1024
int clientNum=0;
int serv_sock;
int fd1,fd2,fd3;
static char* Buf1;
static char* Buf2;
static char* Buf3;
int CHECK(char *Buf_,char *personal);
int SIGNUP(char *ID,char *PS);
int SIGNIN(char *ID,char *PS);
void delete(char *id,char *ps);
int WITHDRAWAL(char *ID);
int wait=0;

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
information info2;
struct sockaddr_in serv_addr;
void *threadfunc(void *argumentPointer);
void error_handling(char *message)
{
	fputs(message,stderr);
	fputs("\n",stderr);
	exit(1);
}
int main(int argc, char **argv)
{
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
	pthread_t threadID[2];	
	void *thread_result;
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock==-1)
		error_handling("socket() error");
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("bind()error");
	if(listen(serv_sock,2)==-1)
		error_handling("listen() error");

	for (int i=0; i<2;i++)
		pthread_create(&threadID[i],NULL,threadfunc,(void *)(2-i));
	pthread_join(threadID[1],&thread_result);
	pthread_join(threadID[0],&thread_result);
	return 0;
}
void *threadfunc(void *argumentPointer)
{	
	int clnt_sock;
	char message[BUFSIZE];
	int str_len;
	int IDSIZE;
	char enter[100];
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	int num1,num2;
	while(1)
	{	
		clnt_addr_size=sizeof(clnt_addr);
		clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
		if(clnt_sock==-1)
			error_handling("accept() error");
		sprintf(enter,"\nclient : %d\n enter",argumentPointer);
		write(1,enter,strlen(enter));
		write(clnt_sock,(void *)&info,sizeof(info));
	while((str_len=read(clnt_sock,(void *)&info,sizeof(info)))!=0)
	{		
			if((strncmp(info.message,"SIGNUP",6)==0))
			{	
				if(CHECK(Buf1,info.tempid)==0)
				{
				write(1,"success SIGNUP ",15);
				SIGNUP(info.tempid,info.tempps);
				strcpy(info.message,"success");
				}
				else
				{strcpy(info.message,"failed");
				write(1,"failed SIGNUP ",15);
				}
			}
			 else if((strncmp(info.message,"SIGNIN",6)==0))
                        {
                                if((SIGNIN(info.tempid,info.tempps)!=0)&&(strncmp(info.loginID[0],info.tempid,strlen(info.tempid))!=0)&&(strncmp(info.firstID,info.tempid,strlen(info.tempid))!=0)&&(strncmp(info.loginID[1],info.tempid,strlen(info.tempid))!=0))                                  
                              	{	if(strlen(info.loginID[0])==0)
					strcpy(info.loginID[0],info.tempid);	
					else if(strlen(info.loginID[1])==0)
					strcpy(info.loginID[1],info.tempid);
					strcpy(info.message,"success");
                                        write(1,"success SIGNIN ",15);	
				}                           
                                else
                                {strcpy(info.message,"failed");
				write(1,"failed SIGNIN ",15);
                                }

                        }
			  else if((strncmp(info.message,"MAIN_S",6)==0))
                                {
                                        if(strncmp(info.tempid,info.firstID,strlen(info.firstID))==0)
                                        memset(info.firstID,0,100);
					else if(strncmp(info.tempid,info.secondID,strlen(info.secondID))==0)
					memset(info.secondID,0,100);	
                                        write(1,"go main!\n ",10);
					memset(info.tempid,0,20);
                                    	if(strncmp(info.tempid,info.loginID[0],strlen(info.tempid))==0)
					memset(info.loginID[0],0,20);
					if(strncmp(info.tempid,info.loginID[1],strlen(info.tempid))==0)
                                        memset(info.loginID[1],0,20);
                                }


			else if((strncmp(info.message,"DELETE",6)==0))
			{
				if(SIGNIN(info.tempid,info.tempps)!=0)
                                {strcpy(info.message,"success");
				delete(info.tempid,info.tempps);
				write(1,"success DELETE ",15);
				}
                                else
                                {strcpy(info.message,"failed");
				write(1,"failed DELETE ",15);
                                }

			}
			else if((strncmp(info.message,"LOGIN",5)==0))
			{	
				num1=CHECK(Buf1,info.firstID);
			        info.win[0]=(int)Buf3[((int)Buf1[num1]-1)*3+1];
       				info.lose[0]=(int)Buf3[((int)Buf1[num1]-1)*3+2];
				if(strlen(info.firstID)==0)
                                {strncpy(info.firstID,info.tempid,strlen(info.tempid));        
				}
				else if(strlen(info.secondID)==0)
                                strncpy(info.secondID,info.tempid,strlen(info.tempid));

				if(strlen(info.firstID)!=0&&strlen(info.secondID)==0)
				{strcpy(info.message,"waiting");
				}
				if(strlen(info.firstID)!=0&&strlen(info.secondID)!=0)
				{
				num2=CHECK(Buf1,info.secondID);
                                info.win[1]=(int)Buf3[((int)Buf1[num2]-1)*3+1];
                                info.lose[1]=(int)Buf3[((int)Buf1[num2]-1)*3+2];
				wait=2;
				info2=info;
				}
			if(strlen(info.firstID)!=0)
			memset(info.tempid,0,20);
			}

			if((strncmp(info.message,"waiting",7))!=0)
			{write(1,info.message,strlen(info.message));
			write(1,"\n",1);
			}
			if((strncmp(info.message,"waiting",7)==0)&&(strlen(info2.secondID)!=0))
			write(clnt_sock,(void*)&info2,sizeof(info2));
			else
			write(clnt_sock,(void*)&info,sizeof(info));
			memset(info.message,0,BUFSIZE);
			memset(info.tempid,0,20);
			memset(info.tempps,0,20);
			}	
			
	}
		close(clnt_sock);
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



