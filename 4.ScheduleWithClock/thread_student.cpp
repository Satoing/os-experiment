#include "thread_hdr.h"
#include <iostream>

int myinterval = 0, myticks = 0;

void add_ready_thread(thread* ready_thread)
{
	ready_queue.push_back(ready_thread);
	ready_thread -> clock_times = 0;
}

void schedule()
{
	if(current_thread != &idle_thread)
		ready_queue.push_back(current_thread);
	
	if(ready_queue.size() == 0) {
		current_thread = &idle_thread;
		current_thread->clock_times = 0;
		return;
	}

	current_thread = ready_queue.front();
	current_thread -> clock_times = 0;
	ready_queue.pop_front();
}

void current_thread_finished()
{
	if(ready_queue.size() != 0) {
		current_thread = ready_queue.front();
		current_thread -> clock_times = 0;
		ready_queue.pop_front();
	} else {
		current_thread = &idle_thread;
		current_thread->clock_times = 0;
	}
}

void current_thread_blocked()
{
	blocked_queue.push_back(current_thread);
	if(ready_queue.size() != 0) {
		current_thread = ready_queue.front();
		current_thread -> clock_times = 0;
		ready_queue.pop_front();
	} else {
		current_thread = &idle_thread;
		current_thread->clock_times = 0;
	}
}

void notify()
{
	if(blocked_queue.size() != 0) {
		ready_queue.push_back(blocked_queue.front());
		blocked_queue.pop_front();
	}
}

void notify_all()
{
	int len = blocked_queue.size();
	for(int i =0;i<len;i++) {
		ready_queue.push_back(blocked_queue.front());
		blocked_queue.pop_front();
	}
}

void on_clock()
{
	current_thread->clock_times += myinterval;
	std::cout<<current_thread->clock_times<<'\n';
	if(current_thread->clock_times>=myticks  || current_thread == &idle_thread) {
		current_thread->clock_times = 0;
		schedule();
	}
	
}

void set_time_ticks(unsigned int ticks)
{
	myticks = ticks;
}

void set_time_interval(unsigned int interval)
{
	myinterval = interval;
}