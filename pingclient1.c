#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#define SERVER_PORT "9000"
#define MAX_MSG_LEN  1000

int main(int argc, char *argv[])
{
    
    int fd, resolv;
    
    struct addrinfo hints, *result;
    long int numbytes,  numbytesresv;
    
    char msg[MAX_MSG_LEN];
    struct timeval start, end;
    
    if (argc != 2)
    {
        fprintf(stderr, " Enter a host name or IP address\n");
        exit(1);
    }
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    
    resolv = getaddrinfo(argv[1], SERVER_PORT, &hints, &result);
    
    if (resolv != 0)
    {
        fprintf(stderr, "getaddrinfo:%s\n", gai_strerror(resolv));
        return 1;
    }
    
    fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    
    if (fd <= -1)
    {
        perror("socket");
        return 1;
    }
    
    
    strcpy(msg, "Hello, world!");
    
    gettimeofday(&start, NULL);
    
    numbytes = sendto(fd, msg, strlen(msg)+1, result->ai_flags,
                      result->ai_addr,result->ai_addrlen);
    
    if (numbytes == -1)
    {
        perror("sendto");
        exit(1);
    }
    
    numbytesresv = recvfrom(fd, msg, MAX_MSG_LEN,
                            result->ai_flags, result->ai_addr,&( result->ai_addrlen));
    
    if (numbytesresv == -1)
    {
        perror("recvfrom");
        close(fd);
        exit(1);
    }
    gettimeofday(&end, NULL);
    
    printf("Packet: %0g micro-seconds\n",(end.tv_sec - start.tv_sec)*1E6 +(end.tv_usec - start.tv_usec));
    
    freeaddrinfo(result);
    
    return 0;
    
}   
