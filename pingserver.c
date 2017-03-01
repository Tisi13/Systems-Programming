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

#define MYPORT "9000"
#define MAX_MSG_LEN  1000

int main(void) {
    int fd, resolv;
    struct addrinfo hints, *result;
    
    char msg[MAX_MSG_LEN];
    long int numbytes, numbytessent;
    
    struct sockaddr_storage storage_addr;
    socklen_t addr_len;
    
    result = 0;
    
    memset(&hints, 0, sizeof hints); // make sure struct is empty
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE;   //  use my IP
    
    resolv = getaddrinfo(NULL, MYPORT, &hints, &result);
    
    if (resolv != 0) {
        fprintf(stderr, "getaddrinfo:%s\n", gai_strerror(resolv));
        return 1;
    }
    
    /* loop through all the results and bind */
    fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    
    if (fd <= -1) {
        perror("socket");
        return 1;
    }
    
    if (bind(fd, result->ai_addr, result->ai_addrlen) <= -1) {
        perror("bind");
        close(fd);
        return 1;
    }
    
    freeaddrinfo(result);
    
    for (;;) {
        addr_len = sizeof(struct sockaddr_storage);
        
        numbytes = recvfrom(fd, msg, MAX_MSG_LEN, 0,
                            (struct sockaddr *) &storage_addr, &addr_len);
        if (numbytes <= -1) {
            perror("recvfrom");
            close(fd);
            exit(1);
        }
        
        numbytessent = sendto(fd, msg, strlen(msg)+1, 0,
                              (struct sockaddr *) &storage_addr, addr_len);
        if (numbytessent == -1) {
            perror("sendto");
            exit(1);
        }
    }
    close(fd);
    return 0;
}
