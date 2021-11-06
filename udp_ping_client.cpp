// UDP Pinger

// Must have this server running before you can run the UDP Pinger Client code

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <chrono>

using namespace std;

#define PORT	 12000


int main() {
	int sockfd, n;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr, cliaddr;


	// Create a UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Fill server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number


	//setting time-outs
	struct timeval t;
	t.tv_sec = 1;
	t.tv_usec = 0;


	int ping_number = 0; // variable that identifies ping number

        //for loop for sending 10 pings
        for(int i = 0; i < 10; i++) {
            ping_number = i + 1;



		auto t_start = std::chrono::high_resolution_clock::now(); //time at which the ping was sent

		//sending ping to server
		sendto(sockfd, (const char *)buffer, strlen(buffer),
		       MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		cout << "ping message #" << ping_number << " has been sent" << endl;

			if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(struct timeval))<0){
			  cout << " timeout error" << endl;
			}

		// recieveing ping from server
		n = recvfrom(sockfd, (char *)buffer, sizeof(buffer),
			0, ( struct sockaddr *) &servaddr, &len);
		buffer[n] = '\0';

		//checking for timeouts
		if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(struct timeval))<0){
		  cout << " timeout error" << endl;
		}

		auto t_end = std::chrono::high_resolution_clock::now(); //time at whoch the ping was recieved back

		// if the number of bytes, n, is less than 0, then the packet was lost
		if(n < 0){
		    cout << "the packet was lost" << endl;
		    cout << "--------------------------" << endl; //separator for readability
		}

		else{
		  double elapsed_time_ns = std::chrono::duration<double, std::nano>(t_end-t_start).count(); //calculate RTT

		    cout << "The RTT for ping message " << ping_number << " is " << elapsed_time_ns <<" nano seconds" << endl;
		    cout << "--------------------------" << endl; //separator for readability
		}


	}
	close(sockfd);
	return 0;
} 
