#include "Timer.hh"

Timer::Timer()
{

}

Timer::~Timer()
{

}

void Timer::restart(std::string  text_p)
{
	std::cout << text_p << std::endl;
	time(&(this -> start_t));
}

void Timer::print_seconds(std::string pretext_p, std::string post_text_p)
{
	time(&current_t);
	seconds_elapsed = difftime(current_t, start_t);
	std::cout << pretext_p << seconds_elapsed << post_text_p << std::endl;
}