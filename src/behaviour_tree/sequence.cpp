#include "sequence.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTSequence::_bind_methods() {
}

// Constructor and destructor.

NNBTSequence::NNBTSequence() {
	_current_child_index = 0;
}

NNBTSequence::~NNBTSequence() {
}

// Handling methods.

void NNBTSequence::reset_bt_node() {
	_current_child_index = 0;
}

NNBTNodes::Status NNBTSequence::tick(Variant blackboard, float delta) {
	if (get_internal_status() == BT_INTERNAL_STATUS_UNTICKED) {
		_current_child_index = 0;
		//emit_signal("btree_node_entered", blackboard, delta, this);
	}
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	//emit_signal("btnode_ticked", blackboard, delta);
	//int num_children = get_child_count();
	//while( _current_child_index < num_children ) {
	//    NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(_current_child_index));
	//    if( btnode != nullptr ) {
	while (_current_child_index < (int)_num_child_btnodes) {
		NNBTNodes *btnode = _child_btnodes[_current_child_index];
		if (btnode->get_is_enabled()) {
			Status result = btnode->tick(blackboard, delta);
			set_tick_result(result);
			if (result == Status::FAILURE) {
				set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
				//emit_signal("btnode_exited", blackboard, delta);
				return Status::FAILURE;
			} else if (result == Status::RUNNING) {
				return Status::RUNNING;
			}
		} //endif is active
		//}//endif node was of correct type
		++_current_child_index;
	} //endwhile children to tick
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::SUCCESS;
}
