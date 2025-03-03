#include "random_sequence.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTRandomSequence::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_is_reactive", "is_reactive"), &NNBTRandomSequence::set_is_reactive);
	//ClassDB::bind_method(D_METHOD("get_is_reactive"), &NNBTRandomSequence::get_is_reactive);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_reactive", PROPERTY_HINT_NONE), "set_is_reactive","get_is_reactive");

	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTRandomSequence::tick);

	//ClassDB::bind_method(D_METHOD("set_reset_rule", "reset_rule"), &NNBTRandomSequence::set_reset_rule);
	//ClassDB::bind_method(D_METHOD("get_reset_rule"), &NNBTRandomSequence::get_reset_rule);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "reset_rule", PROPERTY_HINT_ENUM, "WhenTicked:0,WhenCompleted:1,WhenTickedAfterBeingCompleted:2,Never:3" ), "set_reset_rule","get_reset_rule");
}

// Constructor and destructor.

NNBTRandomSequence::NNBTRandomSequence() {
	_current_child_index = 0;
	//_is_reactive = true;
}

NNBTRandomSequence::~NNBTRandomSequence() {
}

// Handling functions.

// Getters and Setters.

/**
void NNBTRandomSequence::set_is_reactive( bool is_reactive ) {
	_is_reactive = is_reactive;
}

bool NNBTRandomSequence::get_is_reactive() const {
	return _is_reactive;
}
/**/

void NNBTRandomSequence::reset_bt_node() {
	_current_child_index = 0;
	// Create a random order.
	_child_node_order.clear();
	//for( int i = 0; i < get_child_count(); ++i ) {
	//    NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(i));
	//    if( btnode == nullptr ) continue;
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_active()) {
			continue;
		}
		_child_node_order.push_back(i);
	}
	_child_node_order.shuffle();
}

NNBTNodes::Status NNBTRandomSequence::tick(Variant blackboard, float delta) {
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
		//if( btnode != nullptr ) {
		//if( !btnode->get_is_active() ) {
		//    continue;
		//}
		NNBTNodes *btnode = _child_btnodes[(int)_child_node_order[_current_child_index]];
		Status result = btnode->tick(blackboard, delta);
		set_tick_result(result);
		if (result == Status::FAILURE) {
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			//emit_signal("btnode_exited", blackboard, delta);
			return Status::FAILURE;
		} else if (result == Status::RUNNING) {
			return Status::RUNNING;
		}
		//}//endif node was of correct type
		++_current_child_index;
	} //endwhile children to tick
	//_current_child_index = -1;
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	set_tick_result(Status::SUCCESS);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::SUCCESS;
}
