#include "TCPRequestChannel.h"
#include <thread>

void connection_handler (int client_socket) {
    char buf[1024];
    while (true) {
        if (recv (client_socket, buf, sizeof(buf), 0) < 0) {
            perror("server: Receive failure");
            exit(0);
        }
        int num = *(int*) buf;
        num *= 2;
        if (num == 0) {
            break;
        }
        if (send(client_socket, &num, sizeof(num), 0) == -1) {
            perror("send");
            break;
        }
        cout << "Closing client socket" << endl;
        close(client_socket);
    }
}

void talk_to_server (int sockfd) {
    char buf[1024];
    while (true) {
        cout << "Type a number for the server: ";
        int num;
        cin >> num;
        if (send (sockfd, &num, sizeof(int), 0) == -1) {
            perror("client: Send failure");
            break;
        }
        if (recv(sockfd, buf, sizeof(buf), 0) < 0) {
            perror("client: Receive failure");
            exit(0);
        }
        cout << "The server sent back " << *(int*) buf << endl; 
    }
}

int TCPRequestChannel::serverSetUp(const string port) {
    struct addrinfo hints, *serv;
    struct sockaddr_storage their_addr; // connector's addr info
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my ip

    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &serv)) != 0) {
        cerr << "getaddrinfo: " << gai_strerror(rv) << endl;
        return -1;
    }
    if ((sockfd == socket(serv->ai_family, serv->ai_socktype, serv->ai_protocol)) == -1) {
        perror("server: socket");
        return -1;
    }
    if (bind(sockfd, serv->ai_addr, serv->ai_addrlen) == -1) {
        close(sockfd);
        perror("server: bind");
        return -1;
    }
    freeaddrinfo(serv);
    if (listen(sockfd, 20) == -1) {
        perror("listen");
        exit(1);
    }

    cout << "server: waiting for connections..." << endl;
    char buf[1024];
    while(1) {
        sin_size = sizeof(their_addr);
        int client_socket = accept(sockfd, (struct sockaddr*) &their_addr, &sin_size);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }
        thread t (connection_handler, client_socket);
        t.detach();
    }
    return 0;
}

int TCPRequestChannel::clientSetUp(const string host, const string port) {
    struct addrinfo hints, *res;
    
    // load up address structs with getaddrinfo()
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int status;
    if ((status =getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0) {
        cerr << "getaddrinfo: " << gai_strerror(status) << endl;
        return -1;
    }

    // make a socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        perror("Cannot create socket");
        return -1;
    }

    // connect!
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("Cannot Connect");
        return -1;
    }

    cout << "Connected" << endl;
    // now free the memory dynamically allocated onto the res pointer by the getaddrinfo function
    freeaddrinfo(res);
    talk_to_server(sockfd);
    return 0;
}

TCPRequestChannel::TCPRequestChannel (const string host_name, const string port_no) {
    // if host name empty string -> server side; else -> client side
    (host_name.empty()) ? serverSetUp(port_no) : clientSetUp(host_name, port_no);
}

TCPRequestChannel::TCPRequestChannel(int s) {
    sockfd = s;
}

TCPRequestChannel::~TCPRequestChannel() {
    close(sockfd);
}

int TCPRequestChannel::cread(void* msgbuf, int buflen) {
    return read(sockfd, msgbuf, buflen);
}

int TCPRequestChannel::cwrite(void* msgbuf, int msglen) {
    return write(sockfd, msgbuf, msglen);
}