#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

#define TIMER_TIMEOUT 1

#include <stdio.h>
#include <stdint.h>

#define HOST_REQUEST_REBOOT        0x01
#define HOST_PREPARE_BIN           0x04
#define HOST_DOWNLOAD_BIN          0x02
#define HOST_SEND_MOTE_ID          0x03

#define HOST_WAIT_ACK              0x05


#define HOST_FINISHED              0x80
#define HOST_ERROR                 0xFF


#define HOST_MAX_RETRY              0x03
#define HOST_ERASE_TIMEOUT          1200
#define HOST_COMMUNI_TIMEOUT         500
typedef struct zxy_control
{
    uint8_t  status;
    long     timer;
    uint8_t  last_status;
    long     last_timeout;
    uint8_t   timeout_cout;
    void (*set_timer)(struct zxy_control *control,long time);
    int  (*check_timer)(struct zxy_control *control);
    void (*change_status)(struct zxy_control *control,int current,int next,long time);
    int  (*is_max_time_out)(struct zxy_control *control,int maxTimeOut);
    void (*resume_last_mission)(struct zxy_control *control);
}zxy_control;

extern zxy_control __control;

#endif // CONTROL_H_INCLUDED
