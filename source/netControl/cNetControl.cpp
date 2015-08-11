#include "cNetControl.h"

void cNetControl::sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *cNetControl::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int cNetControl::InitNetServer(int own)
{
    thisOwner = own;
    yes=1;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    //Getting the addressinfo of your local machine
    if ((rv = getaddrinfo(NULL, PORT.c_str(), &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)

        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(listen_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(listen_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(listen_fd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

 /*   sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }*/

    printf("server: waiting for connections...\n");

    sin_size = sizeof their_addr;
    sockfd = accept(listen_fd, (struct sockaddr *)&their_addr, &sin_size);

    if (sockfd == -1)
    {
        perror("accept");
    }

    inet_ntop(their_addr. ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);

    printf("server: got connection from %s\n", s);

    //no longer waiting for connections
    close(listen_fd);

    //preparing the polling
    n = sockfd + 1;
    //Load the ReadFd to be focused on the corresponding socket
    FD_ZERO(&read_fd);
    FD_SET( sockfd ,&read_fd);

    //set waitingtime
    //waiting time very low
    tv.tv_sec = 0;
    tv.tv_usec = 50000;

    return 1;
}

int cNetControl::InitNetClient(int own)
{
    thisOwner = own;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    Error("test");

//    host = "192.168.178.42";

    Error("test");

    if ((rv = getaddrinfo(IP_ADDY.c_str(), PORT.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    Error("test");

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        Error("loop ");

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        Error("loop2 ");

        break;
    }

    Error("test");

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    Error("test2");

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    Error("test2");

    freeaddrinfo(servinfo); // all done with this structure

    Error("SOCKET ",sockfd);

    //getting the read_fd ready
    FD_ZERO(&read_fd);
    FD_SET( sockfd ,&read_fd);
    n = sockfd + 1;
    //waiting time very low
    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    return 1;
}
void cNetControl::NetworkManage(Uint32 t,cNetPackageList* toSend ,cNetPackageList* toStrip)
{
//    int numberInList = 1;
    NetPackage* toPack;
    NetPackage* toUse;

    ///DEBUGING
    Error("Starting NetworkManage");

    while((toPack = toSend->getFirst()) != NULL)
    {
        //packIt
        //packNetPack(toPack,package);
        //pack & send It
        ///DEBUGING
        Error("sending");
        sending(toPack);
        Error("doneloop");
    }

    ///DEBUGING
    Error("receiving");
    //receiving
    ///TEST
    int test;
    //waiting time very low
    FD_ZERO(&read_fd);
    FD_SET( sockfd ,&read_fd);
    tv.tv_sec = 0;
    tv.tv_usec = 5000;
    test = select(n,&read_fd,NULL,NULL,&tv);
    Error("select does: ",test);
    while(test > 0)
    {
        ///DEBUGING
        Error("receiving item");
        toUse = new NetPackage;

        receive(toUse);

        ///TEST
        //waiting time very low
        tv.tv_sec = 0;
        tv.tv_usec = 5000;
        FD_ZERO(&read_fd);
        FD_SET( sockfd ,&read_fd);
        (test = select(n,&read_fd,NULL,NULL,&tv));
        Error("select does: ",test);

        toStrip->Add(toUse);

/*        if(toUse->owner == thisOwner)
        {
            toStrip->RemoveID(toUse->packID);
        }
        else
        {

            //check if this ID was seen before
            if(usedID(toUse->packID) == false)
            {
                //use package
                toStrip->Add(toUse);
            }
            //tell owner that it arrived
            sending(package);
        }*/
    }
}

int cNetControl::sending(NetPackage* toPack)
{
    char sending[NET_PACKSIZE];
    char buf1[4];
    char buf2[4];
    //char buf3[8];
    sprintf(buf1, "%i", toPack->packID);
    sprintf(buf2, "%i", toPack->owner);
    for(int i=0; i<4;i++)
    {
        sending[i] = buf1[i];
    }
    for(int i=4; i<8;i++)
    {
        sending[i] = buf2[i-4];
    }
    for(int i=8; i<NET_PACKSIZE;i++)
    {
        sending[i] = toPack->packageCore[i-8];
    }
    int test;
    test = atoi(buf2);
    Error("test",test);

    ///DEBUG
    //sending[0] = 1;
    //for(int i=1; i<NET_PACKSIZE;i++)
    //{
        //sending[i] = 0;
    //}

    ///DEBUGING
    Error("actual send");
    Error(sending);

    if (send(sockfd,sending , NET_PACKSIZE, 0) == -1) {perror("send");}
    Error("sending content");
    Error(sending);
    return 1;

    /*int test;
    test = 10;
    char buf1[4];
    sprintf(buf1, "%i", test);
    Error("actual sending");
    Error(buf1);

    if (send(sockfd,buf1 , 4, 0) == -1) {perror("send");}*/

    return 1;
}

int cNetControl::receive(NetPackage* toUse)
{
    ///DEBUGING
    Error("actual receive");
    char stripping[NET_PACKSIZE];
    char buf1[4];
    char buf2[4];
    if ((recv(sockfd, stripping, NET_PACKSIZE, 0)) == -1) {perror("receive");}
    Error(stripping);

    for(int i=0; i<4;i++)
    {
         buf1[i] = stripping[i];
    }
    for(int i=4; i<8;i++)
    {
        buf2[i-4] = stripping[i];
    }
    for(int i=8; i<NET_PACKSIZE;i++)
    {
        toUse->packageCore[i-8] = stripping[i];
    }
    toUse->packID = atoi(buf1);
    toUse->owner = atoi(buf2);

    ///DEBUGING
    Error("did all rearranging");

    return 1;
}

