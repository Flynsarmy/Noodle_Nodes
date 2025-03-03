#include "pass_by.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTPassBy::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_tick_result", "tick_result"), &NNBTPassBy::set_tick_result);
	//ClassDB::bind_method(D_METHOD("get_tick_result"), &NNBTPassBy::get_tick_result);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "tick_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1" ), "set_tick_result","get_tick_result");

	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTPassBy::tick);
}

// Constructor and destructor.

NNBTPassBy::NNBTPassBy() {
	//_tick_result = 1;
	_has_on_tick_method = false;
}

NNBTPassBy::~NNBTPassBy() {
}

// Getters and Setters.
/*
void NNBTPassBy::set_tick_result( int tick_result ) {
	_tick_result = tick_result;
	if( _tick_result > 1 ) {
		_tick_result = 1;
	} else if (_tick_result < -1 ) {
		_tick_result = -1;
	}
}


int  NNBTPassBy::get_tick_result() const {
	return _tick_result;
}
/**/

// Handling methods.

NNBTNodes::Status NNBTPassBy::tick(Variant blackboard, float delta) {
	// The passBy node just calls its tick and then ticks the first
	// behaviour tree node child and returns the result of the child.
	// Otherwise it returns what ever is set as the tick result property.
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	if (_has_on_tick_method) {
		call("on_tick", blackboard, delta);
	}
	//emit_signal("btnode_ticked", blackboard, delta);
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::SUCCESS;
}

// Godot virtuals.

void NNBTPassBy::_notification(int p_what) {
	if (p_what == NOTIFICATION_POST_ENTER_TREE) {
		_has_on_tick_method = has_method("on_tick");
	} else if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		update_child_vectors();
	}
}
