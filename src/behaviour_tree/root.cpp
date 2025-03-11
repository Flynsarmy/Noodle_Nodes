#include "root.h"
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

void NNBTRoot::_bind_methods() {
#ifdef DEBUG_ENABLED
	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_total_tick_usec", "total_tick_usec"), &NNBTRoot::set_total_tick_usec);
	ClassDB::bind_method(D_METHOD("get_total_tick_usec"), &NNBTRoot::get_total_tick_usec);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "total_tick_usec", PROPERTY_HINT_NONE), "set_total_tick_usec", "get_total_tick_usec");
#endif

	ClassDB::bind_method(D_METHOD("tick", "blackboard", "delta"), &NNBTRoot::tick);
}

// Constructor and destructor.

NNBTRoot::NNBTRoot() {
#ifdef DEBUG_ENABLED
	_total_tick_usec = 0;
#endif
	_num_child_sensors = 0;
}

NNBTRoot::~NNBTRoot() {
}

// Getters and Setters.

#ifdef DEBUG_ENABLED
void NNBTRoot::set_total_tick_usec(uint64_t total_tick_usec) {
	_total_tick_usec = total_tick_usec;
}

uint64_t NNBTRoot::get_total_tick_usec() const {
	return _total_tick_usec;
}
#endif

// Handling functions.

NNBTNodes::Status NNBTRoot::tick(Variant blackboard, float delta) {
#ifdef DEBUG_ENABLED
	uint64_t method_start_time_usec = godot::Time::get_singleton()->get_ticks_usec();
#endif
	if (!get_is_enabled()) {
#ifdef DEBUG_ENABLED
		_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
		NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_behaviour_trees_usec(_total_tick_usec);
#endif
		return Status::FAILURE;
	}
#ifdef DEBUG_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
		NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_behaviour_trees_usec(_total_tick_usec);

		return Status::FAILURE;
	}
#endif

	if (get_reset_rule() == NNBTNodesResetRule::NEVER) {
		if (get_internal_status() == BT_INTERNAL_STATUS_COMPLETED) {
#ifdef DEBUG_ENABLED
			_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
			NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_behaviour_trees_usec(_total_tick_usec);
#endif
			return get_tick_result();
		}
	}
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	for (unsigned int i = 0; i < _num_child_sensors; ++i) {
		NNSensors *sensor = _child_sensors[i];
		if (!sensor->get_is_enabled()) {
			continue;
		}
		sensor->evaluate_sensor_value();
	} //endfor sensors

	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_enabled()) {
			continue;
		}
		Status result = btnode->tick(blackboard, delta);
		set_tick_result(result);
		if (btnode->get_internal_status() == BT_INTERNAL_STATUS_COMPLETED) {
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			//emit_signal("btnode_exited", blackboard, delta);
		}
#ifdef DEBUG_ENABLED
		_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
		NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_behaviour_trees_usec(_total_tick_usec);
#endif
		return result;
	} //endfor btnodes
	/**
	for( int i = 0; i < get_child_count(); ++i ) {
		Node* node = get_child(i);
		if( NNSensors* sensor = godot::Object::cast_to<NNSensors>(node) ) {
			if( !sensor-> get_is_enabled() ) {
				continue;
			}
			sensor->evaluate_sensor_value();
			continue;
		}
		if( NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(node) ) {
			if( !btnode->get_is_enabled() ) {
				continue;
			}
			Status result = btnode->tick(blackboard, delta);
			set_tick_result(result);
			if( btnode->get_internal_status() == BT_INTERNAL_STATUS_COMPLETED ) {
				set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
				//emit_signal("btnode_exited", blackboard, delta);
			}
			#ifdef DEBUG_ENABLED
			_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
			NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_behaviour_trees_usec(_total_tick_usec);
			#endif
			return result;
		}
	}
	*/
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
//emit_signal("btnode_exited", blackboard, delta);
#ifdef DEBUG_ENABLED
	_total_tick_usec = godot::Time::get_singleton()->get_ticks_usec() - method_start_time_usec;
	NNPerformanceMonitorSingleton::get_singleton()->increment_total_time_elapsed_behaviour_trees_usec(_total_tick_usec);
#endif
	return Status::FAILURE; // We shouldn't get here. If we do, there were no child nodes.
}

// Godot virtuals.
void NNBTRoot::_notification(int p_what) {
	NNBTNodes::_notification(p_what);

	if (p_what == NOTIFICATION_READY || p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
#ifdef DEBUG_ENABLED
		if (Engine::get_singleton()->is_editor_hint())
			return;
		NNDebuggerOverlay::get_singleton()->register_behaviour_tree(this->get_instance_id());
#endif
		_child_sensors.clear();
		for (int i = 0; i < get_child_count(); ++i) {
			if (NNSensors *sensor = godot::Object::cast_to<NNSensors>(get_child(i))) {
				_child_sensors.push_back(sensor);
			}
		}
		_num_child_sensors = (unsigned int)_child_sensors.size();
		reset();
	}
}
