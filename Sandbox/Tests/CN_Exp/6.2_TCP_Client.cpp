#include "Entrypoint.h"
#if CHECK_ACTIVE(6, 2, 1)

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // ���� Winsock ��

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024] = { 0 };

    // ��ʼ�� Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }

    // �����׽���
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return 1;
    }

    // ���÷�������ַ
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5005);
    inet_pton(AF_INET, "10.249.109.56", &serverAddr.sin_addr);

    // ���ӵ�������
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // �������ݵ�������
    const char* message = "Hello from client!";
    send(clientSocket, message, strlen(message), 0);

    // ���շ���������Ӧ
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        std::cout << "Received from server: " << buffer << std::endl;
    }

    // �ر��׽���
    closesocket(clientSocket);

    // ���� Winsock
    WSACleanup();
    return 0;
}


#endif