 #include <stdio.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <netinet/in.h>
    #define BUFFSIZE 777777
    #define PORT 5100 

    void Die(char *mess) //to print error message
    {
        perror(mess); 
        exit(1); 
    }
    int main(int argc, char *argv[]) 
    {
        int sock = 0; 
        struct sockaddr_in serv_addr; 
        char buffer[BUFFSIZE];//stores the message received from server
        int echolen;
       
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        {
            Die("Failed to create socket");
        }
        
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons(PORT); 
        if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0){
            Die("Inet_pton error\n");
        }
        if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        {
            Die("Failed to connect with server");
        }
        char s[100];//to store the command that the client enters
    while(1) { 
        fgets(s,100,stdin);
        s[strlen(s)-1]='\0'; //to remove new line \n
        echolen = strlen(s);

        
        if (send(sock, s, echolen, 0) != echolen) 
        {
            Die("Mismatch in number of sent bytes");
        }
        if(strcmp(s,"exit") == 0) 
                exit(0);
        int bytes = 0;
        bytes=recv(sock,buffer,BUFFSIZE-1,0);
        buffer[bytes]='\0';//to make sure command output does not have extra characters
        printf("%s\n",buffer);
    }
    return 0;
}