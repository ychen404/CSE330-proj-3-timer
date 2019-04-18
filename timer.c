#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

unsigned long timer_interval_ns = 1e9;
static struct hrtimer hr_timer;

void my_function(void){
    printk("hello\n");
}


enum hrtimer_restart timer_callback( struct hrtimer *timer_for_restart )
{
    ktime_t currtime , interval;
    currtime  = ktime_get();
    interval = ktime_set(0,timer_interval_ns);
    hrtimer_forward(timer_for_restart, currtime , interval);
    my_function();
    return HRTIMER_RESTART;
}

static int timer_init(void) {
    ktime_t ktime;
    ktime = ktime_set( 0, timer_interval_ns );
    hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
    hr_timer.function = &timer_callback;
    hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );
    return 0;
}

static void timer_exit(void) {
    int ret;
    ret = hrtimer_cancel( &hr_timer );
    if (ret) printk("Timer was still in use!\n");
    printk("HR Timer removed\n");
}


MODULE_LICENSE("GPL");
module_init(timer_init);
module_exit(timer_exit);
