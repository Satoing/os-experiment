#include "thread_hdr.h"
#include "thread_student.cpp"
#include <iostream>

thread idle_thread = { 0 };

thread_queue first_ready_queue;
thread_queue second_ready_queue;

thread_queue blocked_queue;

thread* current_thread = &idle_thread;

void _common_init()
{
	first_ready_queue.clear();
	second_ready_queue.clear();
	blocked_queue.clear();
	set_time_interval(20);
	set_first_time_ticks(40);
	set_second_time_ticks(60);
	current_thread = &idle_thread;
}

bool _is_valid_status(
	pthread target_thread,
	unsigned int first_queue_size,
	unsigned int second_queue_size
)
{
	if (current_thread != target_thread ||
		first_ready_queue.size() != first_queue_size ||
		second_ready_queue.size() != second_queue_size)
	{
		return false;
	}
	return true;
}

bool test_simple_thread_with_block()
{
	_common_init();

	thread thread1 = { 1 };
	thread thread2 = { 2 };
	add_ready_thread(&thread1);
	add_ready_thread(&thread2);

	for (int i = 0; i < 2; i++)
	{
		on_clock();
		if (!_is_valid_status(&thread1, 1, 0))
		{
			std::cout<<"error1"<<'\n';
			return false;
		}
	}

	on_clock();
	if (!_is_valid_status(&thread2, 0, 1))
	{
		std::cout<<"error2"<<'\n';
		return false;
	}

	current_thread_blocked();
	if (!_is_valid_status(&thread1, 0, 0))
	{
		std::cout<<"error3"<<'\n';
		return false;
	}
	notify_all();
	on_clock();

	if (!_is_valid_status(&thread2, 0, 1))
	{
		std::cout<<"error4"<<'\n';
		return false;
	}
	on_clock();
	on_clock();
	on_clock();
	if (!_is_valid_status(&thread1, 0, 1))
	{
		std::cout<<"error5"<<'\n';
		return false;
	}
	current_thread_blocked();
	on_clock();
	notify();
	if (!_is_valid_status(&thread1, 0, 1))
	{
		std::cout<<"error6"<<'\n';
		return false;
	}
	current_thread_blocked();
	current_thread_blocked();
	notify_all();
	if (!_is_valid_status(&thread1, 1, 0))
	{
		std::cout<<"error7"<<'\n';
		return false;
	}
	current_thread_finished();
	if (!_is_valid_status(&thread2, 0, 0))
	{
		std::cout<<"error8"<<'\n';
		return false;
	}
	on_clock();
	on_clock();
	if (!_is_valid_status(&thread2, 0, 0))
	{
		std::cout<<"error9"<<'\n';
		return false;
	}
	current_thread_finished();
	if (!_is_valid_status(&idle_thread, 0, 0))
	{
		std::cout<<"error10"<<'\n';
		return false;
	}
	add_ready_thread(&thread1);
	on_clock();
	if (!_is_valid_status(&thread1, 0, 0))
	{
		std::cout<<"error11"<<'\n';
		return false;
	}
	on_clock();
	add_ready_thread(&thread2);
	on_clock();
	if (!_is_valid_status(&thread2, 0, 1))
	{
		std::cout<<"error12"<<'\n';
		return false;
	}

	return true;
}

int main()
{
	bool ret = test_simple_thread_with_block();
	std::cout << ret << std::endl;
	return 0;
}
