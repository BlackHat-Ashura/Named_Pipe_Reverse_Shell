


bool RunCMD(CHAR* cmd, CHAR* result) {
	HANDLE PipeWrite;
	HANDLE PipeRead;

	if (!::CreatePipe(&PipeRead, &PipeWrite, NULL, 0)) {
		printf("[-] Failed to create CMD Pipes.\n");
		return 1;
	}

	::SetHandleInformation(PipeWrite, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

	PROCESS_INFORMATION pi;
	STARTUPINFOA si = { sizeof(si) };
	si.hStdOutput = PipeWrite;
	si.hStdError = PipeWrite;
	si.dwFlags = STARTF_USESTDHANDLES;

	CHAR name[200] = "cmd.exe /c ";
	::strcat_s(name, "\"");
	::strcat_s(name, cmd);
	::strcat_s(name, "\"");
	if (!::CreateProcessA(NULL, name, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		printf("[-] Failed to create CMD Process.\n");
		return 1;
	}
	::CloseHandle(pi.hThread);
	::CloseHandle(PipeWrite);

	::WaitForSingleObject(pi.hProcess, 15000);

	DWORD recv = DATA_BUFFER_SIZE;
	if (!::ReadFile(PipeRead, result, DATA_BUFFER_SIZE, &recv, NULL) || recv == 0) {
		printf("[-] Unable to read CMD Pipe or no data.\n");
		return 1;
	}
	::CloseHandle(pi.hProcess);
	::CloseHandle(PipeRead);
	return 0;
}