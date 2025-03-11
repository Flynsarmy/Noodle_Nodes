#include "selector.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTSelector::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_is_reactive", "is_reactive"), &NNBTSelector::set_is_reactive);
	//ClassDB::bind_method(D_METHOD("get_is_reactive"), &NNBTSelector::get_is_reactive);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_reactive", PROPERTY_HINT_NONE), "set_is_reactive","get_is_reactive");

	/**
	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNBTSelector::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNBTSelector::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE ), "set_score","get_score");
	/**/

	//ClassDB::bind_method(D_METHOD("set_reset_rule", "reset_rule"), &NNBTSelector::set_reset_rule);
	//ClassDB::bind_method(D_METHOD("get_reset_rule"), &NNBTSelector::get_reset_rule);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "reset_rule", PROPERTY_HINT_ENUM, "WhenTicked:0,WhenCompleted:1,WhenTickedAfterBeingCompleted:2,Never:3" ), "set_reset_rule","get_reset_rule");
}

// Constructor and destructor.

NNBTSelector::NNBTSelector() {
	_current_child_index = 0;
	//_is_reactive = true;
}

NNBTSelector::~NNBTSelector() {
}

// Handling functions.

// Getters and Setters.

/**
void NNBTSelector::set_is_reactive( bool is_reactive ) {
	_is_reactive = is_reactive;
}

bool NNBTSelector::get_is_reactive() const {
	return _is_reactive;
}

/**
void NNBTSelector::set_score( float score ) {
	_score = score;
}

float NNBTSelector::get_score() const {
	return _score;
}
*/

void NNBTSelector::reset_bt_node() {
	_current_child_index = 0;
}

NNBTNodes::Status NNBTSelector::tick(Variant blackboard, float delta) {
	if (get_internal_status() == BT_INTERNAL_STATUS_UNTICKED) {
		reset_bt_node();
	}
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	//emit_signal("btnode_ticked", blackboard, delta);
	//while( _current_child_index < get_child_count() ) {
	//    NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(_current_child_index));
	//if( btnode != nullptr ) {
	while (_current_child_index < (int)_num_child_btnodes) {
		NNBTNodes *btnode = _child_btnodes[_current_child_index];
		if (btnode->get_is_enabled()) {
			Status result = btnode->tick(blackboard, delta);
			set_tick_result(result);
			if (result == Status::SUCCESS) { //&& _current_child_index == get_child_count() - 1 ) {
				set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
				//emit_signal("btnode_exited", blackboard, delta);
				return Status::SUCCESS;
			} else if (result == Status::RUNNING) {
				return Status::RUNNING;
			}
		} //endif is active
		//}//endif node was of correct type
		++_current_child_index;
	} //endwhile children to tick
	//_current_child_index = -1;
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}
