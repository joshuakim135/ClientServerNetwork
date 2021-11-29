#ifndef _TCPRequestChannel_H_
#define _TCPRequestChannel_H_

#include "common.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>


class TCPRequestChannel {
    private:
        int sockfd;
    public:
        TCPRequestChannel (const string host_name, const string port_no);
        TCPRequestChannel(int);
        ~TCPRequestChannel();
        int cread(void* msgbuf, int buflen);
        int cwrite(void* msgbuf, int msglen);
        int serverSetUp(const string port);
        int clientSetUp(const string host, const string port);
        int getfd() {
            return sockfd;
        };
};

#endif