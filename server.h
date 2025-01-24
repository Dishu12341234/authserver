#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <map>
#include <functional>

namespace Server
{
    extern int serverSocket;
    extern int clientSocket;
    extern int port;
    extern sockaddr_in serverAddr;
    extern std::vector<std::pair<std::string, std::function<void()>>> GET_ROUTES;
    // extern std::vector<std::map<std::string,std::function<void>>> POST_ROUTES;

    bool initilizeServer(int port = 80);
    void serverBind();
    void serverHandleClients();
    void serverClose();

    void routeEndpoint(const std::string &buffer);
    std::vector<std::string> tokenizeBuffer(std::string buffer);

}

#endif