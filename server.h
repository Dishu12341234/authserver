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
    extern std::vector<std::pair<std::string, std::function<void(std::vector<std::pair<std::string,std::string>>)>>> GET_ROUTES;
    extern std::vector<std::pair<std::string, std::function<void(std::vector<std::pair<std::string,std::string>>)>>> POST_ROUTES;

    bool initilizeServer(int port = 80);
    void serverBind();
    void serverHandleClients();
    void serverClose();

    std::vector<std::pair<std::string,std::string>> extractPOSTParams(std::vector<std::string>& tokens);

    void routeEndpoint(const std::string &buffer);
    std::string decodeURLParams(std::string);
    std::vector<std::pair<std::string,std::string>> extractURLParams(std::string &methodParam, std::string &method);
    std::vector<std::string> tokenizeBuffer(std::string buffer);

}

#endif