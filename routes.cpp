#include "routes.h"
#include <iostream>
#include <sys/socket.h>
#include <fstream>
namespace Routes
{
    int clientSocket = 0;

    std::string readFileAsString(std::string filename)
    {
        std::ifstream filebuffer("views/" + filename);
        if (!filebuffer.is_open())
        {
            std::cout << "\x1B[31mThis file does not exist \x1B[0m" << std::endl;
            std::cout << "\x1B[31m" + filename + " \x1B[0m" << std::endl;
            return "NULLFILE";
        }
        std::string content((std::istreambuf_iterator<char>(filebuffer)),
                            std::istreambuf_iterator<char>());
        return content;
    }

    void home() // this is a GET Endpoint
    {
        std::string indexHTML = readFileAsString("index.html");
        size_t indexHTMLSize = indexHTML.size();
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " +
            std::to_string(indexHTMLSize) + "\r\n"
                                            "\r\n" +
            indexHTML;
        ssize_t bytesSent = send(clientSocket, response.c_str(), strlen(response.c_str()), 0);
        if (bytesSent < 0)
        {
            perror("Error in send");
        }
        std::cout << "This is the home page" << std::endl;
    }

    void serveStaticCSS(std::string filename) // this is a GET Endpoint
    {
        std::string stylesheet = readFileAsString(filename);
        size_t stylesheetSize = stylesheet.size();
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/css\r\n"
            "Content-Length: " +
            std::to_string(stylesheetSize) + "\r\n"
                                             "\r\n" +
            stylesheet;
        ssize_t bytesSent = send(clientSocket, response.c_str(), strlen(response.c_str()), 0);
        if (bytesSent < 0)
        {
            perror("Error in send");
        }
    }

} // namespace Routes
