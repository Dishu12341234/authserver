#ifndef ROUTES_H
#define ROUTES_H

#include <string>
namespace Routes
{
    extern int clientSocket;
    void serveStaticCSS(std::string filename);

    std::string readFileAsString(std::string);
    void parsedHTML(std::string&,std::vector<std::pair<std::string,std::string>>);
    std::string getValue(std::vector<std::pair<std::string,std::string>>,std::string);

    //GET routes
    void home(std::vector<std::pair<std::string,std::string>>);
    //POST routes
    void selfauth(std::vector<std::pair<std::string,std::string>> keyvaluePairs);
} // namespace Routes


#endif