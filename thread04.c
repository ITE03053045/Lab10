#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
void *connection_handler(void *);

int main(int argc , char *argv[])
{
	int socket_desc, new_socket, c, *new_sock;
	struct sockaddr_in server, client;
	char *message;
	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket.");
	}
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	//Bind
	if(bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("bind failed.");
		return 1;
	}
	puts("bind done");
	//Listen
	listen(socket_desc, 3);
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		puts("Connection accepted.");
		//Reply to the client
		message = "Hello Client, I have received your connection. And now I will assign a handler for you.\n";
		write(new_socket, message, strlen(message));
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = new_socket;
		if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0)
		{
			perror("could not create thread.");
			return 1;
		}
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned.");
	}
	if (new_socket<0)
	{
		perror("accept failed.");
		return 1;
	}
	return 0;
}
/*
* This will handle connection for each client
* */
void *connection_handler(void *socket_desc)
{
	int new_socket, read_size;
	struct sockaddr_in server , client;
	int sock = *(int*)socket_desc;
	char *message , client_message[2000] , str[4];
	int i , p , Ans[10] , tmp;
	int guessing[4] , num , countA = 0, countB = 0 , j , k;
for ( i = 0 ; i < 10 ; ++i )
	Ans[i] = i;
srand(time(NULL)) ;
for ( i = 0 ; i < 4 ; ++i )
{
	p = rand() % 10 ;
	if ( p == i )
		continue ;
	tmp = Ans[i] ;
	Ans[i] = Ans[p] ;
	Ans[p] = tmp ;
}
printf("%d%d%d%d\n",Ans[3],Ans[2],Ans[1],Ans[0]);

//Reply to the client
while(1)
{
	bzero(str,4);
	read(new_socket,str,4);
	num = (str[0]-'0')*1000+(str[1]-'0')*100+(str[2]-'0')*10+(str[3]-'0')*1;
	guessing[3] = num / 1000 ;
	guessing[2] = ( num / 100 ) % 10 ;
	guessing[1] = ( num / 10 ) % 10 ;
	guessing[0] = num % 10 ;

	for( j = 0 ; j < 4 ; j++ )
		for( k = 0 ; k < 4 ; k++ )
			if( Ans[j] == guessing[k] )
				if( j == k )
					countA++;
				else
					countB++;
	if( countA == 4 )
	{
		message="Correct!!\n";
		write(new_socket,message,strlen(message));
		return 1;
	}
	else
	{
		sprintf(client_message,"Hint: %dA%dB\n",countA,countB);
		write(new_socket,client_message,strlen(client_message));
		countA = 0;
		countB = 0;
	}
}
if (read_size == 0)
	{
		puts("Client disconnected.");
		fflush(stdout);
	}
	else if (read_size == -1)
	{
		perror("recv failed.");
	}
	free(socket_desc);
	return 0;
}