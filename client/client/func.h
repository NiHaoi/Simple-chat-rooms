#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

constexpr auto PORT = 8888;
constexpr auto BUFFER_SIZE = 512;

void receiveMessages(SOCKET socket);
bool WSAinit();
void colorPrint(const char* s, int color);