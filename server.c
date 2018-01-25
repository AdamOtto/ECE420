#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include <iostream>     // For std::cout, std::endl
#include <string>       // For std::string
#include <fstream>      // For std::ifstream, std::ofstream
#include <sstream>      // For std::istringstream
#include <vector>       // For std::vector
#include <iterator>     // For std::istream_iterator

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

//Default port # is 3000
int NumberOfStrings;
char** library;
pthread_mutex_t* mutex;

void *ServerEcho(void *args)
{
	long clientFileDescriptor=(long)args;
	char * str = new char[20];
	bool Read;
	//Get whether the user wants to read or write.
	read(clientFileDescriptor,str,20);
	if (str[0] == 'r')
	{
		Read = true;
	}
	else if (str[0] == 'w')
	{
		Read = false;
	}
	//Get Which spot in the library the client wants to read/write from
	read(clientFileDescriptor,str,20);
	int index = atoi(str);
	if(index >= NumberOfStrings)
	{
		write(clientFileDescriptor, "Index out of bounds.",20);
		close(clientFileDescriptor);
		return NULL;
	}	
	
	//Read or Write opreation.
	pthread_mutex_lock(&mutex[index]);
	if (Read)
	{
		std::stringstream s;
		s << library[index];
		str = (char*)s.str().c_str();		
	}
	else
	{
		//Get the message of the client and set it to library.
		read(clientFileDescriptor,str,20);
		printf("nreading from client: %s\n",str);
		library[index] = str;
		//Re-package str
		std::stringstream s;
		s << library[index];
		str = (char*)s.str().c_str();
	}
	pthread_mutex_unlock(&mutex[index]);

	

	//Echo back to the client
	write(clientFileDescriptor,str,20);
	printf("nechoing back to client");
	close(clientFileDescriptor);
}


int main(int argc, char** argv)
{
	if (argc != 3) {
		cout << "Error: Invalid number of Arguments: " << argc << endl;
		exit(0);
	}	
	NumberOfStrings = atoi(argv[2]);
	int i;

	//Initialize the library of strings	
	library = new char *[NumberOfStrings];
	
	for(i = 0; i < NumberOfStrings; i++)
	{
		std::stringstream s;
		s << "String " << i << ": the initial value";
		library[i] = (char*)s.str().c_str();
	}	

	//Initialize the mutex(s)
	mutex = new pthread_mutex_t [NumberOfStrings];
	for (int i = 0; i < NumberOfStrings; i++)
        	pthread_mutex_init(&mutex[i], NULL); 
	
	struct sockaddr_in sock_var;
	int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
	int clientFileDescriptor;
	pthread_t t[20];

	sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_var.sin_port= atoi(argv[1]);
	sock_var.sin_family=AF_INET;
	if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
	{
		printf("nsocket has been created");
		listen(serverFileDescriptor,2000); 
		while(1)        //loop infinity
		{
			for(i=0;i<20;i++)      //can support 20 clients at a time
			{
				clientFileDescriptor=accept(serverFileDescriptor,NULL,NULL);
				printf("nConnected to client %dn",clientFileDescriptor);
				pthread_create(&t[i],NULL,ServerEcho, (void *)clientFileDescriptor);
			}
		}
		close(serverFileDescriptor);
	}
	else{
		printf("nsocket creation failed");
	}
	return 0;
}
