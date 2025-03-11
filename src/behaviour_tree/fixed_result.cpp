#include "fixed_result.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTFixedResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_fixed_result", "fixed_result"), &NNBTFixedResult::set_fixed_result);
	ClassDB::bind_method(D_METHOD("get_fixed_result"), &NNBTFixedResult::get_fixed_result);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "fixed_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1"), "set_fixed_result", "get_fixed_result");

	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTFixedResult::tick);
}

// Constructor and destructor.

NNBTFixedResult::NNBTFixedResult() {
	_fixed_result = Status::SUCCESS;
}

NNBTFixedResult::~NNBTFixedResult() {
}

// Getters and Setters.

void NNBTFixedResult::set_fixed_result(Status fixed_result) {
	_fixed_result = fixed_result;
}

NNBTNodes::Status NNBTFixedResult::get_fixed_result() const {
	return _fixed_result;
}

// Handling methods.

NNBTNodes::Status NNBTFixedResult::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	set_tick_result(_fixed_result);
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_enabled()) {
			continue;
		}
		btnode->tick(blackboard, delta);
		//emit_signal("btnode_ticked", blackboard, delta);
		break;
	}
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	//emit_signal("btnode_exited", blackboard, delta);
	return _fixed_result;
}
