#include <iostream>
#include "server.h"
#include "routes.h"
namespace Server
{
    int serverSocket, clientSocket = 0;
    int port;
    sockaddr_in serverAddr;
    std::vector<std::pair<std::string, std::function<void()>>> GET_ROUTES;

    bool initilizeServer(int Sport)
    {
        serverSocket = 0;
        port = Sport;

        std::cout << "Initilizing HTTP server" << std::endl;
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0)
        {
            perror("Failed creation of server socket");
            return false;
        }

        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            perror("Error setting SO_REUSEADDR");
            exit(EXIT_FAILURE);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        serverBind();

        if (listen(serverSocket, 5) < 0)
        {
            perror("Listen failed");
            return false;
        }
        else
        {
            std::cout << "Server is listening on port " << port << std::endl;
            return true;
        }
        return true;
    }

    void serverBind()
    {
        std::cout << "Server in binding stage" << std::endl;
        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            perror("Failed to bind the server");
        }
    }

    void serverHandleClients()
    {
        // std::cout << "Server is accepting connections on port " << port << std::endl;

        clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0)
        {
            perror("Error in accept");
            return;
        }

        char buffer[2048] = {0};

        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0)
        {
            perror("Error in recv");
            close(clientSocket);
            return;
        }

        if (bytesReceived == 0)
        {
            close(clientSocket);
            return;
        }

        buffer[bytesReceived] = '\0';

        std::cout << "Client:\n"
                  << "\x1B[36m" << buffer << " \x1B[0m " << std::endl;
        std::cout << "Message Length: " << "\x1B[33m " << bytesReceived << " bytes" << " \x1B[0m " << std::endl;

        Routes::clientSocket = clientSocket;
        routeEndpoint(buffer);

        close(clientSocket);
    }

    void routeEndpoint(const std::string &buffer)
    {
        std::vector<std::string> tokens = tokenizeBuffer(buffer);
        if (tokens.size() == 0)
            return;

        std::string methodParam = tokens.at(0);

        std::string method = methodParam.substr(0, methodParam.find(" "));
        std::string route = methodParam.substr(method.size() + 1, std::string(methodParam.substr(method.size() + 1)).find(" "));

        std::cout << method << std::endl;
        std::cout << route << std::endl;
        if (method == "GET")
            for (size_t i = 0; i < GET_ROUTES.size(); i++)
            {
                for (auto &&token : tokens)
                {
                    if (token.substr(0, 16) == "Accept: text/css" || token.substr(0, 24) == "Sec-Fetch-Dest: style" || methodParam.substr(method.size() + route.size() - 3).substr(0,4) == ".css")
                    {
                        if (route[0] == '/')
                            Routes::serveStaticCSS(route.substr(1));
                        else
                            Routes::serveStaticCSS(route);
                        return;
                    }
                }
                if (GET_ROUTES.at(i).first == route)
                {
                    GET_ROUTES.at(i).second();
                }
            }

        // for (const auto &str : tokens)
        // {
        //     std::cout << "---->" << str << std::endl;
        // }
    }

    std::vector<std::string> tokenizeBuffer(std::string buffer)
    {
        std::string delimeter = "\n";
        std::vector<std::string> tokens{};

        size_t pos = 0;
        while ((pos = buffer.find(delimeter)) != std::string::npos)
        {
            tokens.push_back(buffer.substr(0, pos));
            buffer.erase(0, pos + delimeter.length());
        }
        if (!buffer.empty())
            tokens.push_back(buffer.substr(0, pos));
        return tokens;
    }

    void serverClose()
    {
        std::cout << "shutting down the server" << std::endl;
        close(serverSocket);
    }
}