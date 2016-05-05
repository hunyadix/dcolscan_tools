#ifndef CONSOLE_ACTOR_H
#define CONSOLE_ACTOR_H

#include "../Console_colors/Console_colors.hh"
#include <string>
#include <ostream>

class Console_actor
{
	private:
		std::string name;
		Console_color::Modifier c_modifier;
	public:
		Console_actor(std::string name_p, Console_color::Modifier modifier_p);
		~Console_actor();
		friend std::ostream& operator <<(std::ostream& os_p, const Console_actor& actor_p);
};

#endif