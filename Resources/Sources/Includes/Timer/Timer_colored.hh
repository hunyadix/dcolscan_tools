#ifndef TIMER_COLORED_H
#define TIMER_COLORED_H

#include "Timer.hh"
#include "../Console_actor/Console_actor.hh"

class Timer_colored : public Timer
{
	private:
		Console_actor actor;
	public:
		Timer_colored(Console_actor& actor_p);
		void restart(std::string text_p);
		void print_seconds(std::string pretext_p, std::string post_text_p);
};

#endif
