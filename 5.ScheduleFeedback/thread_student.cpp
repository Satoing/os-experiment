#include "thread_hdr.h"
#include <deque>

int myinterval = 0, myticks1 = 0, myticks2 = 0;
int flag = 0;

void schedule()
{
	if(current_thread != &idle_thread)
		second_ready_queue.push_back(current_thread);
	
	if(first_ready_queue.size() !=0) {
		current_thread = first_ready_queue.front();
		first_ready_queue.pop_front();
		flag = 0;
	}
	else if(second_ready_queue.size() != 0) {
		current_thread = second_ready_queue.front();
		second_ready_queue.pop_front();
		flag = 1;
	}
	else current_thread = &idle_thread;

	current_thread->clock_times = 0;
}

void add_ready_thread(thread* ready_thread)
{
	first_ready_queue.push_back(ready_thread);
	ready_thread -> clock_times = 0;
}

void current_thread_finished()
{
	current_thread = &idle_thread;
	schedule();
}

void current_thread_blocked()
{
	blocked_queue.push_back(current_thread);
	current_thread->clock_times = 0;
	current_thread = &idle_thread;
	schedule();
}

void notify()
{
	if(blocked_queue.size() != 0) {
		first_ready_queue.push_back(blocked_queue.front());
		blocked_queue.pop_front();
		if(flag == 1) schedule();
	}
}

void notify_all()
{
	int len = blocked_queue.size();
	for(int i =0;i<len;i++) {
		first_ready_queue.push_back(blocked_queue.front());
		blocked_queue.pop_front();
	}
	if(flag == 1) schedule();
}

void on_clock()
{
	current_thread->clock_times += myinterval;
	if((current_thread == &idle_thread || flag == 1) && first_ready_queue.size() != 0) schedule();
	if(((!flag&&current_thread != &idle_thread) ? current_thread->clock_times >= myticks1:current_thread->clock_times >= myticks2)  || current_thread == &idle_thread) {
		schedule();
	}
}

void set_first_time_ticks(unsigned int ticks)
{
    myticks1 = ticks;
}

void set_second_time_ticks(unsigned int ticks)
{
    myticks2 = ticks;
}

void set_time_interval(unsigned int interval)
{
    myinterval = interval;
}