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

    void parsedHTML(std::string &HTML, std::vector<std::pair<std::string, std::string>> keyValues)
    {
        for (size_t i = 0; i < HTML.size(); i++)
        {
            if (HTML.at(i) == '{')
            {
                std::string key;
                key = HTML.substr(i + 1, HTML.substr(i).find("}") - 1);
                std::cout << "Key: " << key << std::endl;
                HTML.replace(i, key.size() + 2, getValue(keyValues, key));
            }
        }
    }

    std::string getValue(std::vector<std::pair<std::string, std::string>> keyValuePair, std::string key)
    {
        for (auto &&pair : keyValuePair)
        {
            if (pair.first == key)
            {
                return pair.second;
            }
        }
        return std::string("NULL");
    }

    void home(std::vector<std::pair<std::string, std::string>> URLParams) // this is a GET Endpoint
    {
        std::string indexHTML = readFileAsString("index.html");
        size_t indexHTMLSize = indexHTML.size();
        std::cout << "The size of URL params: " << URLParams.size() << std::endl;
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

    void selfauth(std::vector<std::pair<std::string, std::string>> keyvaluePairs)
    {
        std::string authHTML = readFileAsString("auth.html");
        parsedHTML(authHTML, std::vector<std::pair<std::string, std::string>>({
                                 {"uname", getValue(keyvaluePairs, "uname")},
                                 {"pswrd", getValue(keyvaluePairs, "pswrd")},
                                 {"email", getValue(keyvaluePairs, "email")},
                             }));
        size_t authHTMLSize = authHTML.size();
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " +
            std::to_string(authHTMLSize) + "\r\n"
                                           "\r\n" +
            authHTML;
        std::cout << std::endl;
        ssize_t bytesSent = send(clientSocket, response.c_str(), strlen(response.c_str()), 0);
        if (bytesSent < 0)
        {
            perror("Error in send");
        }
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
        if (stylesheet == "NULLFILE")
        {
            response =
                "HTTP/1.1 404  Not Found\r\n"
                "Content-Type: text/css\r\n"
                "Content-Length: 0"
                "\r\n"
                "\r\n";
        }
        ssize_t bytesSent = send(clientSocket, response.c_str(), strlen(response.c_str()), 0);
        if (bytesSent < 0)
        {
            perror("Error in send");
        }
    }

} // namespace Routes
