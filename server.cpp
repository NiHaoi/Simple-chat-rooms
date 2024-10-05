// chat_server.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

constexpr auto PORT = 8888;
constexpr auto BUFFER_SIZE = 512;

void broadcastMessage(const std::vector<SOCKET>& clients, const std::string& message, SOCKET sender) {
    for (const auto& client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return -1;
    }

    // 创建服务器 socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return -1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // listen
    listen(serverSocket, SOMAXCONN);
    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    std::vector<SOCKET> clients;

    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        SOCKET maxSocket = serverSocket;

        // add the client to the set
        for (const auto& client : clients) {
            FD_SET(client, &readfds);
            maxSocket = (((maxSocket) > (client)) ? (maxSocket) : (client));
        }

        int activity = select(maxSocket + 1, &readfds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            std::cerr << "Select error." << std::endl;
            break;
        }

        // new client
        if (FD_ISSET(serverSocket, &readfds)) {
            newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
            if (newSocket != INVALID_SOCKET) {
                clients.push_back(newSocket);
                std::cout << "New client connected." << std::endl;
            }
        }

        // check
        for (size_t i = 0; i < clients.size(); ++i) {
            if (FD_ISSET(clients[i], &readfds)) {
                int bytesRead = recv(clients[i], buffer, BUFFER_SIZE, 0);
                if (bytesRead <= 0) {
                    //the client disconnected
                    closesocket(clients[i]);
                    clients.erase(clients.begin() + i);
                    --i;
                    std::cout << "Client disconnected." << std::endl;
                }
                else {
                    buffer[bytesRead] = '\0';
                    std::string message = "Client " + std::to_string(i) + ": " + buffer;
                    std::cout << message << "\n";
                    broadcastMessage(clients, message, clients[i]);
                }
            }
        }
    }

    for (const auto& client : clients) {
        closesocket(client);
    }
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
