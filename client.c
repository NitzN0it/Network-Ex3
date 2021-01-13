#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define PORT 8080
#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char status[15]={0};
    char buffer[BUFSIZE]={0};
    char CC_algo[256];
    int read_size=0,i=1,sent_size=0;    

    FILE *f1,*f2;    
    f1=fopen("100mb.txt","r");
    f2=fopen("100mb.txt","r");
    if(f1==NULL)
    {
        printf("Unable open file\n");
        exit(0);
    }
    if(f2==NULL)
    {
        printf("Unable open file\n");
        exit(0);
    }
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
	int total_sent = 0;
    socklen_t len = sizeof(CC_algo);
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, CC_algo, &len) != 0)
    {
        perror("getsockopt");
        return -1;
    }

    printf("Current CC algorithm: %s\n", CC_algo);
    
    for (int j=1;j<=5;j++)
    {
    while((read_size=fread(buffer,1,BUFSIZE,f1))>0)
    {
        sent_size=send(sock,buffer,read_size , 0 );
		total_sent+=sent_size;
        i++;
    }
    strcpy(status,"done");                      //Flag to be sent to the server to indicate that the file transfer has been completed
    send(sock,status,strlen(status)+1, 0 );
    printf("%d file sent %d bytes\n",j,total_sent);

    for(i=0;i<BUFSIZE;i++)
        buffer[i]='\0';
    i=1;
    total_sent = 0;
    f1=fopen("100mb.txt","r");
    }
    
    strcpy(CC_algo, "reno");
    len = strlen(CC_algo);
        if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, CC_algo, len) != 0)
    {
        perror("setsockopt");
        return -1;
    }

    len = sizeof(CC_algo);

    if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, CC_algo, &len) != 0)
    {
        perror("getsockopt");
        return -1;
    }

    printf("New CC Algorithm: %s\n", CC_algo);
    
     for (int j=1;j<=5;j++)
    {
    while((read_size=fread(buffer,1,BUFSIZE,f1))>0)
    {
        sent_size=send(sock,buffer,read_size , 0 );
		total_sent+=sent_size;
        i++;
    }
    strcpy(status,"done");    //Flag to be sent to the server to indicate that the file transfer has been completed
    send(sock,status,strlen(status)+1, 0 );
    printf("%d file sent %d bytes\n",j,total_sent);

    for(i=0;i<BUFSIZE;i++)
        buffer[i]='\0';
    i=1;
    total_sent = 0;
    f1=fopen("100mb.txt","r");
    }
    
    
    fclose(f1);
    close(sock);
    return 0;
}