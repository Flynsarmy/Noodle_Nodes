#pragma once

#include "noodle_nodes.h"

#include "performance_monitor_singleton.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

#include "action.h"
#include "behaviour.h"
#include "behaviour_group.h"
#include "sensors.h"

#define UTILITYAIAGENT_MAX_TOP_SCORING_BEHAVIOURS 16

namespace godot {

class NNAgent : public NoodleNodes {
	GDCLASS(NNAgent, NoodleNodes)

private:
	Ref<RandomNumberGenerator> _rng;
	NNBehaviourGroup *_current_behaviour_group_node;
	NNBehaviour *_current_behaviour_node;
	NNAction *_current_action_node;

	float _current_behaviour_bias;

	int _num_behaviours_to_select;
	int _num_possible_behaviours;
	NNBehaviourGroup *_top_scoring_behaviours_group_nodes[UTILITYAIAGENT_MAX_TOP_SCORING_BEHAVIOURS];
	NNBehaviour *_top_scoring_behaviours_nodes[UTILITYAIAGENT_MAX_TOP_SCORING_BEHAVIOURS];
	float _top_scoring_behaviours_score[UTILITYAIAGENT_MAX_TOP_SCORING_BEHAVIOURS];
	godot::String _top_scoring_behaviour_name;
	godot::String _current_behaviour_name;

	float _thinking_delay_in_seconds;
	float _thinking_delay_in_seconds_current_timer;

#ifdef DEBUG_ENABLED
	uint64_t _total_evaluate_options_usec;
	uint64_t _total_update_behaviour_usec;
#endif

	std::vector<NNSensors *> _child_sensors;
	std::vector<NNBehaviours *> _child_behaviours;
	//std::vector<NNBehaviourGroup*> _child_behaviour_groups;
	unsigned int _num_child_sensors;
	unsigned int _num_child_behaviours;

protected:
	static void _bind_methods();
	void place_into_top_n_behaviour_list(NNBehaviourGroup *behaviour_group, NNBehaviour *behaviour, float score);

#ifdef DEBUG_ENABLED
	uint64_t _last_evaluated_timestamp;
	uint64_t _last_visited_timestamp;
#endif
public:
	NNAgent();
	~NNAgent();

	// Getters and setters for attributes.

#ifdef DEBUG_ENABLED
	inline uint64_t get_last_visited_timestamp() const { return _last_visited_timestamp; };
	inline uint64_t get_last_evaluated_timestamp() const { return _last_evaluated_timestamp; };
#endif

	void set_thinking_delay_in_seconds(float thinking_delay_in_seconds);
	float get_thinking_delay_in_seconds() const;

	void set_current_behaviour_group_node(NNBehaviourGroup *new_behaviour_group_node);
	NNBehaviourGroup *get_current_behaviour_group_node() const;

	void set_current_behaviour_node(NNBehaviour *new_behaviour_node);
	NNBehaviour *get_current_behaviour_node() const;

	//void set_current_behaviour_index( int current_behaviour_index );
	//int  get_current_behaviour_index() const;

	void set_current_behaviour_name(godot::String current_behaviour_name);
	godot::String get_current_behaviour_name() const;

	void set_num_behaviours_to_select(int num_behaviours_to_select);
	int get_num_behaviours_to_select() const;

	void set_current_behaviour_bias(float current_behaviour_bias);
	float get_current_behaviour_bias() const;

	void set_top_scoring_behaviour_name(godot::String top_scoring_behaviour_name);
	godot::String get_top_scoring_behaviour_name() const;

	void set_current_action_node(NNAction *new_action_node);
	NNAction *get_current_action_node() const;

#ifdef DEBUG_ENABLED
	void set_total_evaluate_options_usec(uint64_t total_evaluate_options_usec);
	uint64_t get_total_evaluate_options_usec() const;

	void set_total_update_behaviour_usec(uint64_t total_update_behaviour_usec);
	uint64_t get_total_update_behaviour_usec() const;
#endif
	//void set_random_number_generator( RandomNumberGenerator rng );
	//RandomNumberGenerator get_random_number_generator() const;

	// Handling functions.

	void evaluate_options(float delta);

	void update_current_behaviour();

	void set_current_action_is_finished(bool is_finished);
	void abort_current_behaviour();

	// Godot virtuals
	void _ready() override;
	//virtual void _notification( int p_what ) override;
	void _process(double delta) override;
	void _physics_process(double delta) override;
};

} //namespace godot
