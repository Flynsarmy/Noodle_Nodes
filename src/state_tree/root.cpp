#include "root.h"
#include "../agent_behaviours/sensors.h"
#include "nodes.h"

#include "../performance_monitor_singleton.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>
#ifdef DEBUG_ENABLED
#include "../debugger/debugger_overlay.h"
#endif

using namespace godot;

// Method binds.

void NNSTRoot::_bind_methods() {
#ifdef DEBUG_ENABLED
	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_total_tick_usec", "total_tick_usec"), &NNSTRoot::set_total_tick_usec);
	ClassDB::bind_method(D_METHOD("get_total_tick_usec"), &NNSTRoot::get_total_tick_usec);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "total_tick_usec", PROPERTY_HINT_NONE), "set_total_tick_usec", "get_total_tick_usec");
#endif

	ClassDB::bind_method(D_METHOD("tick", "blackboard", "delta"), &NNSTRoot::tick);

	ClassDB::bind_method(D_METHOD("get_active_states"), &NNSTRoot::get_active_states);

	ClassDB::bind_method(D_METHOD("get_child_sensors"), &NNSTRoot::get_child_sensors);
}

// Constructor and destructor.

NNSTRoot::NNSTRoot() {
#ifdef DEBUG_ENABLED
	_total_tick_usec = 0;
	_total_transition_usec = 0;
#endif

	_child_sensors.clear();
	_num_child_sensors = 0;
}

NNSTRoot::~NNSTRoot() {
}

// Getters and Setters.

#ifdef DEBUG_ENABLED
void NNSTRoot::set_total_tick_usec(uint64_t total_tick_usec) {
	_total_tick_usec = total_tick_usec;
}

uint64_t NNSTRoot::get_total_tick_usec() const {
	return _total_tick_usec;
}
#endif

TypedArray<NNSTNode> NNSTRoot::get_active_states() const {
	return _active_states;
}

// Handling methods.

void NNSTRoot::transition_to(NodePath path_to_node, Variant blackboard, float delta) {
	NNSTNodes *new_state = get_node<NNSTNodes>(path_to_node);
	if (new_state == nullptr) {
		return;
	}
	bool result = try_transition(new_state, blackboard, delta);
}

bool NNSTRoot::try_transition(NNSTNodes *transition_target_node, Variant blackboard, float delta) {
#ifdef DEBUG_ENABLED
	uint64_t method_start_time_usec = godot::Time::get_singleton()->get_ticks_usec();
#endif
	// Check that this is a valid transition for this tree.
	if (transition_target_node == nullptr) {
#ifdef DEBUG_ENABLED
		_total_transition_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
		NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_state_trees_usec(_total_transition_usec);
#endif
		return false;
	}
	if (!get_is_enabled()) {
#ifdef DEBUG_ENABLED
		_total_transition_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
		NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_state_trees_usec(_total_transition_usec);
#endif
		return false;
	}
	if (Engine::get_singleton()->is_editor_hint()) {
#ifdef DEBUG_ENABLED
		_total_transition_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
		NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_state_trees_usec(_total_transition_usec);
#endif
		return false;
	}
	bool new_state_found = false;
	if (NNSTNodes *new_state = transition_target_node->evaluate_state_activation(blackboard, delta)) {
		// We got a new leaf state. Get the new state list.
		_active_states_vector.clear();
		TypedArray<NNSTNode> new_active_states;
		std::vector<bool> is_existing_state; // = new std::vector<bool>();
		Node *cur_state = new_state;
		while (cur_state != this) {
			new_active_states.push_back(cur_state);
			is_existing_state.push_back(_active_states.has(cur_state));
			_active_states_vector.push_back(godot::Object::cast_to<NNSTNode>(cur_state));

			cur_state = cur_state->get_parent();
		}

		// Now that we have the new states in the list, do on_exit to those that
		// are leaving the list.
		for (int i = 0; i < _active_states.size(); ++i) {
			NNSTNode *cur_active_state = godot::Object::cast_to<NNSTNode>(_active_states[i]);
			if (!new_active_states.has(cur_active_state)) {
				cur_active_state->on_exit_state(blackboard, delta);
			}
		}

		// And then enter the new states.
		_active_states = new_active_states;
		for (int i = _active_states_vector.size() - 1; i > -1; --i) {
			if (!is_existing_state[i]) {
				//NNSTNodes* cur_active_state = godot::Object::cast_to<NNSTNodes>(_active_states[i]);
				//cur_active_state->on_enter_state(blackboard, delta);
				_active_states_vector[i]->on_enter_state(blackboard, delta);
			}
		}
		new_state_found = true;
	} //endif new state found
#ifdef DEBUG_ENABLED
	_total_transition_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
	NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_state_trees_usec(_total_transition_usec);
#endif
	return new_state_found;
}

void NNSTRoot::tick(Variant blackboard, float delta) {
#ifdef DEBUG_ENABLED
	uint64_t method_start_time_usec = godot::Time::get_singleton()->get_ticks_usec();
#endif

	// No states, so try transition to the root.
	if (_active_states_vector.size() == 0) {
		try_transition(this, blackboard, delta);
	}

	// Update the sensors.
	//for( int i = 0; i < get_child_count(); ++i ) {
	//    Node* node = get_child(i);
	//    if( NNSensors* sensor = godot::Object::cast_to<NNSensors>(node) ) {
	for (unsigned int i = 0; i < _num_child_sensors; i++) {
		NNSensors *sensor = godot::Object::cast_to<NNSensors>(_child_sensors[i]);
		// NNSensors *sensor = _child_sensors[i];
		if (!sensor->get_is_enabled()) {
			continue;
		}
		sensor->evaluate_sensor_value();
		//    } else {
		//        break; // No more sensors.
		//    }
	} //endfor sensors

	// If there are active states, tick their custom method from the
	// root to the active leaf.
	if (_active_states_vector.size() > 0) {
		for (int i = _active_states_vector.size() - 1; i > -1; --i) {
			NNSTNodes *stnode = _active_states_vector[i];

			//if( NNSTNodes* stnode = godot::Object::cast_to<NNSTNodes>(_active_states[i]) ) {
			stnode->on_tick(blackboard, delta);
			//}
		}
	}
#ifdef DEBUG_ENABLED
	_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
	NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_state_trees_usec(_total_tick_usec);
#endif
}

// Godot virtuals.

void NNSTRoot::_notification(int p_what) {
	NNSTNodes::_notification(p_what);

	if (p_what == NOTIFICATION_READY || p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		if (Engine::get_singleton()->is_editor_hint())
			return;

		set_root_node(this);
		_child_sensors.clear();
		for (int i = 0; i < get_child_count(); ++i) {
			if (NNSensors *sensor = godot::Object::cast_to<NNSensors>(get_child(i))) {
				_child_sensors.push_back(sensor);
			}
		}
		_num_child_sensors = (unsigned int)_child_sensors.size();

#ifdef DEBUG_ENABLED
		NNDebuggerOverlay::get_singleton()->register_state_tree(this->get_instance_id());
#endif

		return;
	}
}
