#pragma once

#define CVAR( name, variable ) struct name { static game_convar* convar( ) { return interfaces->convar->find( variable ); } }

/*
* put all your convars here cuz its easier to find and maintain.
* example of use:
* struct [what ever name u want] <-- this here is to keep it maintained and less messy also easier to find.
* {
*	CVAR([convar name], "convar variable here.");
* };
*/

namespace convars
{


	struct other
	{
		CVAR(inferno_flame_lifetime, "inferno_flame_lifetime");
	};
}