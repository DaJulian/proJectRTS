#ifndef CNETCONTROL_H_INCLUDED
#define CNETCONTROL_H_INCLUDED

#define BACKLOG 10     // how many pending connections queue will hold

#define PORTDEF "3700"

#include "cPackageSender.h"

class cNetControl
{
protected:
    //Adress to the socket to be listend on
    int listen_fd;

    //Adress for the client
    int sockfd, numbytes;

    struct addrinfo hints, *servinfo, *p;

    // connector's address information
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    struct sigaction sa;
    int yes;
    char s[INET6_ADDRSTRLEN];
    int rv;

    //NETWORK data
    int n;
    //accepting
    fd_set read_fd;
    struct timeval tv;

    short int thisOwner;

public:
    int InitNetServer(int own);
    int InitNetClient(int own);

    void sigchld_handler(int s);
    void *get_in_addr(struct sockaddr *sa);

    void NetworkManage(Uint32 t,cNetPackageList* toSend ,cNetPackageList* toStrip);

    int sending(NetPackage* toPack);
    int receive(NetPackage* toUse);

    bool usedID(int checkID);
};

#endif // CNETCONTROL_H_INCLUDED
