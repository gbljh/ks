#include "multi_thread_server.h"

THREAD_VOID handle_connection(void* psocket) {
	CHECK_VOID_IO(psocket, "Empty connection thread data\n");
	SOCKET socket;
	CHECK_VOID_IO((socket = *((SOCKET*)psocket)) > 0, "Invalid connection thread data\n");
	sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	CHECK_VOID_IO(!getpeername(socket, (sockaddr*)&addr, &addr_len), "Error retrieving peer info\n");
	char* str_in_addr = inet_ntoa(addr.sin_addr);
	printf("[%s:%d]>>%s\n", str_in_addr, ntohs(addr.sin_port), "Establish new connection");
	while (true) {
		char buffer[256] = "";
		int rc = recv(socket, buffer, sizeof(buffer), 0);
		if (rc > 0) {
			printf("[%s]:%s\n", str_in_addr, buffer);
			
		}
		else {
			break;
		}
	}
	close_socket(socket);
	printf("[%s]>>%s", str_in_addr, "Close incomming connection");
}