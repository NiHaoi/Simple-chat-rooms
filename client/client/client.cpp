#include "func.hpp"

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSAinit();
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;

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

    colorPrint("Connected to the server. Type your messages:\n",2);

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
