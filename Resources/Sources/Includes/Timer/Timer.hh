#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <iostream>

class Timer
{
	protected:
		time_t start_t;
		time_t current_t;
		double seconds_elapsed;	
	public:
		Timer();
		~Timer();
		virtual void restart(std::string  text_p);
		virtual void print_seconds(std::string pretext_p, std::string post_text_p);
};

#endif
