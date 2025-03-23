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
	ClassDB::bind_method(D_METHOD("set_blackboard", "blackboard"), &NNSTRoot::set_blackboard);
	ClassDB::bind_method(D_METHOD("get_blackboard"), &NNSTRoot::get_blackboard);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "blackboard", PROPERTY_HINT_DICTIONARY_TYPE), "set_blackboard", "get_blackboard");

#ifdef DEBUG_ENABLED
	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_total_tick_usec", "total_tick_usec"), &NNSTRoot::set_total_tick_usec);
	ClassDB::bind_method(D_METHOD("get_total_tick_usec"), &NNSTRoot::get_total_tick_usec);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "total_tick_usec", PROPERTY_HINT_NONE), "set_total_tick_usec", "get_total_tick_usec");
#endif

	ClassDB::bind_method(D_METHOD("tick", "delta"), &NNSTRoot::tick);

	// ClassDB::bind_method(D_METHOD("get_active_states"), &NNSTRoot::get_active_states);

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

	_blackboard = Dictionary();
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

void NNSTRoot::set_blackboard(Dictionary blackboard) {
	_blackboard = blackboard;
};
Dictionary NNSTRoot::get_blackboard() const {
	return _blackboard;
};

// Handling methods

void NNSTRoot::tick(float delta) {
#ifdef DEBUG_ENABLED
	uint64_t method_start_time_usec = godot::Time::get_singleton()->get_ticks_usec();
#endif

	if (_num_active_states == 0) {
		_transition_in();
	}

	// Update the sensors.
	for (unsigned int i = 0; i < _num_child_sensors; i++) {
		NNSensors *sensor = godot::Object::cast_to<NNSensors>(_child_sensors[i]);
		// NNSensors *sensor = _child_sensors[i];
		if (!sensor->get_is_enabled()) {
			continue;
		}
		sensor->evaluate_sensor_value();
	} //endfor sensors

	// If there are active states, tick their custom method from the
	// root to the active leaf.
	for (unsigned int i = 0; i < _num_active_states; i++) {
		NNSTTickedNodes *stnode = _active_states[i];

		stnode->set_root(this);
		stnode->on_tick(delta);
	}
#ifdef DEBUG_ENABLED
	_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
	NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_state_trees_usec(_total_tick_usec);
#endif
}

void NNSTRoot::send_event(String name) {
	for (unsigned int i = 0; i < _num_active_states; i++) {
		NNSTTickedNodes *stnode = _active_states[i];
		stnode->send_event(name);
	}
}

void NNSTRoot::_transition_in() {
	if (_num_active_states > 0) {
		return;
	}

	set_internal_status(ST_INTERNAL_STATUS_ACTIVE);

	std::vector<NNSTTickedNodes *> new_active_states;
	_evaluate_child_activations(new_active_states);

	NNSTTickedNodes *cur_active_state;

	// do on_exit for any states no longer active
	for (unsigned int i = 0; i < _num_active_states; i++) {
		cur_active_state = _active_states[i];

		bool found = false;
		for (unsigned int j = 0; j < new_active_states.size(); j++) {
			if (new_active_states[j] == cur_active_state) {
				found = true;
				break;
			}
		}
		if (!found) {
			cur_active_state->_transition_out();
		}
	}

	// And then enter the new states.
	for (unsigned int i = 0; i < new_active_states.size(); i++) {
		cur_active_state = new_active_states[i];

		bool found = false;
		for (unsigned int j = 0; j < _num_active_states; j++) {
			if (_active_states[j] == cur_active_state) {
				found = true;
				break;
			}
		}
		if (!found) {
			cur_active_state->set_root(this);
			cur_active_state->_transition_in();
		}
	}

	_active_states = new_active_states;
	_num_active_states = _active_states.size();
};

// Godot virtuals.

void NNSTRoot::_notification(int p_what) {
	NNSTBranchNodes::_notification(p_what);

	if (p_what == NOTIFICATION_READY || p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		if (Engine::get_singleton()->is_editor_hint())
			return;

		for (unsigned int i = 0; i < _num_child_states; i++) {
			NNSTTickedNodes *stnode = _child_states[i];
			stnode->set_root(this);
		}

		_child_sensors.clear();
		for (unsigned int i = 0; i < get_child_count(); i++) {
			if (NNSensors *sensor = godot::Object::cast_to<NNSensors>(get_child(i))) {
				_child_sensors.push_back(sensor);
			}
		}
		_num_child_sensors = (unsigned int)_child_sensors.size();

#ifdef DEBUG_ENABLED
		NNDebuggerOverlay::get_singleton()->register_state_tree(this->get_instance_id());
#endif

		// This will do nothing if we already have an active child. We want to
		// call it on child order changed incase the root has no active children
		// when another child gets added.
		_transition_in();

		return;
	}
}
