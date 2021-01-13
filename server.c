#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define PORT 8080
#define BUFSIZE 1024
double what_time_is_it()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char status[15]={0},buffer[BUFSIZE]={0}; 
	    char CC_algo[256];
    int read_size=0,i=1,j;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))                                                
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)                                 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server Waiting\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
		printf("New Connection");
        perror("accept");
        exit(EXIT_FAILURE);
    }
	    socklen_t len = sizeof(CC_algo);
        if (getsockopt(new_socket, IPPROTO_TCP, TCP_CONGESTION, CC_algo, &len) != 0)
    {
        perror("getsockopt");
        return -1;
    }

    printf("Current CC algorithm: %s\n", CC_algo);
	int file_size=0;
	double start_time,end_time,avarage=0;
for (int j=1;j<=5;j++)
{
    start_time = what_time_is_it();

    while((read_size=recv(new_socket,buffer,BUFSIZE,0))>0)
    {
		file_size+=read_size;
                if((strcmp(buffer,"done")==0) || (file_size >= 104857605))
                {
					end_time = what_time_is_it() - start_time;
					avarage+= end_time;
					file_size-=5;
                    printf("Flag received : done, received %d bytes, in %lf",file_size,end_time);
                    break;
                }
        i++;
    }

    printf("\n%d File received\n",j);

    for(i=0;i<BUFSIZE;i++)
    buffer[i]='\0';
    i=1;
	file_size=0;

}
	avarage/=5;
printf("5 Files avarage time:%lf\n",avarage);		
		    strcpy(CC_algo, "reno");
    len = strlen(CC_algo);
        if (setsockopt(new_socket, IPPROTO_TCP, TCP_CONGESTION, CC_algo, len) != 0)
    {
        perror("setsockopt");
        return -1;
    }

    len = sizeof(CC_algo);

    if (getsockopt(new_socket, IPPROTO_TCP, TCP_CONGESTION, CC_algo, &len) != 0)
    {
        perror("getsockopt");
        return -1;
    }

    printf("New CC Algorithm: %s\n", CC_algo);
		avarage=0;
		for (int j=1;j<=5;j++)
{
	start_time=what_time_is_it();

    while((read_size=recv(new_socket,buffer,BUFSIZE,0))>0)
    {
		file_size+=read_size;
                if((strcmp(buffer,"done")==0) || (file_size >= 104857605))
                {
					end_time = what_time_is_it() - start_time;
					avarage+= end_time;
					file_size-=5;
                    printf("Flag received : done, received %d bytes, in %lf",file_size,end_time);
                    break;
                }
        i++;
    }

    printf("\n%d File received\n",j);

    for(i=0;i<BUFSIZE;i++)
    buffer[i]='\0';
    i=1;
	file_size=0;

}
	avarage/=5;
printf("5 Files avarage time:%lf\n",avarage);		
    close(new_socket);
    return 0;
}
