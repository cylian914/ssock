// Description: Client for UNIX domain socket
// use gcc ssock.c -o ssock to compile
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
	if (argc < 2)
		return -1;
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	int resv;
	struct sockaddr_un serverAddress = {0};
	
	serverAddress.sun_family  = AF_UNIX;
	strcpy(serverAddress.sun_path, argv[1]);
	printf("Connecting to %s\n", argv[1]);

	resv=connect(sock, (struct sockaddr*)&serverAddress, SUN_LEN(&serverAddress));
	if (resv < 0)
		return resv;
	printf("Connected\n");
	//  concat argv
	char mess[1024];
	memcpy(mess,argv[2],strlen(argv[2])+1);
	for(int i=3; i<argc; i++){
		strcat(mess," ");
		strcat(mess,argv[i]);
	}
	printf("Message: %s\n", mess);
	strcat(mess,"\0");
	write(sock, mess, strlen(mess)+1);
	//reply
	char reply[1024];
	read(sock, reply, 1024);
	printf("Reply: %s\n", reply);
	close(sock);
	return 0;
}
