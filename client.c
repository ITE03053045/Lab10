#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
	int socket_desc;
	struct sockaddr_in server;
	char message[1000], server_reply[2000];
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	puts("Connected\n");
	while (recv(socket_desc, server_reply, 2000, 0))
	{
		puts(server_reply);
		if (server_reply[0] == 'C')
			break;
		bzero(server_reply, 2000);
	}
	bzero(server_reply, 2000);
	while(1)
	{
		bzero(message, 1000);
		scanf("%s", message);
		if (send(socket_desc, message, strlen(message), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		if (recv(socket_desc, server_reply, 2000, 0) < 0)
		{
			puts("recv failed");
		}
		puts(server_reply);
		bzero(server_reply, 2000);
	}
	return 0;
}