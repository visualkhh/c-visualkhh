//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "APP_UTILS.h"
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// ���� : CRC�� ��
// ���� : ���������, �����ͱ���
// ��ȯ : short �� CRC
//---------------------------------------------------------------------------
unsigned short int  makeCRC(unsigned char *puchMsg,  unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;
	unsigned uIndex ;

	while (usDataLen--)
    {
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);

}

void ntoh( char *src, char *des, int size)
{
	for ( int i=0; i < size ; i++)
    {
        des[i] =  src[size - 1 - i];
	}
}






















