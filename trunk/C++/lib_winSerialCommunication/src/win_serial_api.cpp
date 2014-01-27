//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include "win_serial_api.h"

#define     MAX_SERIAL_MANAGE_COUNT         64

static int      serial_count = 0;
static char     serial_port_name  [MAX_SERIAL_MANAGE_COUNT][256];

// 시리얼 포트를 스캐닝 한다.
void wsa_scan_ports( void )
{
    HKEY    hKey;

    char    serial_key_name[256];
    DWORD   key_name_size;
    DWORD   port_name_size;

    DWORD   type_code;

    LONG    ret;

    memset( serial_key_name,    0, sizeof( serial_key_name ) );
    memset( serial_port_name,   0, sizeof( serial_port_name )   );
    serial_count = 0;

    ret = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                        "HARDWARE\\DEVICEMAP\\SERIALCOMM",
                        0,
                        KEY_READ,
                        &hKey
                      );

    if( ret == ERROR_SUCCESS )
    {
        serial_count = 0;

        while(1)
        {
            key_name_size  = 256;
            port_name_size = 256;

            ret = RegEnumValue( hKey,
                                (DWORD) serial_count,
                                &(serial_key_name[0]), &key_name_size,
                                NULL,
                                &type_code,
                                (BYTE *) &(serial_port_name[serial_count][0]), &port_name_size
                              );
            if(  ret != ERROR_SUCCESS ) break;

//            if( type_code == REG_SZ)
//              printf( "name = [%s:%s]\n", serial_key_name, serial_port_name[serial_count] );

            serial_count++;

        }
        RegCloseKey(hKey);
    }

}

// 시리얼 포트 수를 얻는다.
int wsa_get_port_count  ( void )
{
    return serial_count;
}

// 시리얼 포트 명을 얻는다.
int wsa_get_port_name( int index, char *name )
{
    if( index <     0               ) return -1;
    if( index >=    serial_count    ) return -1;

    strcpy( name , serial_port_name[index] );
    return 0;
}

// 관리자 생성
win_serial_api_t *  wsa_create  ( void )
{
    win_serial_api_t *wsa;

    wsa = ( win_serial_api_t * ) malloc( sizeof(win_serial_api_t) );
    if( wsa == NULL ) return NULL;

    memset( wsa, 0, sizeof( wsa ) );

//    wsa->com_debug           =   1;
    wsa->com_handle          =   0;

    wsa->w_wait              =   0;
    wsa->w_timeout_msec      =   100;

    wsa->tx_packet_max_size  = 8192;
    wsa->tx_data             = (unsigned char *) malloc( wsa->tx_packet_max_size );
                               memset( wsa->tx_data, 0, wsa->tx_packet_max_size );

    wsa->tx_size             = 0;

    wsa->r_wait              = 0;
    wsa->r_timeout_msec      = 100;

    wsa->w_event.Offset      = 0 ;
    wsa->w_event.OffsetHigh  = 0 ;
    wsa->r_event.Offset      = 0 ;
    wsa->r_event.OffsetHigh  = 0 ;

    wsa->r_event.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ;
    wsa->w_event.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ;

    wsa->log_func = NULL;

    return wsa;
}

//---------------------------------------------------------------------------
// 설명 : 관리자 소멸
//---------------------------------------------------------------------------
void wsa_free    ( win_serial_api_t * wsa )
{
    if( wsa == NULL ) return;

    if( wsa->tx_data != NULL ) free( wsa->tx_data );

    CloseHandle( wsa->r_event.hEvent ) ;
    CloseHandle( wsa->w_event.hEvent ) ;

    wsa_close( wsa );

    wsa_log_write( wsa, "free call" );

    free( wsa );
}


//---------------------------------------------------------------------------
// 읽기 이벤트를 설정한다.
//---------------------------------------------------------------------------
int wsa_set_read_event_mode( win_serial_api_t * wsa )
{
    DWORD   active_events;

    SetCommMask( wsa->com_handle, EV_RXCHAR );
    active_events = WaitCommEvent( wsa->com_handle, &active_events, &(wsa->r_event) );

    return  active_events;
}

//---------------------------------------------------------------------------
// 읽기 이벤트 핸들을 얻는다.
//---------------------------------------------------------------------------
HANDLE  wsa_get_read_event( win_serial_api_t * wsa )
{
    return wsa->r_event.hEvent;
}

//---------------------------------------------------------------------------
// 쓰기 이벤트 핸들을 얻는다.
//---------------------------------------------------------------------------
HANDLE  wsa_get_write_event( win_serial_api_t * wsa )
{
    return wsa->w_event.hEvent;
}

//---------------------------------------------------------------------------
// 설명 : 로그 함수를 설정한다.
//---------------------------------------------------------------------------
void wsa_set_log_func( win_serial_api_t * wsa, int (*log_func)( void *wsa, const char *log_str ) )
{
    wsa->log_func = log_func;
}

