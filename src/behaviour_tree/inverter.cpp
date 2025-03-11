#include "inverter.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTInverter::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTInverter::tick);
}

// Constructor and destructor.

NNBTInverter::NNBTInverter() {
}

NNBTInverter::~NNBTInverter() {
}

// Getters and Setters.

// Handling methods.

NNBTNodes::Status NNBTInverter::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_enabled()) {
			continue;
		}
		Status result = btnode->tick(blackboard, delta);

		if (result == Status::SUCCESS) {
			result = Status::FAILURE;
		} else if (result == Status::FAILURE) {
			result = Status::SUCCESS;
		}

		set_tick_result(result);
		//emit_signal("btnode_ticked", blackboard, delta);
		if (result != Status::RUNNING) {
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			//emit_signal("btnode_exited", blackboard, delta);
		}
		return result;
	}
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	set_tick_result(Status::FAILURE);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}
