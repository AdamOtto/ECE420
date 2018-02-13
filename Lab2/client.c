#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <iostream>
#include "timer.h"

using std::cout;
using std::endl;
using std::cin;

const char* address = "127.0.0.1";
unsigned int port;
unsigned int* seed;
int TheArray_size;
const int NumberOfCalls = 100;
char r = 'r';
char w = 'w';

int main(int argc, char** argv)
{
	if (argc != 3) {
		cout << "Error: Invalid number of Arguments: " << argc << endl;
		exit(0);
	}

	struct sockaddr_in sock_var;
	int clientFileDescriptor[NumberOfCalls]; // = socket(AF_INET,SOCK_STREAM,0);
	char str_clnt[20],str_ser[20];
	double start, finish;

	TheArray_size = atoi(argv[2]);
	port = atoi(argv[1]);

	/* Intializes random number generators - Obtained through lab examples */
	int i = 0;
	seed = (unsigned int *)malloc(NumberOfCalls*sizeof(int));
	for (i = 0; i < NumberOfCalls; i++)
	{
		seed[i] = i;
		clientFileDescriptor[i] = socket(AF_INET,SOCK_STREAM,0);
	}

	sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_var.sin_port= atoi(argv[1]);
	sock_var.sin_family=AF_INET;

	// BEGIN TIMED Analysis
	GET_TIME(start)
	for (i = 0; i < NumberOfCalls; i++)
	{
		if(connect(clientFileDescriptor[i],(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
		{
			//cout << "Connected to Server" << clientFileDescriptor << endl;
			//cout << "nEnter Srting to send: ";
			//cin >> str_clnt;
			cout << "Starting client " << i << "\n";	

			int pos = rand_r(&seed[i]) % TheArray_size;
			int randNum = rand_r(&seed[i]) % 20;	// write with 10% probability
			cout << "pos: " << pos << ", randNum: " << randNum << "\n";

			//First Send which kind of operation you want to perform: 'r' for read, 'w' for write.
			if (randNum < 19) {// 5% are write operations, others are reads
				cout << "send read request to " << pos << "\n";
				write(clientFileDescriptor[i],&r,1);
			}
			else {
				cout << "send write request to " << pos << "\n";
				write(clientFileDescriptor[i],&w,1);
			}
			read(clientFileDescriptor[i],str_ser,20);
			//cout <<	"Got first Ack\n";

			//Second send which array index in TheArray you want to access
			char numberstring[20] = {0};
			sprintf(numberstring, "%d", pos);
			write(clientFileDescriptor[i],&numberstring,20);
			read(clientFileDescriptor[i],str_ser,20);
			//cout <<	"Got second Ack\n";

			//Receive your response from the server
			sprintf(str_clnt, "String %d modified", pos);
			write(clientFileDescriptor[i],str_clnt,20);
			read(clientFileDescriptor[i],str_ser,20);
			cout << "Response from Server: " << str_ser << endl;
			cout << "\n\n";
			close(clientFileDescriptor[i]);
			
		}
		else {
			cout << "Failed to create socket for connection " << endl;
		}
		
	}
	GET_TIME(finish)
	// END TIMED Analysis
	cout << "Execution time: " << finish-start << endl;
	return 0;
}
