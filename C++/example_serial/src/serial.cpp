//============================================================================
// Name        : serial.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>

struct termios tio; // �͹̳� ����ü

int main(int argc, char *argv[]) {
	int fd, result; //fd ���� ��ũ����, result : ���� RW �����
	long baud = B38400; //��żӵ�
	unsigned char buf; // ������ ���� buf -> unsigned�� �Ⱥ��̸� ���� �̻��ϰ� ���´�.
	char sBuffer[7]={0x02, 0x03, 0x49, 0x56, 0x00, 0x03, 0x1C}; // ���� ������ (���� �Ѹ��� ����ϰ� �ִ� ŰƮ�� ���ǵ� �������� ����


	if((fd = open("/dev/ttyS0", O_RDWR|O_NDELAY|O_NOCTTY)) < 0) { // READ, WRITE�� Serial0 ��Ʈ ����
		exit(1);
	}

	////////////////// ���� �ɼ� ����

	tio.c_cflag = baud|CS8|CREAD|CLOCAL; // baud ��� �ӵ�, CS8 (8bit, No Parity, 1 Stop Bit)����
	// CREAD ���� ���Ű����ϰ� ��, CLOCAL, Local Connection ������ ����..
	//ó���� CRTSCTS�� ���� c_cflag�� �����. �׷��� ���������� �۵��� �ȵǼ� ����� �긦 �Ծ���.
	// CRCTSCTS�� �ϵ���� �帧����, ���� ���̺��� ��� ���� ����Ǿ� ���� �� ��� -> ���� ��缱�� �� �Ⱦ��Ƿ�. ^^
	tio.c_cflag &= ~HUPCL;
	tio.c_lflag = 0;  // Local Mode ����
	tio.c_iflag = IGNPAR; // Parity ������ �ִ� ���� ����
	tio.c_oflag = 0; // ���ó�� ���� 0�̸� �ƹ��͵� ����

	///////////////// �ɼ� ���� �� (���� �߰����� �ɼǵ� ���� ������ ã�ƺ��� �ٶ���.)

	tcflush(fd, TCIFLUSH); // ������ �ʱ�ȭ
	tcsetattr(fd, TCSANOW, &tio); // tio �͹̳� ����ü�� �������� ������ �����Ų��.

	fcntl(fd, F_SETFL, FNDELAY); // �����ִ� ���� ��� ���� ���

	result = write(fd, sBuffer, 7); // ���������� �ø���� �����͸� ������. sBuffer���� 7���� �ø���0���� ������.

	if(result < 0) { // ������?
		printf("write error\n");
		close(fd);
		exit(1);
	}

	usleep(1000); // ����

	while(1) { // ������ �޴� �κ�.. ���°� ��� ����Ѵ�. �ֱ���â.. ^^
		if((result = read(fd, &buf, 1)) > 0) {
			printf("%02x ", buf);
			fflush(stdout); // �ѷ���3.
		}

	}

	close(fd); // ������ �ݰ�
}
