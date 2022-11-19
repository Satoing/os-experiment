#include "thread_hdr.h"

void add_ready_thread(thread* ready_thread)
{
	ready_queue.push_back(ready_thread);
}

void schedule()
{
	if(current_thread != &idle_thread)
		ready_queue.push_back(current_thread);
	
	if(ready_queue.size() == 0) return;

	current_thread = ready_queue.front();
	ready_queue.pop_front();
}

void current_thread_finished()
{
	if(ready_queue.size() != 0) {
		current_thread = ready_queue.front();
		ready_queue.pop_front();
	} else current_thread = &idle_thread;
}

void current_thread_blocked()
{
	blocked_queue.push_back(current_thread);
	if(ready_queue.size() != 0) {
		current_thread = ready_queue.front();
		ready_queue.pop_front();
	} else current_thread = &idle_thread;
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