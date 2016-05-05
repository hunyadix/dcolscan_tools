#include "Timer_colored.hh"

Timer_colored::Timer_colored(Console_actor& actor_p) : actor(actor_p)
{

}

void Timer_colored::restart(std::string text_p)
{
	std::cout << actor << text_p << std::endl;
	time(&(this -> start_t));
}

void Timer_colored::print_seconds(std::string pretext_p, std::string post_text_p)
{
	time(&current_t);
	seconds_elapsed = difftime(current_t, start_t);
	std::cout << actor << pretext_p << seconds_elapsed << post_text_p << std::endl;
}