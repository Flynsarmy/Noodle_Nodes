#pragma once

#include "node.h"
#include "task_nodes.h"
#include <agent_behaviours/sensors.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class NNSTRoot : public NNSTTaskNodes {
	GDCLASS(NNSTRoot, NNSTTaskNodes)

private:
#ifdef DEBUG_ENABLED
	uint64_t _total_tick_usec;
	uint64_t _total_transition_usec;
#endif
	// TypedArray<NNSTNodes> _active_states;
	// std::vector<NNSTNodes *> _active_states_vector;
	TypedArray<NNSensors> _child_sensors;
	unsigned int _num_child_sensors;
	bool _is_first_tick;

	Variant _ai_context;

protected:
	static void _bind_methods();

public:
	NNSTRoot();
	~NNSTRoot();

// Getters and setters for attributes.
#ifdef DEBUG_ENABLED
	uint64_t get_total_tick_usec() const;
	void set_total_tick_usec(uint64_t total_tick_usec);
#endif

	// Handling functions.
	virtual void send_event(String name, Variant blackboard, float delta) override;

	virtual void _transition_in(Variant blackboard, float delta) override;

	void tick(Variant blackboard, float delta);

	TypedArray<NNSensors> get_child_sensors() { return _child_sensors; };

	// Godot virtuals.
	void _notification(int p_what);
};

} //namespace godot
