#include "crypto.h"
#include "network.h"
#include <iostream>
#include <atomic>
#include <csignal>
#include <thread>
#include <chrono>



std::atomic<bool> running{true};

void exitonsig(int n)
{
    std::cout << "\nInterrupt signal (" << n << ") received. Shutting down server..." << std::endl;
    shutdown(Server::serverSocket, SHUT_RDWR);
    Server::serverClose();
    running = false;
}

void setupRoutes()
{
    Server::GET_ROUTES.push_back(std::make_pair("/", Routes::home));
    Server::GET_ROUTES.push_back(std::make_pair("/index", Routes::home));
    Server::GET_ROUTES.push_back(std::make_pair("/home", Routes::home));
}

int main(int argc, char const *argv[])
{
    // Register signal handler
    std::signal(SIGINT, exitonsig);

    // Configure port
    int port = (argc > 1) ? std::stoi(argv[1]) : 80;

    // Initialize the server
    if (!Server::initilizeServer(port)) {
        std::cerr << "Failed to initialize server on port " << port << "." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Server started on port " << port << ". Press Ctrl+C to stop." << std::endl;

    setupRoutes();
    // Main loop to handle clients
    while (running)
    {
        Server::serverHandleClients();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Close the server
    Server::serverClose();
    std::cout << "Server shut down successfully." << std::endl;

    return 0;
}
