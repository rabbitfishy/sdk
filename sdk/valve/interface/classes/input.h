#pragma once
#include "../../classes/user_cmd.h"

#define multiplayer_backup 150

class game_input
{
public:
    std::byte               _pad0[0xC];             // 0x00
    bool                    track_ir_available;     // 0x0C
    bool                    mouse_installed;        // 0x0D
    bool                    mouse_active;           // 0x0E
    std::byte               _pad1[0x9A];            // 0x0F
    bool                    camera_in_thirdperson;  // 0xA9
    std::byte               _pad2[0x2];             // 0xAA
    vector_3d               camera_offset;          // 0xAC
    std::byte               _pad3[0x38];            // 0xB8
    game_user_cmd*          commands;               // 0xF0
    game_verified_user_cmd* verified_commands;      // 0xF4
    
    game_user_cmd* user_cmd(const int sequence_number) const 
    {
        if (!this) return nullptr;
        return &commands[sequence_number % multiplayer_backup]; 
    }

    game_verified_user_cmd* verified_cmd(const int sequence_number) const 
    {
        if (!this) return nullptr;
        return &verified_commands[sequence_number % multiplayer_backup]; 
    }
};

class game_input_system
{
public:
    void enable_input(bool enable) { return virtuals->call<void>(this, 11, enable); }
    void reset_input() { return virtuals->call<void>(this, 39); }
};