#include "score_based_picker.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTScoreBasedPicker::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_is_reactive", "is_reactive"), &NNBTScoreBasedPicker::set_is_reactive);
	//ClassDB::bind_method(D_METHOD("get_is_reactive"), &NNBTScoreBasedPicker::get_is_reactive);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_reactive", PROPERTY_HINT_NONE), "set_is_reactive","get_is_reactive");

	/*
	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNBTScoreBasedPicker::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNBTScoreBasedPicker::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE ), "set_score","get_score");
	/**/

	//ClassDB::bind_method(D_METHOD("set_reset_rule", "reset_rule"), &NNBTScoreBasedPicker::set_reset_rule);
	//ClassDB::bind_method(D_METHOD("get_reset_rule"), &NNBTScoreBasedPicker::get_reset_rule);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "reset_rule", PROPERTY_HINT_ENUM, "WhenTicked:0,WhenCompleted:1,WhenTickedAfterBeingCompleted:2,Never:3" ), "set_reset_rule","get_reset_rule");
}

// Constructor and destructor.

NNBTScoreBasedPicker::NNBTScoreBasedPicker() {
	_current_child_index = -1;
	//_is_reactive = true;
}

NNBTScoreBasedPicker::~NNBTScoreBasedPicker() {
}

// Handling functions.

// Getters and Setters.
/**
void NNBTScoreBasedPicker::set_is_reactive( bool is_reactive ) {
	_is_reactive = is_reactive;
}

bool NNBTScoreBasedPicker::get_is_reactive() const {
	return _is_reactive;
}
/**/

void NNBTScoreBasedPicker::reset_bt_node() {
	// Evaluate the scores and pick the child with the highest
	// score to run.
	_current_child_index = -1;
	float current_highest_score = -99999999.9999;
	//for( int i = 0; i < get_child_count(); ++i ) {
	//    NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(i));
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		//if( btnode == nullptr ) {
		//    continue;
		//}
		if (!btnode->get_is_enabled()) {
			continue;
		}
		// Evaluate the node to get its score.
		float score = btnode->evaluate();
		if (score > current_highest_score) {
			_current_child_index = i;
			current_highest_score = score;
		}
	} //endfor children
}

NNBTNodes::Status NNBTScoreBasedPicker::tick(Variant blackboard, float delta) {
	if (get_internal_status() == BT_INTERNAL_STATUS_UNTICKED) {
		reset_bt_node();
	}

	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	if (_current_child_index < 0) {
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		set_tick_result(Status::FAILURE);
		//emit_signal("btnode_exited", blackboard, delta);
		return Status::FAILURE; // No valid child found.
	}

	//NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(_current_child_index));
	NNBTNodes *btnode = _child_btnodes[_current_child_index];
	if (btnode != nullptr) {
		Status return_value = btnode->tick(blackboard, delta);
		set_tick_result(return_value);
		//emit_signal("btnode_ticked", blackboard, delta);
		if (return_value == Status::FAILURE || return_value == Status::SUCCESS) {
			_current_child_index = -1;
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			//emit_signal("btnode_exited", blackboard, delta);
		}
		return return_value;
	} //endif node was of correct type
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	set_tick_result(Status::FAILURE);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}
