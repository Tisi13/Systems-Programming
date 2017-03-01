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
    int fd, resolv, nb, counter, counterres;
    fd_set read_set;
    
    struct addrinfo hints, *result;
    long int numbytes, numbytesresv;
    
    char msg[MAX_MSG_LEN], msgres[MAX_MSG_LEN];
    struct timeval start, end, timeout;
    
    if (argc != 2)
    {
        fprintf(stderr, " Enter a host name or IP address\n");
        exit(1);
    }
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
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
    
    
    
    for (counter = 1; ;counter++)
    {
        
        sprintf( msg, "%d", counter);
        
        gettimeofday(&start, NULL);
        
        numbytes = sendto(fd, msg, strlen(msg)+1, result->ai_flags,
                          result->ai_addr,result->ai_addrlen);
        
        if (numbytes == -1)
        {
            perror("sendto");
            exit(1);
        }
        
        FD_ZERO(&read_set);
        FD_SET(fd, &read_set);
        
        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;
        nb = select(fd+1, &read_set, NULL, NULL, &timeout);
        
        if (nb<0)
        {
            perror("select");
            exit(1);
        }
        
        if (nb == 0)
        {
            printf("packet%d: lost\n",counter);
            continue;
        }
        
        if (FD_ISSET(fd,&read_set))
        {
            numbytesresv = recvfrom(fd, msgres, MAX_MSG_LEN,
                                    result->ai_flags, result->ai_addr,&(result->ai_addrlen));
            
            if (numbytesresv == -1)
            {
                perror("recvfrom");
                close(fd);
                exit(1);
            }
            
            gettimeofday(&end, NULL);
            
            counterres = atoi(msgres);
            
            if (counter != counterres)
            {
                printf("Wrong counter! Received counter %d instead of  %d\n", counterres, counter);
            }
            else
            {
                printf("Packet%d: %0g micro-seconds\n",counter, (end.tv_sec - start.tv_sec)*1E6 +(end.tv_usec - start.tv_usec));
            }
        }
        usleep(1000000 - (end.tv_usec - start.tv_usec));
    }
    
    freeaddrinfo(result);
    
    return 0;
}



