#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#define PORT 5100 
#define BUFFSIZE 777777
#define MAX 100000
typedef struct P{
    int id;
    int remainingtime;
    int Round;
    int socket;
    char* command;
}Info;

void Die(char *mess) 
{
    perror(mess);
    exit(1);
}


void* HandleClient(void* param){

    Info* info=(Info*) param;
    int sock=info->socket;
    char buffer[BUFFSIZE];
    int j=0;
    while(info->command[j] != '\0'){
        buffer[j]=info->command[j];
        j++;
    }
    memset(info->command,0,BUFFSIZE);
    buffer[j]='\0';
    char *token;
    char arr[10][MAX];

while(1) { 
   
    
    token=strtok(buffer," ");
        int i=0;
        while(token!=NULL){//parsing (to split words that are separated by spaces)
            strcpy(arr[i],token);
            token=strtok(NULL," ");//token is the word before an empty space
            i++;
        }

    char* cmd= arr[0];
    char* tok=strtok(cmd,"\n");//to remove the new line 
        if (strcmp(tok, "exit")==0) 
            exit(0);

        if(strcmp(tok,"listFiles")==0){//ls
            int pipefd[2],lenght;

        if(pipe(pipefd))//create the pipe
            Die("Failed to create pipe");

        pid_t pid = fork();
        char path[MAX];//to contain the output of command

        if(pid==0)
        {
            close(1);//to close output in child process 
            dup2(pipefd[1],1); //to redirect output to write end of pipe
            close(pipefd[0]); 
            close(pipefd[1]); 
            execlp("ls","ls",NULL); 
        }
        else
            if(pid>0)
            {
                close(pipefd[1]);
                memset(path,0,MAX);
                lenght=read(pipefd[0],path,MAX-1);
                    
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        Die("Failed");
                    }
                   
                    
                memset(path,0,MAX);
                
                close(pipefd[0]);
                
            }
            else
            {
                printf("Error !\n");
                exit(0);
            }}
            else if(strcmp(tok,"PrintTill")==0){
                
                char* cmd2= arr[1];
                char* tok2= strtok(cmd2,"\n");
                send(sock,tok2,strlen(tok2),0);

            }
     else if(strcmp(tok,"Printproc")==0){//ps
         int pipefd[2],lenght;

        if(pipe(pipefd))
            Die("Failed to create pipe");

        pid_t pid = fork();
        char path[MAX];

        if(pid==0)
        {
            close(1); 
            dup2(pipefd[1],1); 
            close(pipefd[0]); 
            close(pipefd[1]); 
            execlp("ps","ps",NULL); 
        }
        else
            if(pid>0)
            {
                close(pipefd[1]);
                memset(path,0,MAX);
                lenght=read(pipefd[0],path,MAX-1);
                   
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        Die("Failed");
                    }
                   
                   
                memset(path,0,MAX);
                
                close(pipefd[0]);
                
            }
            else
            {
                printf("Error !\n");
                exit(0);
            }}
             else if(strcmp(tok,"Printproc|count")==0){//ps|wc -l
                int lenght;
                int fd[2];
                int fd2[2];
                char path[MAX];
                pipe(fd);
                pid_t pid=fork();
               
                if(pid==0){
                    close(1);
                    dup(fd[1]);
                    close(fd[0]);
                    close(fd[1]);
                    execlp("ps","ps",NULL);
                }
                pipe(fd2);
                int pid2=fork();
                if(pid2==0){
                    close(0);
                    close(1);
                    dup(fd[0]);
                    close(fd[0]);
                    close(fd[1]);
                    dup(fd2[1]);
                    close(fd2[0]);
                    close(fd2[1]);
                    execlp("wc","wc","-l",NULL);
                }
                close(fd[0]);
                close(fd[1]);
                close(fd2[1]);
                 memset(path,0,MAX);
                lenght=read(fd2[0],path,MAX-1);
                   
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        Die("Failed");
                    }
                  
                   
                memset(path,0,MAX);
                
                close(fd2[0]);
            }
        else if(strcmp(tok,"printDir")==0){//pwd
            int fd[2],lenght;
            char path[MAX];
            pipe(fd);
            pid_t pid=fork();
            if(pid==0){
                close(1);
                dup(fd[1]);
                close(fd[0]);
                close(fd[1]);
                execlp("pwd","pwd",NULL);
            }
            
            close(fd[1]);
                memset(path,0,MAX);
                lenght=read(fd[0],path,MAX-1);
                   
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        Die("Failed");
                    }
                   
                   
                memset(path,0,MAX);
                
                close(fd[0]);
           
        }
        else if(strcmp(tok,"listFiles|find")==0){ //ls|grep (word)|wc -l
                char *cmd3=arr[2];
                char *tok3=strtok(cmd3,"\n");
                if(strcmp(tok3,"|count")==0){
                    char* cmd2=arr[1];
                    char* tok2=strtok(cmd2,"\n");
                    int lenght;
                    char path[MAX];
                    int fd[2];
                    int fd2[2];
                    int fd3[2];
                    pipe(fd);
                    pid_t pid=fork();
                    if(pid==0){
                        close(1);
                        dup(fd[1]);
                        close(fd[0]);
                        close(fd[1]);
                        execlp("ls","ls","-l",NULL);
                    }
                pipe(fd2);
                int pid2=fork();
                if(pid2==0){
                    close(1);
                    close(0);
                    dup(fd[0]);
                    close(fd[0]);
                    close(fd[1]);
                    dup(fd2[1]);
                    close(fd2[0]);
                    close(fd2[1]);
                    execlp("grep","grep",tok2,NULL);
                 }
                close(fd[0]);
                close(fd[1]);
                pipe(fd3);
                int pid3=fork();
                if(pid3==0){
                    close(0);
                    close(1);
                    dup(fd2[0]);
                    close(fd2[0]);
                    close(fd2[1]);
                    dup(fd3[1]);
                    close(fd3[0]);
                    close(fd3[1]);
                    execlp("wc","wc","-l",NULL);
                }       
                close(fd2[0]);
                close(fd2[1]);
                close(fd3[1]);
                memset(path,0,MAX);
                lenght=read(fd3[0],path,MAX-1);
                  
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        Die("Failed");
                    }
            
                close(fd3[0]);

        }   }
        else if(strcmp(tok,"listFiles|display")==0){//ls|grep word
                char* cmd2=arr[1];
                char* tok2=strtok(cmd2,"\n");
                int fd[2];
                int fd2[2];
                int lenght;
                char path[MAX];
                pipe(fd);
                pid_t pid=fork();
                if(pid==0){
                    close(1);
                    dup(fd[1]);
                    close(fd[0]);
                    close(fd[1]);
                    execlp("ls","ls",NULL);
                }
                pipe(fd2);
                int pid2=fork();
                if(pid2==0){
                    close(0);
                    close(1);
                    dup(fd[0]);
                    close(fd[0]);
                    close(fd[1]);
                    dup(fd2[1]);
                    close(fd2[0]);
                    close(fd2[1]);
                    execlp("grep","grep",tok2,NULL);
                }
                close(fd[0]);
                close(fd[1]);
                close(fd2[1]);
                memset(path,0,MAX);
                lenght=read(fd2[0],path,MAX-1);
                   
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        Die("Failed");
                    }
                  
                   
                memset(path,0,MAX);
                
                close(fd2[0]);
            }
            else if(strcmp(tok,"search")==0){//cat (filename)|grep (word)|uniq|wc -l
                char *cmd3=arr[2];
                char *tok3=strtok(cmd3,"\n");
                if(strcmp(tok3,"|find")==0){
                    char *cmd5=arr[4];
                    char *tok5=strtok(cmd5,"\n");
                    if(strcmp(tok5,"|unique|count")==0){
                        char path[MAX];
                        int lenght;
                        char *cmd2=arr[1];
                        char *tok2=strtok(cmd2,"\n");
                        char *cmd4=arr[3];
                        char *tok4=strtok(cmd4,"\n");
                        int fd[2];
                        int fd2[2];
                        int fd3[2];
                        int fd4[2];
                        pipe(fd);
                        pid_t pid=fork();
                        if(pid==0){
                            close(1);
                            dup(fd[1]);
                            close(fd[0]);
                            close(fd[1]);
                            execlp("cat","cat",tok2,NULL);
                        }
                        pipe(fd2);
                        int pid2=fork();
                        if(pid2==0){
                            close(0);
                            close(1);
                            dup(fd[0]);
                            close(fd[0]);
                            close(fd[1]);
                            dup(fd2[1]);
                            close(fd2[0]);
                            close(fd2[1]);
                            execlp("grep","grep",tok4,NULL);
                        }
                        close(fd[0]);
                        close(fd[1]);
                        pipe(fd3);
                        int pid3=fork();
                        if(pid3==0){
                            close(0);
                            close(1);
                            dup(fd2[0]);
                            close(fd2[0]);
                            close(fd2[1]);
                            dup(fd3[1]);
                            close(fd3[0]);
                            close(fd3[1]);
                            execlp("uniq","uniq",NULL);
                        }
                        close(fd2[0]);
                        close(fd2[1]);
                        pipe(fd4);
                        int pid4=fork();
                        if(pid4==0){
                            close(0);
                            close(1);
                            dup(fd3[0]);
                            close(fd3[0]);
                            close(fd3[1]);
                            dup(fd4[1]);
                            close(fd4[1]);
                            close(fd4[0]);
                            execlp("wc","wc","-l",NULL);
                        }
                        close(fd3[0]);
                        close(fd3[1]);
                        close(fd4[1]);
                        memset(path,0,MAX);
                    lenght=read(fd4[0],path,MAX-1);
                  
                        if(send(sock,path,strlen(path),0) != strlen(path) ){
                            Die("Failed");
                    }
                  
                   
                     memset(path,0,MAX);
                 
                close(fd4[0]);
                    }
                }
            }
            else if(strcmp(tok,"showContent")==0){//cat (filename)
                char *cmd2=arr[1];
                char *tok2=strtok(cmd2,"\n");
                int lenght;
                char path[MAX];
                int fd[2];
                pipe(fd);
                pid_t pid=fork();
                if(pid==0){
                    close(1);
                    dup(fd[1]);
                    close(fd[1]);
                    close(fd[0]);
                    execlp("cat","cat",tok2,NULL);
                }
                close(fd[1]);
                memset(path,0,MAX);
                    lenght=read(fd[0],path,MAX-1);
                  
                        if(send(sock,path,strlen(path),0) != strlen(path) ){
                            Die("Failed");
                    }
                   
                   
                    memset(path,0,MAX);
                 
                 close(fd[0]);
            }
            else if(strcmp(tok,"listFiles|count")==0){//ls|wc -l
                int lenght;
                int fd[2];
                int fd2[2];
                char path[MAX];
                pipe(fd);
                pid_t pid=fork();
               
                if(pid==0){
                    close(1);
                    dup(fd[1]);
                    close(fd[0]);
                    close(fd[1]);
                    execlp("ls","ls",NULL);
                }
                pipe(fd2);
                int pid2=fork();
                if(pid2==0){
                    close(0);
                    close(1);
                    dup(fd[0]);
                    close(fd[0]);
                    close(fd[1]);
                    dup(fd2[1]);
                    close(fd2[0]);
                    close(fd2[1]);
                    execlp("wc","wc","-l",NULL);
                }
                close(fd[0]);
                close(fd[1]);
                close(fd2[1]);
                 memset(path,0,MAX);
                lenght=read(fd2[0],path,MAX-1);
                   
                    if(send(sock,path,strlen(path),0) != strlen(path) ){
                        Die("Failed");
                    }
                 
                  
                memset(path,0,MAX);
                
                close(fd2[0]);
            }
             else if(strcmp(tok,"kill")==0){ //kill (pid)
                char *cmd2 =arr[1];
                char *tok2=strtok(arr[1],"\n");
                kill(atoi(tok2),SIGINT);
                char msg[]="Killed task successfully.\n";
                send(sock,msg,strlen(msg),0);
        }
        else if(strcmp(tok,"copy")==0){ 
            char *cmd2=arr[1];
            char *tok2=strtok(cmd2,"\n");
            char *cmd3=arr[2];
            char *tok3=strtok(cmd3,"\n");
            pid_t pid=fork();
            if(pid==0){
                execlp("cp","cp",tok2,tok3,NULL);
            }
            char msg[]="File copied successfully.\n";
            send(sock,msg,strlen(msg),0);
        }
        else if(strcmp(tok,"delete")==0){// rm (filename)
            char *cmd2=arr[1];
            char *tok2=strtok(cmd2,"\n");
            
                pid_t pid=fork();
                if(pid==0){
                    execlp("rm","rm",tok2,NULL);
                }
                char msg[]="Deleted file successfully.\n";
                    send(sock,msg,strlen(msg),0);
                }
            else if (strcmp (tok, "exit")==0){
    
                exit (1);
    
                 printf("Closing socket\n");
                close(sock);
            }
            else{
                char msg[]="Invalid Command.\n";
                send(sock,msg,strlen(msg),0);
            }   
            memset(buffer,0,BUFFSIZE);     
            int received2=recv(info->socket,buffer,BUFFSIZE-1,0);
            buffer[received2]='\0';
}

}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; //server socket and client socket
        struct sockaddr_in address; 
        int addrlen = sizeof(address); 
        //initializations
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        { 
                Die("Socket creation failed.\n");
        } 

        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( PORT ); 

      
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
        { 
                Die("Bind failed.\n");
        } 
        
        
        
        if (listen(server_fd, 10) < 0) 
        { 
                Die("error in listening"); 
                
        }
        close(1);//close server's output
     int q[10000];
     Info infarr[100];
     int i=0;
     char buff[BUFFSIZE];
     while(1)
    { 


         if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                        (socklen_t*)&addrlen))<0) 
        { 
               Die("accept error"); 
                
        }
        int received= recv(new_socket,buff,BUFFSIZE-1,0);
        buff[received]='\0';
         char *token;
        char arr[10][MAX];
    
        token=strtok(buff," ");
        int i=0;
        while(token!=NULL){//parsing (to split words that are separated by spaces)
            strcpy(arr[i],token);
            token=strtok(NULL," ");//token is the word before an empty space
            i++;
        }
        char* cmd= arr[0];
        char* tok=strtok(cmd,"\n");
        Info inf;
        inf.command=(char *)buff;
        inf.id=i;
        inf.Round=1;
        inf.socket=new_socket;
        infarr[i]=inf;
        int flag=0;
        if(strcmp(tok,"PrintTill")==0){
            flag=1;
            char* cmd2=arr[1];
            char* tok2=strtok(cmd2,"\n");
            inf.remainingtime=(int) tok2;
        }else{
            inf.remainingtime=1;
        }
        pthread_t th;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        int min=-1;
        int minIndex=-1;
        //scheduler initialization
        for(int j=0;j<=i;j++){
            if(infarr[j].remainingtime<min){
                min=infarr[j].remainingtime;
                minIndex=j;
            }
        }
        int allocTime;
        int R=infarr[minIndex].Round;
        if(R=1){
            allocTime=4;
        }
        if(R>1){
            allocTime=100;
        }
        if(flag==1){
            int tmp=neededInf.remainingtime;
            for(int k=0;k<allocTime;k++){
                printf("%d ",neededInf.remainingtime);
                neededInf.remainingtime--;
                neededInf.Round++;
                sleep(1);
            }
        }else{
            pthread_create(&th,&attr,HandleClient,(void*) &inf)
        }
        pthread_create(&th,&attr,HandleClient,(void*) &inf);//the thread handles sending the output of commands to the client   
        
    }
    close(server_fd);
    return 0;
}