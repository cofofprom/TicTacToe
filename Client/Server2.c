#include "../clientNetworking/packet.h"
#include "../clientNetworking/packetEnums.h"
#include "../gameboard/gameboard.h"
#include "../userdata/userdata.h"
#include "serverNetworkWorker.h"

int CreateServer()
{
    SOCKET server, client;
    struct sockaddr_in localaddr, clientaddr;
    int size;
    server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (server == INVALID_SOCKET)
    {
        printf("Error create server\n");
        return 1;
    }
    localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(5510);//port number is for example, must be more than 1024
    if (bind(server, (struct sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
    {
        printf("Can't start server\n");
        return 2;
    }
    else
    {
        printf("Server is started\n");
    }
    listen(server, 50);//50 клиентов в очереди могут стоять

    while (1)
    {
        size = sizeof(clientaddr);
        client = accept(server, (struct sockaddr*)&clientaddr, &size);

        if (client == INVALID_SOCKET)
        {
            printf("Error accept client\n");
            continue;
        }
        else
        {
            printf("Client is accepted\n");
        }
        pthread_t mythread;
        int status = pthread_create(&mythread, NULL, ClientStart, (void*)client);
        pthread_detach(mythread);
    }

    printf("Server is stopped\n");
    closesocket(server);
    return 0;
}

int main()
{
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
    {
        printf("Connected to socket lib\n");
    }
    else
    {
        return 1;
    }
    return CreateServer();
}