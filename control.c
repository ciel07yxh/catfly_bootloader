#include "control.h"
#include <sys/time.h>

long get_sys_time_ms(void)
{
  struct timeval tv;
  gettimeofday(&tv, 0);
   // printf("msecond:%ld\n",tv.tv_sec*1000+tv.tv_usec/1000);  //秒
    return tv.tv_sec*1000+tv.tv_usec/1000;
}

void set_timer(struct zxy_control *control,long time)
{
    control->timer = get_sys_time_ms()+time;

}

 int  check_timer(struct zxy_control *control,int max_timeout_cout,void (* ptr)())
 {
    if(get_sys_time_ms() > control->timer)
    {
        control->timeout_cout++;
        if(control->timeout_cout>max_timeout_cout && max_timeout_cout>0)
        {
            //ptr();
            control->timeout_cout=0;
        }
        return TIMER_TIMEOUT;
    }
    else
        return 0;
 }

 void change_status(struct zxy_control *control,int sta,long time)
 {
        control->status = sta;
        control->set_timer(control,time);

 }

int is_max_time_out(struct zxy_control *control,int maxTimeOut)
{
    if(control->timeout_cout>maxTimeOut-1)
        return 1;
    else
        return 0;

}


zxy_control __control=
{
0x01,
0,
0,
0,
set_timer,
check_timer,
change_status,
is_max_time_out
};

