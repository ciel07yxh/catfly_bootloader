#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED
#include <sys/time.h>

#define TIMER_TIMEOUT 1

#include <stdio.h>

#define HOST_SEND_COMMAND   0x01
#define HOST_WAIT_ACK       0x02
#define HOST_TX_APP         0x03

#define HOST_MAX_RETRY      0x03
#define HOST_CMD_TIMEOUT    1000  //100ms
typedef struct zxy_control
{
    int status;
    long timer;
    long last_timeout;
    int  timeout_cout;
    void (*set_timer)(struct zxy_control *control,long time);
    int  (*check_timer)(struct zxy_control *control,void (* ptr)());
    void (*change_status)(struct zxy_control *control,int sta,long time);
    int (*is_max_time_out)(struct zxy_control *control,int maxTimeOut);

}zxy_control;

extern zxy_control __control;

#endif // CONTROL_H_INCLUDED
