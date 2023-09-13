#pragma once

class game_localize
{
public:
	wchar_t*			find(const char* token) { return virtuals->call<wchar_t*>(this, 11, token); }

	/*
	* this funcion is basically the "find" function above,
	* however this "find_safe" function doesn't crash the game
	* if the value token you inputted is either invalid or incorrect.
	*/
	const wchar_t*		find_safe(const char* token) { return virtuals->call<wchar_t*>(this, 12, token); }
};