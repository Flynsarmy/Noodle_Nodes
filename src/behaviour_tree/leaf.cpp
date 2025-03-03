#include "leaf.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTLeaf::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_tick_result", "tick_result"), &NNBTLeaf::set_tick_result);
	//ClassDB::bind_method(D_METHOD("get_tick_result"), &NNBTLeaf::get_tick_result);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "tick_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1" ), "set_tick_result","get_tick_result");
	//ADD_SIGNAL(MethodInfo("ticked", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta"), PropertyInfo(Variant::OBJECT, "leaf_node")));
}

// Constructor and destructor.

NNBTLeaf::NNBTLeaf() {}

NNBTLeaf::~NNBTLeaf() {
}

// Getters and Setters.
/**
void NNBTLeaf::set_tick_result( int tick_result ) {
	_tick_result = tick_result;
	if( _tick_result > 1 ) {
		_tick_result = 1;
	} else if (_tick_result < -1 ) {
		_tick_result = -1;
	}
}


int  NNBTLeaf::get_tick_result() const {
	return _tick_result;
}
/**/

// Handling methods.

NNBTNodes::Status NNBTLeaf::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	godot::Variant result;
	result = call("on_tick", blackboard, delta);
	Status return_value = static_cast<Status>((int64_t)result); // or result.operator int64_t();

	set_tick_result(return_value);

	//emit_signal("btnode_ticked", blackboard, delta);
	if (return_value == Status::FAILURE || return_value == Status::SUCCESS) {
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		//emit_signal("btnode_exited", blackboard, delta);
	}

	return return_value;
}

// Godot virtuals.

void NNBTLeaf::_notification(int p_what) {
	if (p_what != NOTIFICATION_POST_ENTER_TREE)
		return;
}
