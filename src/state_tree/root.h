#pragma once

#include "branch_nodes.h"
#include "node.h"
#include <agent_behaviours/sensors.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class NNSTRoot : public NNSTBranchNodes {
	GDCLASS(NNSTRoot, NNSTBranchNodes)

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
	Dictionary _blackboard;

protected:
	static void _bind_methods();

public:
	NNSTRoot();
	~NNSTRoot();

	// Getters and setters for attributes.

	void set_blackboard(Dictionary blackboard);
	Dictionary get_blackboard() const;

#ifdef DEBUG_ENABLED
	uint64_t get_total_tick_usec() const;
	void set_total_tick_usec(uint64_t total_tick_usec);
#endif

	// Handling functions.
	virtual void send_event(String name) override;

	virtual void _transition_in() override;

	void tick(float delta);

	TypedArray<NNSensors> get_child_sensors() { return _child_sensors; };

	// Godot virtuals.
	void _notification(int p_what);
};

} //namespace godot
