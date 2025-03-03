#include "cooldown_msec.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTCooldownMsec::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_cooldown_msec", "cooldown_msec"), &NNBTCooldownMsec::set_cooldown_msec);
	ClassDB::bind_method(D_METHOD("get_cooldown_msec"), &NNBTCooldownMsec::get_cooldown_msec);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cooldown_msec", PROPERTY_HINT_RANGE, "0,1000,or_greater"), "set_cooldown_msec", "get_cooldown_msec");

	ClassDB::bind_method(D_METHOD("set_cooldown_return_value", "cooldown_return_value"), &NNBTCooldownMsec::set_cooldown_return_value);
	ClassDB::bind_method(D_METHOD("get_cooldown_return_value"), &NNBTCooldownMsec::get_cooldown_return_value);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cooldown_return_value", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1"), "set_cooldown_return_value", "get_cooldown_return_value");

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_cooldown_start_timestamp", "cooldown_start_timestamp"), &NNBTCooldownMsec::set_cooldown_start_timestamp);
	ClassDB::bind_method(D_METHOD("get_cooldown_start_timestamp"), &NNBTCooldownMsec::get_cooldown_start_timestamp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cooldown_start_timestamp", PROPERTY_HINT_RANGE, "0,1000,or_greater"), "set_cooldown_start_timestamp", "get_cooldown_start_timestamp");
}

// Constructor and destructor.

NNBTCooldownMsec::NNBTCooldownMsec() {
	_cooldown_msec = 0;
	_cooldown_start_timestamp = 0;
	_cooldown_return_value = Status::FAILURE;
	_is_in_cooldown = false;
}

NNBTCooldownMsec::~NNBTCooldownMsec() {
}

// Getters and Setters.

void NNBTCooldownMsec::set_cooldown_msec(int cooldown_msec) {
	_cooldown_msec = cooldown_msec;
}

int NNBTCooldownMsec::get_cooldown_msec() const {
	return _cooldown_msec;
}

void NNBTCooldownMsec::set_cooldown_start_timestamp(int cooldown_start_timestamp) {
	_cooldown_start_timestamp = cooldown_start_timestamp;
}

int NNBTCooldownMsec::get_cooldown_start_timestamp() const {
	return _cooldown_start_timestamp;
}

void NNBTCooldownMsec::set_cooldown_return_value(Status cooldown_return_value) {
	_cooldown_return_value = cooldown_return_value;
}

NNBTNodes::Status NNBTCooldownMsec::get_cooldown_return_value() const {
	return _cooldown_return_value;
}

// Handling methods.

NNBTNodes::Status NNBTCooldownMsec::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	if (_is_in_cooldown) {
		uint64_t wait_time = godot::Time::get_singleton()->get_ticks_msec() - _cooldown_start_timestamp;
		if (wait_time < _cooldown_msec) {
			return _cooldown_return_value;
		}
		_is_in_cooldown = false;
	}

	for (int i = 0; i < get_child_count(); ++i) {
		Node *node = get_child(i);
		if (NNBTNodes *btnode = godot::Object::cast_to<NNBTNodes>(node)) {
			if (!btnode->get_is_active()) {
				continue;
			}
			Status result = btnode->tick(blackboard, delta);

			if (result != Status::RUNNING) {
				_is_in_cooldown = true;
				_cooldown_start_timestamp = godot::Time::get_singleton()->get_ticks_msec();

				set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
				set_tick_result(result);
				//emit_signal("btnode_exited", blackboard, delta);
			} else {
				set_internal_status(BT_INTERNAL_STATUS_TICKED);
				set_tick_result(result);
			}

			//emit_signal("btnode_exited", blackboard, delta);
			return result;
		}
	}
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	set_tick_result(Status::FAILURE);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}
