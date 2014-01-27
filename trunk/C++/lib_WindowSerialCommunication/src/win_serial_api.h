//---------------------------------------------------------------------------

#ifndef _WIN_SERIAL_API_HEADER_
#define _WIN_SERIAL_API_HEADER_
//---------------------------------------------------------------------------

typedef struct
{
    int             com_debug;

    HANDLE          com_handle;
    OVERLAPPED      w_event;
    OVERLAPPED      r_event;

    int             w_wait;
    int             w_timeout_msec;
    int             tx_packet_max_size;

    unsigned char   *tx_data;
    int              tx_size;

    int             r_wait;
    int             r_timeout_msec;

    void            *priv;

    int             (*log_func)( void * wsa, const char *log_str );

} win_serial_api_t;

#ifdef __cplusplus
extern "C"
{
#endif

    void                wsa_scan_ports      ( void );
    int                 wsa_get_port_count  ( void );
    int                 wsa_get_port_name   ( int index, char *name );

    win_serial_api_t *  wsa_create      ( void );
    void                wsa_free        ( win_serial_api_t * wsa );

    int                 wsa_log_write( win_serial_api_t * wsa, const char *fmt, ... );

    HANDLE              wsa_open        ( win_serial_api_t * wsa, char *port_filename );
    void                wsa_close       ( win_serial_api_t * wsa );

    int                 wsa_set_read_event_mode( win_serial_api_t * wsa );

    HANDLE              wsa_get_read_event( win_serial_api_t * wsa );
    HANDLE              wsa_get_write_event( win_serial_api_t * wsa );

    void                wsa_set_log_func( win_serial_api_t * wsa, int (*log_func)( void *wsa, const char *log_str ) );

    int                 wsa_setup       ( win_serial_api_t * wsa, int baud, int data, char parity );
    void                wsa_set_write_option ( win_serial_api_t * wsa, int blocking, int time_out_msec );
    int                 wsa_clear_error ( win_serial_api_t * wsa );

    int                 wsa_write( win_serial_api_t *wsa, void *data, int size );
    int                 wsa_flush( win_serial_api_t *wsa );

    int                 wsa_get_rx_data_size( win_serial_api_t *wsa );
    int                 wsa_read( win_serial_api_t *wsa, void *data, int size );

#ifdef __cplusplus
}
#endif

#endif // _WIN_SERIAL_API_HEADER_
