#include "actions.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNActions::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_is_finished", "is_finished"), &NNActions::set_is_finished);
	ClassDB::bind_method(D_METHOD("get_is_finished"), &NNActions::get_is_finished);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_finished", PROPERTY_HINT_NONE), "set_is_finished", "get_is_finished");

	ClassDB::bind_method(D_METHOD("set_has_failed", "has_failed"), &NNActions::set_has_failed);
	ClassDB::bind_method(D_METHOD("get_has_failed"), &NNActions::get_has_failed);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_failed", PROPERTY_HINT_NONE), "set_has_failed", "get_has_failed");
}

// Constructor and destructor.

NNActions::NNActions() {
	_is_finished = false;
	_has_failed = false;
}

NNActions::~NNActions() {
}

// Handling functions.

bool NNActions::start_action() {
	_is_finished = false;
	_has_failed = false;
	//_has_previous_step_failed = false;
	return true;
}

bool NNActions::end_action() {
	_is_finished = false;
	_has_failed = false;
	//_has_previous_step_failed = false;
	return true;
}

Node *NNActions::step_actions() {
	return nullptr;
}

// Getters and Setters.

void NNActions::set_is_finished(bool is_finished) {
	_is_finished = is_finished;
}

bool NNActions::get_is_finished() const {
	return _is_finished;
}

void NNActions::set_has_failed(bool has_failed) {
	_has_failed = has_failed;
}

bool NNActions::get_has_failed() const {
	return _has_failed;
}

/**
void NNActions::set_has_previous_step_failed( bool has_previous_step_failed ) {
	_has_previous_step_failed = has_previous_step_failed;
}

bool NNActions::get_has_previous_step_failed() const {
	return _has_previous_step_failed;
}
/**/
