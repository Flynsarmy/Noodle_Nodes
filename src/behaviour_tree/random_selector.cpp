#include "random_selector.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTRandomSelector::_bind_methods() {
}

// Constructor and destructor.

NNBTRandomSelector::NNBTRandomSelector() {
	_current_child_index = 0;
}

NNBTRandomSelector::~NNBTRandomSelector() {
}

// Getters and Setters.

void NNBTRandomSelector::reset_bt_node() {
	_current_child_index = 0;
	// Create a random order.
	_child_node_order.clear();
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_enabled()) {
			continue;
		}
		_child_node_order.push_back(i);
	}
	_child_node_order.shuffle();
}

// Handling functions.

NNBTNodes::Status NNBTRandomSelector::tick(Variant blackboard, float delta) {
	if (get_internal_status() == BT_INTERNAL_STATUS_UNTICKED) {
		reset_bt_node();
	}

	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	//emit_signal("btnode_ticked", blackboard, delta);
	while (_current_child_index < _child_node_order.size()) {
		//NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(_child_node_order[_current_child_index]));
		NNBTNodes *btnode = _child_btnodes[(int)_child_node_order[_current_child_index]];
		//if( btnode != nullptr ) {
		//if( !btnode->get_is_enabled() ) {
		//    continue;
		//}
		Status result = btnode->tick(blackboard, delta);
		if (result == Status::SUCCESS) {
			//_current_child_index = -1;
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			//emit_signal("btnode_exited", blackboard, delta);
			return Status::SUCCESS;
		} else if (result == Status::RUNNING) {
			return Status::RUNNING;
		}
		//}//endif node was of correct type
		++_current_child_index;
	} //endwhile children to tick
	//_current_child_index = -1;
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}