//---------------------------------------------------------------------------
// 설명 : 로그 기록 처리
//---------------------------------------------------------------------------
int  wsa_log_write( win_serial_api_t * wsa, const char *fmt, ... )
{
    char    buff[2048];
    va_list ap;

    if( !wsa->com_debug ) return 0;

    if( wsa->log_func != NULL )
    {
        va_start(ap, fmt);
        sprintf( buff, fmt, ap);
        va_end(ap);
        return wsa->log_func( wsa, buff );
    }

    return 0;
}
//---------------------------------------------------------------------------
// 설명 : 포트 열기
// 인자 : port_filename : 통신 포트명
//---------------------------------------------------------------------------
HANDLE wsa_open( win_serial_api_t * wsa, char *port_filename )
{
    char win32_portname[128];

    if( wsa == NULL ) return 0;

    wsa_log_write( wsa, "open call" );

    if( wsa->com_handle ) CloseHandle( wsa->com_handle ) ;

    sprintf( win32_portname, "%c%c.%c%s", 0x5C, 0x5C, 0x5C, port_filename );

    wsa->com_handle = CreateFile(   win32_portname,
                                    GENERIC_READ | GENERIC_WRITE,
                                    0,                                  // exclusive access
                                    NULL,                               // no security attrs
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O
                                    NULL
                                );
    if( wsa->com_handle )
    {
        SetupComm( wsa->com_handle, 8192, 8192 );
        PurgeComm( wsa->com_handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
        wsa_clear_error( wsa );
    }

    return wsa->com_handle;
}

//---------------------------------------------------------------------------
// 설명 : 포트 닫기
//---------------------------------------------------------------------------
void wsa_close( win_serial_api_t * wsa )
{
    if( wsa == NULL ) return;

    wsa_log_write( wsa, "close call" );

    if( wsa->com_handle != 0 )
    {
        PurgeComm( wsa->com_handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
        CloseHandle( wsa->com_handle ) ;
        wsa->com_handle = 0;
    }
}

//---------------------------------------------------------------------------
// 설명 : 환경 설정
// 반환 : 0 보다 작다. 설정 실패
//        0 성공
//---------------------------------------------------------------------------
int wsa_setup   ( win_serial_api_t * wsa, int baud, int data, char parity )
{
    DCB     dcb ;

    if( wsa == NULL )           return -1;
    if( wsa->com_handle == 0 )  return -1;

    dcb.DCBlength = sizeof( DCB ) ;

    GetCommState( wsa->com_handle, &dcb ) ;//dcb의 기본값을 받는다.

    switch( baud )
    {
    case 110    : dcb.BaudRate = CBR_110     ; break;
    case 300    : dcb.BaudRate = CBR_300     ; break;
    case 600    : dcb.BaudRate = CBR_600     ; break;
    case 1200   : dcb.BaudRate = CBR_1200    ; break;
    case 2400   : dcb.BaudRate = CBR_2400    ; break;
    case 4800   : dcb.BaudRate = CBR_4800    ; break;
    case 9600   : dcb.BaudRate = CBR_9600    ; break;
    case 14400  : dcb.BaudRate = CBR_14400   ; break;
    case 19200  : dcb.BaudRate = CBR_19200   ; break;
    case 38400  : dcb.BaudRate = CBR_38400   ; break;
    case 57600  : dcb.BaudRate = CBR_57600   ; break;
    case 115200 : dcb.BaudRate = CBR_115200  ; break;
    case 128000 : dcb.BaudRate = CBR_128000  ; break;
    case 256000 : dcb.BaudRate = CBR_256000  ; break;
    default     : return -1;
    }

    if( data > 0 ) dcb.ByteSize = data;

    switch( parity )
    {
    case 'o' : dcb.Parity = ODDPARITY       ; break;
    case 'e' : dcb.Parity = EVENPARITY      ; break;
    case 'n' : dcb.Parity = NOPARITY        ; break;
    default  : return -1;
    }

    dcb.StopBits        =   ONESTOPBIT;

    dcb.fOutxDsrFlow    =   FALSE;
    dcb.fDtrControl     =   DTR_CONTROL_DISABLE;

    dcb.fOutxCtsFlow    =   FALSE;
    dcb.fRtsControl     =   RTS_CONTROL_DISABLE;

    dcb.fInX            =   FALSE;
    dcb.fOutX           =   FALSE;

    dcb.fBinary         =   TRUE ;
    dcb.fParity         =   FALSE ;

    if( !SetCommState( wsa->com_handle, &dcb ) ) //변경된 Dcb 설정
    {
        wsa_log_write( wsa, "SetCommState Error" );
        wsa_clear_error( wsa );
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
// 에러 상태를 없앤다.
// 반환 : 0 이면 구조체에 에러이거나 , 해들이 오픈되지 않았거나 , 정상
//        그외발생된 에러 코드
//---------------------------------------------------------------------------
int  wsa_clear_error( win_serial_api_t * wsa )
{
    COMSTAT com_stat;
    DWORD   error_flags;

    if( wsa == NULL )          return -1;
    if( wsa->com_handle == 0 ) return -1;

    ClearCommError( wsa->com_handle, &error_flags, &com_stat);

    if( error_flags & CE_BREAK      ) wsa_log_write( wsa, "Error : CE_BREAK"    );
    if( error_flags & CE_FRAME      ) wsa_log_write( wsa, "Error : CE_FRAME"    );
    if( error_flags & CE_OVERRUN    ) wsa_log_write( wsa, "Error : CE_OVERRUN"   );
    if( error_flags & CE_RXOVER     ) wsa_log_write( wsa, "Error : CE_RXOVER"   );
    if( error_flags & CE_RXPARITY   ) wsa_log_write( wsa, "Error : CE_RXPARITY"  );
    if( error_flags & CE_TXFULL     ) wsa_log_write( wsa, "Error : CE_TXFULL"   );
    if( error_flags & CE_IOE        ) wsa_log_write( wsa, "Error : CE_IOE"      );
    if( error_flags & CE_MODE       ) wsa_log_write( wsa, "Error : CE_MODE"         );

    if( com_stat.cbInQue  ) wsa_log_write( wsa, "In  Queue count = %d", com_stat.cbInQue  );
    if( com_stat.cbOutQue ) wsa_log_write( wsa, "Out Queue count = %d", com_stat.cbOutQue );

    return error_flags;
}

//---------------------------------------------------------------------------
// 쓰기 조건을 설정한다.
// blocking : 쓰기 조건이 완료될때 까지 무한 대기
// time_out_msec : blocking = 0 일 경우 시간 초과 한계값
//---------------------------------------------------------------------------
void wsa_set_write_option ( win_serial_api_t * wsa, int blocking, int time_out_msec )
{
    if( wsa == NULL )          return;

    wsa->w_wait         = blocking;
    wsa->w_timeout_msec = time_out_msec;
}

//---------------------------------------------------------------------------
// 버퍼를 플러싱 한다.
// 반환 : 0 보다 크면 써진 데이타 크기
//        0 이면 시간 초과
//        0 보다 작으면 에러
//---------------------------------------------------------------------------
int wsa_flush( win_serial_api_t *wsa )
{
    if( wsa == NULL )          return -1;
    if( wsa->com_handle == 0 ) return -1;
    PurgeComm( wsa->com_handle,
                PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
    return 0;
}


//---------------------------------------------------------------------------
// 데이터를 전송한다.
// 반환 : 0 보다 크면 써진 데이타 크기
//        0 이면 시간 초과
//        0 보다 작으면 에러
//---------------------------------------------------------------------------
int wsa_write( win_serial_api_t *wsa, void *data, int size )
{
    unsigned long   wsize_r;
    DWORD           wait_ret;

    if( wsa == NULL )          return -1;
    if( wsa->com_handle == 0 ) return -1;

    if( !WriteFile( wsa->com_handle, data, size, &wsize_r, &wsa->w_event ) )
    {
        if( GetLastError() == ERROR_IO_PENDING )
        {
            wait_ret =  WaitForSingleObject(wsa->w_event.hEvent, wsa->w_timeout_msec );

            if( wait_ret == WAIT_OBJECT_0  )
            {
                if(GetOverlappedResult(wsa->com_handle,
                      &wsa->w_event,   &wsize_r, wsa->w_wait ))
                {
                    return wsize_r;
                }
                else
                {
                    wsa_clear_error(wsa);
                    return -1;
                }
            }

            // 시간 초과가 발생헸다.
            if( wait_ret == WAIT_TIMEOUT )  return 0;

            // 이 이후에는 다음 상태인데 이 경우는 무조건 에러 처리 해야 한다.
            // WAIT_FAILED
            // WAIT_ABANDONED

        }
        wsa_clear_error(wsa);
        return -1;
    }

    return (int) wsize_r;
}

//---------------------------------------------------------------------------
// 수신된 데이터수를 얻는다.
//---------------------------------------------------------------------------
int wsa_get_rx_data_size( win_serial_api_t *wsa )
{
    COMSTAT com_stat;
    DWORD   error_flags;

    if( wsa == NULL )          return -1;
    if( wsa->com_handle == 0 ) return -1;

    ClearCommError( wsa->com_handle, &error_flags, &com_stat);

    return com_stat.cbInQue;
}

//---------------------------------------------------------------------------
// 데이터를 읽어 온다.
//---------------------------------------------------------------------------
int wsa_read( win_serial_api_t *wsa, void *data, int size )
{
    int             ret;
    unsigned long   rsize_r;


    if( wsa == NULL )          return -1;
    if( wsa->com_handle == 0 ) return -1;

    if( size  > 0)
    {
        ret = ReadFile( wsa->com_handle, data, size , &rsize_r, &wsa->r_event ) ;
        if( !ret ) return -1;
    }
    return rsize_r;
}
