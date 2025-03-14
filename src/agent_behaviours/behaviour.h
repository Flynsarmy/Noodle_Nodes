#pragma once

#include "noodle_nodes.h"
#include <godot_cpp/classes/node.hpp>

#include "action.h"
#include "action_group.h"
#include "behaviours.h"
#include "considerations.h"

namespace godot {

class NNBehaviour : public NNBehaviours {
	GDCLASS(NNBehaviour, NNBehaviours)

private:
	float _cooldown_seconds;
	float _current_cooldown_seconds;
	int _cooldown_turns;
	int _current_cooldown_turns;
	//float  _score;
	int _behaviour_id;
	int _current_action_index;

	bool _can_be_interrupted;

	//std::vector<NNConsiderations*> _child_considerations;
	//unsigned int _num_child_considerations;

protected:
	static void _bind_methods();
#ifdef DEBUG_ENABLED
	uint64_t _last_evaluated_timestamp;
	uint64_t _last_visited_timestamp;
#endif
public:
	NNBehaviour();
	~NNBehaviour();

	// Getters and setters for attributes.

#ifdef DEBUG_ENABLED
	inline uint64_t get_last_visited_timestamp() const { return _last_visited_timestamp; };
	inline uint64_t get_last_evaluated_timestamp() const { return _last_evaluated_timestamp; };
#endif

	void set_behaviour_id(int behaviour_id);
	int get_behaviour_id() const;

	void set_can_be_interrupted(bool can_be_interrupted);
	bool get_can_be_interrupted() const;

	//void  set_score( float score );
	//float get_score() const;

	void set_cooldown_seconds(float cooldown_seconds);
	float get_cooldown_seconds() const;

	void set_cooldown_turns(int cooldown_turns);
	int get_cooldown_turns() const;

	void set_current_action_index(int current_action_index);
	int get_current_action_index() const;

	//NNAction* get_current_action_node() const { return _current_action_node;};

	// Godot virtuals.
	void _ready() override;

	void _process(double delta) override;
	//void _notification(int p_what);

	// Handling functions.

	float evaluate() override;
	void start_behaviour();
	void end_behaviour();
	NNAction *update_behaviour();

	Node *step_actions();
};

} //namespace godot
