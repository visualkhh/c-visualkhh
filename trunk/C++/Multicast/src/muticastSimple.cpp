//컴파일러 방법 : gcc -D_REENTRANT multicast.c -o multicast -lpthread
//실행

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 #include <arpa/inet.h>
#include <iostream>
#include <pthread.h>

#define MAXLINE 1024

void* thread_recv(void* arg);

int main(int argc, char *argv[]) {
	int send_s, recv_s;
	int pid;
	unsigned int yes = 1;
	struct sockaddr_in mcast_group;
	struct ip_mreq mreq;
	char line[MAXLINE + 1];
	char message[MAXLINE + 1];
	char name[10];
	int n, len;

	//쓰레드 필요 변수
	int state;
	pthread_t t_id;
	void* t_return;

	if (argc != 4) {
		printf("사용법: %s multicast_address port My_name \n", argv[0]);
		exit(0);
	}

	sprintf(name, "[%s]", argv[3]);

	memset(&mcast_group, 0, sizeof(mcast_group));
	mcast_group.sin_family = AF_INET;
	mcast_group.sin_port = htons(atoi(argv[2]));
	mcast_group.sin_addr.s_addr = inet_addr(argv[1]);

	if ((recv_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error : Can't create receive socket\n");
		exit(0);
	}

	mreq.imr_multiaddr = mcast_group.sin_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(recv_s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq))
			< 0) {
		printf("error: add membership\n");
		exit(0);
	}

	//ywcha, 2010,

	if (setsockopt(recv_s, SOL_SOCKET, SO_REUSEADDR, &mreq, sizeof(yes)) < 0) {
		printf("error: reuse setsockopt\n");
		exit(0);
	}

	if (bind(recv_s, (struct sockaddr*) &mcast_group, sizeof(mcast_group)) < 0) {
		printf("error: bind receive socket\n");
		exit(0);
	}

	if ((send_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error : Can't create send socket\n");
		exit(0);
	}

	//쓰레드 생성
	state = pthread_create(&t_id, NULL, thread_recv, (void*) recv_s);
	if (state != 0) {
		printf("수신용 쓰레드 생성 실패\n");
		exit(0);
	}

	//메세지 전송

	printf("Send Message : ");
	while (fgets(message, MAXLINE, stdin) != NULL) {
		sprintf(line, "%s %s", name, message);
		len = strlen(line);
		if (sendto(send_s, line, strlen(line), 0,
				(struct sockaddr*) &mcast_group, sizeof(mcast_group)) < 1) {
			printf("error: sendto\n");
			exit(0);
		}
	}

}

void* thread_recv(void* arg) {
	int recv_s = (int) arg;
	int len, n;
	struct sockaddr_in from;
	char message[MAXLINE + 1];

	for (;;) {
		printf("receiving message...\n");
		len = sizeof(from);
		if ((n = recvfrom(recv_s, message, MAXLINE, 0,
				(struct sockaddr*) &from, (socklen_t*)&len)) < 0) {
			printf("error : recvfrom\n");
			exit(0);
		}
		message[n] = 0;
		printf("Received Message : %s\n", message);
	}
}

