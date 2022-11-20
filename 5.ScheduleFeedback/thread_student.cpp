#include "thread_hdr.h"

unsigned int first_time_ticks = 0;
unsigned int second_time_ticks = 0;
unsigned int time_interval = 0;
unsigned int cur = 0;

// 向第一个队列末尾添加线程
void add_ready_thread(thread* thread) {
    thread->max_clock_times = first_time_ticks / time_interval;
    first_ready_queue.push_back(thread);
}

// 调度线程
void schedule() {
    if(first_ready_queue.empty() && second_ready_queue.empty()){
        current_thread = &idle_thread;
        cur = 0;
    }
    else if(first_ready_queue.empty()){
        current_thread = second_ready_queue.front();
        cur = 2;
        second_ready_queue.pop_front();
    }
    else{
        current_thread = first_ready_queue.front();
        cur = 1;
        first_ready_queue.pop_front();
    }
}

// 当前线程结束时，调度新的进程
void current_thread_finished() {
    schedule();
}

// 当前线程阻塞，首先将其加入阻塞队列，然后调度新的线程
void current_thread_blocked() {
    blocked_queue.push_back(current_thread);
    schedule();
}

// 唤醒阻塞队列首部的线程，并将其放入一级队列末尾
// 注意要把该线程的max_clock_times设置成新的
void notify() {
    blocked_queue.front()->max_clock_times =first_time_ticks / time_interval;
    first_ready_queue.push_back(blocked_queue.front());
    blocked_queue.pop_front();
}

// 阻塞队列中的线程依次出队入队
void notify_all() {
    while(!blocked_queue.empty()) {
       notify();
    }
}

// 时隙到达，当前线程能运行的时隙-1
// 如果当前线程减到0，
void on_clock() {
    current_thread->max_clock_times -= 1;
    // 需要调度的情况
    if(current_thread->max_clock_times <= 0 || current_thread == &idle_thread) {
        // 当前非idle线程切换，放到二级队列中
        if(current_thread != &idle_thread) {
            current_thread->max_clock_times = second_time_ticks / time_interval;
            second_ready_queue.push_back(current_thread);
        }
        schedule();
    }

    // 重点，实现抢占式调度（这个需要看测试用例才知道）
    if(cur == 2 && !first_ready_queue.empty()){
        current_thread->max_clock_times = second_time_ticks / time_interval;
        second_ready_queue.push_back(current_thread);
        current_thread = first_ready_queue.front();
        cur = 1;
        first_ready_queue.pop_front();
    }
}

void set_first_time_ticks(unsigned int ticks)
{
    first_time_ticks = ticks;
}

void set_second_time_ticks(unsigned int ticks)
{
    second_time_ticks = ticks;
}

void set_time_interval(unsigned int interval)
{
    time_interval = interval;
}