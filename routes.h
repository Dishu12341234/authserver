#ifndef ROUTES_H
#define ROUTES_H

#include <string>
namespace Routes
{
    extern int clientSocket;
    void home();
    void serveStaticCSS(std::string filename);
    std::string readFileAsString(std::string);
} // namespace Routes


#endif