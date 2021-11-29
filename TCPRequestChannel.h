#ifndef _TCPRequestChannel_H_
#define _TCPRequestChannel_H_

#include "common.h"

class TCPRequestChannel {
    private:
        int sockfd;
    public:
        TCPRequestChannel (const string host_name, const string port_no);
        TCPRequestChannel(int);
        ~TCPRequestChannel();
        int cread(void* msgbuf, int buflen);
        int cwrite(void* msgbuf, int msglen);
        void serverSetUp(const string port);
        void clientSetUp(const string host, const string port);
        int getfd() {
            return sockfd;
        };
};

#endif