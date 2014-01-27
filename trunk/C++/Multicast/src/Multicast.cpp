//#include <stdio.h>
//#include <string.h>
//#include <netinet/in.h>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <signal.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
// #include <arpa/inet.h>
//#include <iostream>
//#define GROUP "239.0.0.1"
//#define PORT 20001
//
//#define TAILLE 100
//
//int main() {
//	int sockfd;
//	struct sockaddr_in servaddr;
//	struct ip_mreq imr;
//	int len_serv;
//	char message[TAILLE];
//
//	memset(message, 0, TAILLE);
//	sprintf(message, "Coucou");
//
//
//	len_serv = sizeof(servaddr);
//
//	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//	if (sockfd < 0) {
//		perror("Problemes lors de l'ouverture de la socket ");
//		return -1;
//	}
//
//	memset(&servaddr, 0, len_serv);
//	servaddr.sin_family = AF_INET;
//	servaddr.sin_port = htons(PORT);
//	servaddr.sin_addr.s_addr = INADDR_ANY;
//
//	if (bind(sockfd, (struct sockaddr *) &servaddr, len_serv) < 0) {
//		perror("Problemes lors de l'association de la socket a un port ");
//		return -1;
//	}
//
//	inet_aton(GROUP, &(imr.imr_multiaddr));
//	imr.imr_interface.s_addr = INADDR_ANY;
//
//	if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(imr))
//			< 0) {
//		perror("Problemes lors de la demande de JOIN ");
//		return -1;
//	}
//
//
//	while(true){
//		memset(message, 0, TAILLE);
//
//		if (recvfrom(sockfd, message, TAILLE, 0,
//				(struct sockaddr *) &servaddr, (socklen_t*)&len_serv) < 0) {
//			perror("Problemes lors de la lecture d'une donnee ");
//			return -1;
//		}
//		printf("%s\n", message);
//		sleep(1);
//	}
////	while(true){
////		if (sendto(sockfd, message, strlen(message), 0,
////				(struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
////			perror("Problemes lors de l'envoi du message ");
//////			return -1;
////		}
////
////		printf("%s\n", message);
////		sleep(1);
////	}
//
//	printf("%s\n", message);
//	close(sockfd);
//
//}
