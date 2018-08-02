#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

#define TIMER_TIMEOUT 1

#include <stdio.h>

#define HOST_REQUEST_REBOOT        0x01
#define HOST_WAIT_BOOTLOADER       0x02
#define HOST_START_TRANS           0x03
#define HOST_WAIT_FILE_ACK         0x04
#define HOST_SEND_MOTE_ID          0x05
#define HOST_WAIT_MOTE_ID_ACK      0x06


#define HOST_FINISHED              0x80
#define HOST_ERROR                 0xFF


#define HOST_MAX_RETRY              0x03
#define HOST_CMD_TIMEOUT            1000  //100ms
typedef struct zxy_control
{
    int status;
    long timer;
    long last_timeout;
    int  timeout_cout;
    void (*set_timer)(struct zxy_control *control,long time);
    int  (*check_timer)(struct zxy_control *control,void (* ptr)());
    void (*change_status)(struct zxy_control *control,int sta,long time);
    int  (*is_max_time_out)(struct zxy_control *control,int maxTimeOut);

}zxy_control;

extern zxy_control __control;

#endif // CONTROL_H_INCLUDED
