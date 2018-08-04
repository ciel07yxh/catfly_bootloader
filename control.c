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

int check_timer(struct zxy_control *control)
 {
    if(get_sys_time_ms() > control->timer)
    {
        control->timeout_cout++;
        return TIMER_TIMEOUT;
    }
    else
        return 0;
 }

 void change_status(struct zxy_control *control,int current,int next,long time)
 {
        control->status = next;
        control->set_timer(control,time);
        control->last_status = current;
        control->last_timeout = time;

 }

int is_max_time_out(struct zxy_control *control,int maxTimeOut)
{
    if(control->timeout_cout>maxTimeOut-1)
        return 1;
    else
        return 0;

}

void resume_last_mission(struct zxy_control *control)
{
    change_status(control,control->last_status,control->last_status,control->last_timeout);
}

zxy_control __control=
{
0x01,
0,
0,
0,
0,
set_timer,
check_timer,
change_status,
is_max_time_out,
resume_last_mission
};

