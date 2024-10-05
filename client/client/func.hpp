#include "func.h"
void receiveMessages(SOCKET socket) {
    char buffer[BUFFER_SIZE];
    while (true) {

        int bytesRead = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytesRead > 0) {
            buffer[bytesRead-1] = '\n';
            buffer[bytesRead] = '\0';
            colorPrint(buffer,3);
        }
        else {
            std::cerr << "Server disconnected." << std::endl;
            break;
        }
    }
}
bool WSAinit() {
    WSADATA wsaData;
    // init Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return false;
    }
    return true;
}
void colorPrint(const char* s, int color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    printf(s);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
