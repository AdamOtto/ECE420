#include<stdlib.h>
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
const unsigned int port = 3000;

int main()
{
	struct sockaddr_in sock_var;
	int clientFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
	char str_clnt[20],str_ser[20];
	double start, finish;

	sock_var.sin_addr.s_addr=inet_addr(address);
	sock_var.sin_port=port;
	sock_var.sin_family=AF_INET;

	// BEGIN TIMED Analysis
	GET_TIME(start)
	if(connect(clientFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0) {
		cout << "Connected to Server" << clientFileDescriptor << endl;
		cout << "nEnter Srting to send: ";
		cin >> str_clnt;
		write(clientFileDescriptor,str_clnt,20);
		read(clientFileDescriptor,str_ser,20);
		cout << "Response from Server: " << str_ser << endl;
		close(clientFileDescriptor);
	}
	else {
		cout << "Failed to create socket for connection " << endl;
	}
	GET_TIME(finish)
	// END TIMED Analysis
	cout << "Execution time: " << finish-start << endl;
	return 0;
}
