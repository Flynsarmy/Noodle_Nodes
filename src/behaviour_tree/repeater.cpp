#include "repeater.h"
#include "nodes.h"
//#include "../sensors.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTRepeater::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_repeat_times", "repeat_times"), &NNBTRepeater::set_repeat_times);
	ClassDB::bind_method(D_METHOD("get_repeat_times"), &NNBTRepeater::get_repeat_times);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "repeat_times", PROPERTY_HINT_RANGE, "-1,100,or_greater"), "set_repeat_times", "get_repeat_times");

	// ClassDB::bind_method(D_METHOD("set_times_repeated", "current_repeat_times"), &NNBTRepeater::set_times_repeated);
	ClassDB::bind_method(D_METHOD("get_times_repeated"), &NNBTRepeater::get_times_repeated);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "current_repeat_times", PROPERTY_HINT_NONE ), "set_times_repeated","get_times_repeated");
	//ClassDB::bind_method(D_METHOD("set_reset_rule", "reset_rule"), &NNBTRepeater::set_reset_rule);
	//ClassDB::bind_method(D_METHOD("get_reset_rule"), &NNBTRepeater::get_reset_rule);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "reset_rule", PROPERTY_HINT_ENUM, "WhenTicked:0,WhenCompleted:1,WhenTickedAfterBeingCompleted:2,Never:3" ), "set_reset_rule","get_reset_rule");
}

// Constructor and destructor.

NNBTRepeater::NNBTRepeater() {
	_repeat_times = -1;
	_times_repeated = 0;
}

NNBTRepeater::~NNBTRepeater() {
}

// Getters and Setters.

void NNBTRepeater::set_repeat_times(int repeat_times) {
	_repeat_times = repeat_times;
}

int NNBTRepeater::get_repeat_times() const {
	return _repeat_times;
}

void NNBTRepeater::set_times_repeated(int current_repeat_times) {
	_times_repeated = current_repeat_times;
}

int NNBTRepeater::get_times_repeated() const {
	return _times_repeated;
}

/**
void NNBTRepeater::set_tick_result( int tick_result ) {
	_tick_result = tick_result;
	if( _tick_result > 1 ) {
		_tick_result = 1;
	} else if (_tick_result < -1 ) {
		_tick_result = -1;
	}
}


int  NNBTRepeater::get_tick_result() const {
	return _tick_result;
}
/**/

// Handling functions.

void NNBTRepeater::reset_bt_node() {
	_times_repeated = 0;
}

NNBTNodes::Status NNBTRepeater::tick(Variant blackboard, float delta) {
	if (!get_is_active() || _child_btnodes.size() == 0)
		return Status::FAILURE;
	if (Engine::get_singleton()->is_editor_hint())
		return Status::FAILURE;

	if (get_internal_status() == BT_INTERNAL_STATUS_UNTICKED) {
		reset_bt_node();
		set_internal_status(BT_INTERNAL_STATUS_TICKED);
	} else if (get_internal_status() == BT_INTERNAL_STATUS_COMPLETED) {
		set_internal_status(BT_INTERNAL_STATUS_UNTICKED);
		set_tick_result(Status::SUCCESS);
		return Status::SUCCESS;
	}

	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	// if (_times_repeated == 0) {
	// 	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	// 	set_tick_result(Status::SUCCESS);
	// 	return Status::SUCCESS;
	// }

	Status result = Status::FAILURE;

	//emit_signal("btnode_ticked",blackboard, delta);
	//for( int i = 0; i < get_child_count(); ++i ) {
	//    if( NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(i)) ) {
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_active()) {
			continue;
		}

		result = btnode->tick(blackboard, delta);

		break;
	}

	if (result == Status::RUNNING) {
		set_tick_result(result);
		return result;
	}

	if (result == Status::FAILURE) {
		set_internal_status(BT_INTERNAL_STATUS_UNTICKED);
		set_tick_result(result);
		return result;
	}

	_times_repeated++;

	if (_repeat_times == -1 || _times_repeated < _repeat_times) {
		set_tick_result(Status::RUNNING);
		return Status::RUNNING;
	}

	set_internal_status(BT_INTERNAL_STATUS_UNTICKED);
	set_tick_result(Status::FAILURE);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}
