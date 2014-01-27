#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include "win_serial_api.h"

#pragma pack(1)
struct Sendinfo{
	unsigned char Addr;//장비 아이디
	unsigned char FunctionCode;//펑션코드
	unsigned short StartAddr;//시작주소
	unsigned short DataCount;//저장할데이터수
	unsigned short ByteCount;//저장할 바이트수

	unsigned short DataWhite;
	unsigned short DataRead;
	unsigned short DataGreen;
	unsigned short DataBlue;

	unsigned short CRC;




};



win_serial_api_t *wsa = NULL;

static int log_func( void *wsa, const char *log_str )
{
    printf( "%s", log_str );
}





HANDLE  events[MAXIMUM_WAIT_OBJECTS];
DWORD   active_events;

int Open(char* port_name,int Comspeed){
	  wsa_scan_ports(); // 시스템에 존재하는 시리얼 포트를 스캔한다.
	  // 포트수를 얻은 후 해당 포트명을 표출한다.
char    port_nameScan[256];
int     port_count;
	    port_count = wsa_get_port_count();
	    printf( "system serial port count = %d\n", port_count );
		int     index;
	    for( index = 0; index < port_count; index++ )
	    {
	        memset( port_nameScan, 0, sizeof( port_nameScan ) );
	        wsa_get_port_name( index, port_nameScan );
	        printf( "port index = %d name = [%s]\n", index, port_nameScan );
	    }


	    wsa = wsa_create();                 // 윈도우 시리얼 아탑터 생성
	    wsa_set_log_func( wsa, log_func );  // 콘솔에 출력 처리를 할 함수를 설정한다.
	//    wsa_open( wsa, port_name );         // 포트 열기 ( 마지막 시리얼 포트를 이용 )
	    wsa_open( wsa, port_name );         // 포트 열기 ( 마지막 시리얼 포트를 이용 )
	    wsa_set_write_option( wsa, 1, 0 );  // 쓰기에 대한 조건 설정
	//    wsa_setup( wsa, 115200, 8, 'n' );   // 포트 통신 환경을 설정한다.
	    wsa_setup( wsa, Comspeed, 8, 'n' );   // 포트 통신 환경을 설정한다.
	    wsa_set_read_event_mode ( wsa );    // 읽기 이벤트 모드로 설정한다.

}
int Send(Sendinfo sendData){
    memset( events, 0, sizeof( events ) );
    events[0] = wsa_get_read_event( wsa );

    active_events = WaitForMultipleObjects(
                    1,                              // 이벤트 수
                    (const HANDLE *) &events[0],    // array of objects
                    FALSE,                          // wait for any object
                    1000                            // msec 대기
                );

    if( active_events < 0 ) return -1; // 에러면 프로그램 종료

    printf( "WRITE\n" );
                             // 시리얼에 데이터를 써 넣는다
	 wsa_write( wsa,(void *) &sendData, sizeof(sendData));
	 Sleep(1);

}
int Receive(char * read_buff){
	int     read_size;
	int index=0;
    if( active_events >= 0 )                        // 입력이 있다.
    {
        read_size = wsa_get_rx_data_size( wsa );   // 데이터가 있는가?

        if( read_size > 0  )
        {
            printf( "READ SIZE = %d\n", read_size );
            read_size = wsa_read( wsa, read_buff, read_size );
            for( index = 0; index < read_size ; index++ )
            {
                printf( "%02X .", read_buff[index] );
                // 입력된 문자가 w 이면 출력한다.

            }
        }
    }
    return read_size;
}
int Close(){
	  wsa_close( wsa );   // 포트 닫기
	   wsa_free( wsa );    // 윈도우 시리얼 아탑터 소멸
}


int main( int argc, char **argv )
{





    Sendinfo sendData={0,};
    printf("================size %d",sizeof sendData);
    sendData.Addr=0x01;
    sendData.FunctionCode=0x10;
    sendData.StartAddr=41004;
    sendData.DataCount=4;
    sendData.ByteCount=sendData.DataCount*2;
    sendData.DataWhite=100;
    sendData.DataRead=20;
    sendData.DataGreen=50;
    sendData.DataBlue=80;

	Open("COM4",9600);
	Send(sendData);
	char    read_buff[8192];
	Receive(read_buff);
	Close();

       return 0;
}
