#include "../hooks.h"

void __stdcall hk_create_move(int sequence, float input_sample_frametime, bool active, bool& send_packet)
{
	originals->create_move_original(interfaces->base_client, sequence, input_sample_frametime, active);

	game_user_cmd* cmd = interfaces->input->user_cmd(sequence);
	game_verified_user_cmd* verified_cmd = interfaces->input->verified_cmd(sequence);

	// check do we have valid commands, finished signing on to server and not playing back demos (where our commands are ignored)
	if (cmd == nullptr || verified_cmd == nullptr || !active)
		return;

	// store global variables to global pointer.
	csgo->cmd = cmd;
	csgo->local_player = interfaces->entity_list->get<cs_player>(interfaces->engine_client->get_local_player());
	csgo->send_packet = &send_packet;



	// anti-untrust (essential if you don't want ban).
	cmd->view_angles.sanitize();

	verified_cmd->cmd		= *cmd;
	verified_cmd->checksum	= cmd->checksum();
}

__declspec (naked) void __stdcall hk_functions::create_move_proxy(int sequence, float input_sample_frametime, bool active)
{
	/*
	* credit: https://github.com/rollraw/qo0-base/blob/2de7c14bf1941ad2ad77f30783f9e06fc646a153/base/core/hooks.cpp#L322
	* credit: https://github.com/lagcomp/csgo_sdk/blob/94ecdd119d8f4a502cca725687c6ab119362c56b/csgo_sdk/csgo/hooks/impl/hooks.cpp#L57
	* both of these resources helped me alot on figuring out how to hook create_move like this.
	*/

	__asm
	{
		push ebp
		mov  ebp, esp							// store the stack.
		push ebx								// send_packet.
		push esp								// restore the stack.

		push dword ptr[active]					// ebp + 16.
		push dword ptr[input_sample_frametime]	// ebp + 12.
		push dword ptr[sequence]				// ebp + 8.

		call hk_create_move						// our function.

		pop  ebx
		pop  ebp

		retn 12
	}
}