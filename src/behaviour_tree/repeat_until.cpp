#include "repeat_until.h"
#include "nodes.h"
//#include "../sensors.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTRepeatUntil::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_max_repeat_times", "max_repeat_times"), &NNBTRepeatUntil::set_max_repeat_times);
	ClassDB::bind_method(D_METHOD("get_max_repeat_times"), &NNBTRepeatUntil::get_max_repeat_times);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_repeat_times", PROPERTY_HINT_RANGE, "-1,100,or_greater"), "set_max_repeat_times", "get_max_repeat_times");

	ClassDB::bind_method(D_METHOD("set_expected_tick_result", "expected_tick_result"), &NNBTRepeatUntil::set_expected_tick_result);
	ClassDB::bind_method(D_METHOD("get_expected_tick_result"), &NNBTRepeatUntil::get_expected_tick_result);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "expected_tick_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1"), "set_expected_tick_result", "get_expected_tick_result");

	//ADD_SUBGROUP("Debugging","");

	//ClassDB::bind_method(D_METHOD("set_current_repeat_times", "current_repeat_times"), &NNBTRepeatUntil::set_current_max_repeat_times);
	ClassDB::bind_method(D_METHOD("get_current_repeat_times"), &NNBTRepeatUntil::get_current_max_repeat_times);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "current_repeat_times", PROPERTY_HINT_RANGE, "-1,100,or_greater"), "set_current_repeat_times","get_current_repeat_times");

	//ClassDB::bind_method(D_METHOD("set_reset_rule", "reset_rule"), &NNBTRepeatUntil::set_reset_rule);
	//ClassDB::bind_method(D_METHOD("get_reset_rule"), &NNBTRepeatUntil::get_reset_rule);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "reset_rule", PROPERTY_HINT_ENUM, "WhenTicked:0,WhenCompleted:1,WhenTickedAfterBeingCompleted:2,Never:3" ), "set_reset_rule","get_reset_rule");
}

// Constructor and destructor.

NNBTRepeatUntil::NNBTRepeatUntil() {
	_max_repeat_times = -1;
	_current_max_repeat_times = -1;
	_expected_tick_result = Status::SUCCESS;
	set_reset_rule(NNBTNodesResetRule::NEVER);
}

NNBTRepeatUntil::~NNBTRepeatUntil() {
}

// Getters and Setters.

void NNBTRepeatUntil::set_max_repeat_times(int max_repeat_times) {
	_max_repeat_times = max_repeat_times;
}

int NNBTRepeatUntil::get_max_repeat_times() const {
	return _max_repeat_times;
}

void NNBTRepeatUntil::set_current_max_repeat_times(int current_max_repeat_times) {
	_current_max_repeat_times = get_max_repeat_times() - current_max_repeat_times;
}

int NNBTRepeatUntil::get_current_max_repeat_times() const {
	return get_max_repeat_times() - _current_max_repeat_times;
}

void NNBTRepeatUntil::set_expected_tick_result(Status expected_tick_result) {
	_expected_tick_result = expected_tick_result;
}

NNBTNodes::Status NNBTRepeatUntil::get_expected_tick_result() const {
	return _expected_tick_result;
}

// Handling functions.

void NNBTRepeatUntil::reset_bt_node() {
	_current_max_repeat_times = _max_repeat_times;
	_is_expected_tick_result_reached = false;
}

NNBTNodes::Status NNBTRepeatUntil::tick(Variant blackboard, float delta) {
	//if( !get_is_active() ) return Status::FAILURE;
	if (Engine::get_singleton()->is_editor_hint())
		return Status::FAILURE;

	if (get_internal_status() == BT_INTERNAL_STATUS_UNTICKED) {
		reset_bt_node();
	}

	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	if (_is_expected_tick_result_reached) {
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		set_tick_result(Status::FAILURE);
		return Status::FAILURE;
	}
	//emit_signal("btnode_ticked", blackboard, delta);
	//for( int i = 0; i < get_child_count(); ++i ) {
	//    if( NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(i)) ) {
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_active()) {
			continue;
		}
		if (_current_max_repeat_times > 0) {
			--_current_max_repeat_times;
		}
		Status result = btnode->tick(blackboard, delta);
		if (result == _expected_tick_result) {
			_is_expected_tick_result_reached = true;
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			set_tick_result(Status::SUCCESS);
			//emit_signal("btnode_exited", blackboard, delta);
			return Status::SUCCESS;
		}
		if (_current_max_repeat_times > 0 || _current_max_repeat_times < 0) {
			set_tick_result(Status::RUNNING);
			return Status::RUNNING;
		} else if (_current_max_repeat_times == 0) {
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			set_tick_result(Status::SUCCESS);
			//emit_signal("btnode_exited", blackboard, delta);
			return Status::SUCCESS;
		}
		//}
	}
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	set_tick_result(Status::FAILURE);
	return Status::FAILURE;
}
