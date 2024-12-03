
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	int dummyfd;
	struct message req;
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,terminate);
	server = open("serverFIFO",O_RDONLY);
	dummyfd = open("serverFIFO",O_WRONLY);

	while (1) {
		// TODO:
		// read requests from serverFIFO
		// if (read(server, &req, sizeof(req)) > 0) {
		// 	printf("Received a request from %s to send the message '%s' to %s.\n", 
		// 	       req.source, req.msg, req.target);

		// 	// TODO:
		// 	// open target FIFO and write the whole message struct to the target FIFO
		// 	// Construct the target FIFO name
		// 	char targetFIFO[100];
		// 	snprintf(targetFIFO, sizeof(targetFIFO), "%sFIFO", req.target);

		// 	// Open target FIFO for writing
		// 	target = open(targetFIFO, O_WRONLY);
		// 	if (target == -1) {
		// 		perror("Error opening target FIFO");
		// 		continue; // Skip to the next message
		// 	}

		// 	// Write the message to the target FIFO
		// 	if (write(target, &req, sizeof(req)) == -1) {
		// 		perror("Error writing to target FIFO");
		// 	}

		// 	// TODO:
		// 	// close target FIFO after writing the message
		// 	close(target);
		// } else {
		// 	perror("Error reading from serverFIFO");
		// 	break; 
		// }




		// Read requests from serverFIFO
        ssize_t bytesRead = read(server, &req, sizeof(struct message));
        if (bytesRead <= 0) {
            continue; // No data or error
        }

        printf("Received a request from %s to send the message '%s' to %s.\n",
               req.source, req.msg, req.target);

        // Open target FIFO and write the message
        target = open(req.target, O_WRONLY);
        if (target < 0) {
            perror("Failed to open target FIFO");
            continue;
        }

        if (write(target, &req, sizeof(struct message)) < 0) {
            perror("Failed to write to target FIFO");
        }

        close(target);
	}
	close(server);
	close(dummyfd);
	return 0;
}

