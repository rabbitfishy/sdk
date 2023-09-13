#pragma once
#include "bitbuf.h"

#define flow_out	0
#define flow_in		1
#define max_flow	2

// reference: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetchannelinfo.h
class game_net_channel_info
{
public:
	enum
	{
		generic = 0,		// must be first and is default group
		local_player,		// bytes for local player entity update
		other_players,		// bytes for other players update
		entities,			// all other entity bytes
		sounds,				// game sounds
		events,				// event messages
		temp_entities,		// temp entities
		user_messages,		// user messages
		entity_messages,	// entity messages
		voice,				// voice data
		string_table,		// a stringtable update
		move,				// client move cmds
		string_cmd,			// string command
		signon,				// various signondata
		paint_map,			// paintmap data
		encrypted,			// encrypted data
		total,				// must be last and is not a real group
	};

	virtual const char* name() const = 0;													// get channel name
	virtual const char* address() const = 0;												// get channel ip address as string
	virtual float		time() const = 0;													// current net time
	virtual float		time_connected() const = 0;											// get connection time in seconds
	virtual int			buffer_size() const = 0;											// netchannel packet history size
	virtual int			data_rate() const = 0;												// send data rate in byte/sec
	virtual bool		is_loopback() const = 0;											// true if loopback channel
	virtual bool		is_timing_out() const = 0;											// true if timing out
	virtual bool		is_playback() const = 0;											// true if demo playback
	virtual float		latency(int flow) const = 0;										// current latency (rtt), more accurate but jittering
	virtual float		average_latency(int flow) const = 0;								// average packet latency in seconds
	virtual float		average_loss(int flow) const = 0;									// average packet loss[0..1]
	virtual float		average_choke(int flow) const = 0;									// average packet choke[0..1]
	virtual float		average_data(int flow) const = 0;									// data flow in bytes/sec
	virtual float		average_packets(int flow) const = 0;								// average packets/sec
	virtual int			total_data(int flow) const = 0;										// total flow in/out in bytes
	virtual int			total_packets(int flow) const = 0;
	virtual int			sequence(int flow) const = 0;										// last send seq number
	virtual bool		valid_packet(int flow, int frame) const = 0;						// true if packet was not lost/dropped/chocked/flushed
	virtual float		packet_time(int flow, int frame) const = 0;							// time when packet was send
	virtual int			packet_bytes(int flow, int frame, int group) const = 0;				// group size of this packet
	virtual bool		stream_progress(int flow, int* received, int* total) const = 0;		// tcp progress if transmitting
	virtual float		time_since_last_received() const = 0;								// get time since last received packet in seconds
	virtual	float		command_interpolation_amount(int flow, int frame) const = 0;
	virtual void		packet_response_latency(int flow, int frame_number, int* latency_secs, int* choke) const = 0;
	virtual void		remote_framerate(float* frame_time, float* frame_time_standard_deviation, float* frame_start_time_standard_deviation) const = 0;
	virtual float		timeout_seconds() const = 0;
};

class game_net_channel
{
public:
	std::byte	_pad0[0x14];			//0x0000
	bool		processing_messages;	//0x0014
	bool		should_delete;			//0x0015
	bool		stop_processing;		//0x0016
	std::byte	_pad1[0x1];				//0x0017
	int			out_sequence;			//0x0018 last send outgoing sequence number
	int			in_sequence;			//0x001c last received incoming sequence number
	int			out_sequence_ack;		//0x0020 last received acknowledge outgoing sequence number
	int			out_reliable_state;		//0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int			in_reliable_state;		//0x0028 state of incoming reliable data
	int			choked_packets;			//0x002c number of choked packets
	std::byte	_pad2[0x414];			//0x0030

	int		send_datagram(bf_write* datagram) { return virtuals->call<int>(this, 46, datagram); }
	bool	transmit(bool only_reliable = false) { return virtuals->call<bool>(this, 49, only_reliable); }
};

// reference: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetmessage.h
class game_net_message
{
public:
	virtual						~game_net_message() { }
	virtual void				set_net_channel(void* net_channel) = 0;
	virtual void				set_reliable(bool state) = 0;
	virtual bool				process() = 0;
	virtual	bool				read_from_buffer(bf_read& buffer) = 0;
	virtual	bool				write_to_buffer(bf_write& buffer) = 0;
	virtual bool				is_reliable() const = 0;
	virtual int					message_type() const = 0;
	virtual int					message_group() const = 0;
	virtual const char*			message_name() const = 0;
	virtual game_net_channel*	net_channel() const = 0;
	virtual const char*			to_string() const = 0;
	virtual std::size_t			get_size() const = 0;
};

class clc_message_move
{
private:
	std::byte _pad0[0x8];
public:
	int backup_commands;
	int new_commands;
};

template <typename t>
class game_net_message_pb : public game_net_message, public t { };
using game_message_move = game_net_message_pb<clc_message_move>;

// reference: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/inetmsghandler.h
class game_net_channel_handler
{
public:
	virtual			~game_net_channel_handler() { }
	virtual void	connection_start(game_net_channel* channel) = 0;											// called first time network channel is established
	virtual void	connection_stop() = 0;																		// called first time network channel is established
	virtual void	connection_closing(const char* reason) = 0;													// network channel is being closed by remote site
	virtual void	connection_crashed(const char* reason) = 0;													// network error occured
	virtual void	packet_start(int incoming_sequence, int outgoing_acknowledged) = 0;							// called each time a new packet arrived
	virtual void	packet_end() = 0;																			// all messages has been parsed
	virtual void	file_requested(const char* file_name, unsigned int transfer_id, bool replay_demo_file) = 0;	// other side request a file for download
	virtual void	file_received(const char* file_name, unsigned int transfer_id, bool replay_demo_file) = 0;	// we received a file
	virtual void	file_denied(const char* file_name, unsigned int transfer_id, bool replay_demo_file) = 0;	// a file request was denied by other side
	virtual void	file_sent(const char* file_name, unsigned int transfer_id, bool replay_demo_file) = 0;		// we sent a file
	virtual bool	change_split_screen_user(int split_screen_user_slot) = 0;									// interleaved networking used by ss system is changing the ss player slot that the subsequent messages pertain to
};