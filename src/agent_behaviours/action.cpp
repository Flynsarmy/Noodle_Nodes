#include "action.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNAction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_action_id", "action_id"), &NNAction::set_action_id);
	ClassDB::bind_method(D_METHOD("get_action_id"), &NNAction::get_action_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "action_id", PROPERTY_HINT_NONE), "set_action_id", "get_action_id");

	// Signals.

	ADD_SIGNAL(MethodInfo("action_entered"));
	ADD_SIGNAL(MethodInfo("physics_frame_tick", PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("idle_frame_tick", PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("action_exited"));
}

// Constructor and destructor.

NNAction::NNAction() {
	_action_id = 0;
}

NNAction::~NNAction() {
}

// Handling functions.

bool NNAction::start_action() {
	set_is_finished(false);
	set_has_failed(false);
	emit_signal("action_entered");
	return true;
}

bool NNAction::end_action() {
	emit_signal("action_exited");
	set_is_finished(false);
	set_has_failed(false);
	return true;
}

Node *NNAction::step_actions() {
	if (get_is_finished()) {
		set_is_finished(false);
		return nullptr;
	}
	return this;
}

// Getters and Setters.

void NNAction::set_action_id(int action_id) {
	_action_id = action_id;
}

int NNAction::get_action_id() const {
	return _action_id;
}
