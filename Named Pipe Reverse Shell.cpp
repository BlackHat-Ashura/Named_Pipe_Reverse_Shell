#include <stdio.h>
#include <Windows.h>


#define DATA_BUFFER_SIZE 4096


#include "server.hpp"
#include "client.hpp"


void ShowHelp(char *argv[]) {
	printf("[-] Usage : %s <server/client> <Named Pipe Name>\n\n", argv[0]);
	printf("    Example : %s server \\\\.\\pipe\\RevShell\n", argv[0]);
	printf("    Example : %s client \\\\127.0.0.1\\pipe\\RevShell\n", argv[0]);
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		ShowHelp(argv);
		return 1;
	}
	
	CHAR NamedPipeName[100];
	if (strlen(argv[2]) > 100) {
		printf("[-] Full pipe path length should be less than 100 characters.\n");
		return 1;
	}
	::strcpy_s(NamedPipeName, argv[2]);

	if (!strcmp("server", argv[1])) {
		if (Server(NamedPipeName) == 1) {
			return 1;
		}
	}
	else if (!strcmp("client", argv[1])) {
		if (Client(NamedPipeName) == 1) {
			return 1;
		}
	}
	else {
		ShowHelp(argv);
		return 1;
	}

	return 0;
}