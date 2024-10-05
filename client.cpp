#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")

constexpr auto PORT = 8888;
constexpr auto BUFFER_SIZE = 512;

void receiveMessages(SOCKET socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytesRead = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << buffer << std::endl;
        }
        else {
            std::cerr << "Server disconnected." << std::endl;
            break;
        }
    }
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;

    // init Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return -1;
    }

    // create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return -1;
    }
    char ipAddress[20];
    std::cout << "the server ip address:";
    std::cin >> ipAddress;
    // set the server's ip address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    //inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    inet_pton(AF_INET, ipAddress, &serverAddr.sin_addr);
    // connect the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to the server. Type your messages:" << std::endl;

    //the thread to recv information
    std::thread receiveThread(receiveMessages, clientSocket);
    receiveThread.detach();

    // the main thread send the
    std::string message;
    while (std::getline(std::cin, message)) {
        send(clientSocket, message.c_str(), message.size(), 0);
    }

    // close the socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
