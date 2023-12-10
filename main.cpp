#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

void handle_client(int client_socket) {
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Arnav Says Hi<3\r\n";

    write(client_socket, response, strlen(response));
    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(3000);

    if (bind(server_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == -1) {
        std::cerr << "Listen failed" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Server listening on port 3000..." << std::endl;

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == -1) {
            std::cerr << "Accept failed" << std::endl;
            close(server_socket);
            return 1;
        }

        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}

