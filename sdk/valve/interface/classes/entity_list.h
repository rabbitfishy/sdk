#pragma once

class base_entity;

class game_entity_listener
{
public:
	virtual void	entity_created(base_entity* entity) { }
	virtual void	entity_deleted(base_entity* entity) { }
};

class client_networkable;
class client_unknown;
class client_entity;

class game_entity_list
{
public:
	virtual client_networkable*		get_networkable(int index) = 0;
	virtual client_networkable*		get_networkable_from_handle(base_handle networkable_handle) = 0;
	virtual client_unknown*			get_unknown_from_handle(base_handle unknown_handle) = 0;
	virtual client_entity*			get_entity(int index) = 0;
	virtual client_entity*			get_entity_from_handle(base_handle entity_handle) = 0;
	virtual int						number_of_entities(bool include_non_networkable) = 0;
	virtual int						highest_entity_index() = 0;
	virtual void					set_max_entities(int max_entity) = 0;
	virtual int						max_entities() = 0;

	template <class t = client_entity>
	inline t* get(const int index) { return static_cast<t*>(get_entity(index)); }

	template <class t = client_entity>
	inline t* get_handle(const base_handle entity_handle) { return static_cast<t*>(get_entity_from_handle(entity_handle)); }

	void add_listener(game_entity_listener* listener) { entity_listeners.add_tail(listener); }
	void remove_listener(game_entity_listener* listener) { entity_listeners.find_remove(listener); }

private:
	utl_vector<game_entity_listener*> entity_listeners;
};