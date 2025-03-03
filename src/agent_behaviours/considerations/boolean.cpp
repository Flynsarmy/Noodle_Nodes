#include "boolean.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBooleanConsideration::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_true_score_value", "true_score_value"), &NNBooleanConsideration::set_true_score_value);
	ClassDB::bind_method(D_METHOD("get_true_score_value"), &NNBooleanConsideration::get_true_score_value);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "true_score_value", PROPERTY_HINT_RANGE, "0.0,1.0,or_less,or_greater"), "set_true_score_value", "get_true_score_value");

	ClassDB::bind_method(D_METHOD("set_false_score_value", "false_score_value"), &NNBooleanConsideration::set_false_score_value);
	ClassDB::bind_method(D_METHOD("get_false_score_value"), &NNBooleanConsideration::get_false_score_value);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "false_score_value", PROPERTY_HINT_RANGE, "0.0,1.0,or_less,or_greater"), "set_false_score_value", "get_false_score_value");

	ClassDB::bind_method(D_METHOD("sample_activation_curve", "input_value"), &NNBooleanConsideration::sample_activation_curve);
}

// Constructor and destructor.

NNBooleanConsideration::NNBooleanConsideration() {
	_true_score_value = 1.0;
	_false_score_value = 0.0;
}

NNBooleanConsideration::~NNBooleanConsideration() {
}

// Getters and Setters.

void NNBooleanConsideration::set_true_score_value(float true_score_value) {
	_true_score_value = true_score_value;
}

float NNBooleanConsideration::get_true_score_value() const {
	return _true_score_value;
}

void NNBooleanConsideration::set_false_score_value(float false_score_value) {
	_false_score_value = false_score_value;
}

float NNBooleanConsideration::get_false_score_value() const {
	return _false_score_value;
}

// Godot virtuals.
/**
void NNBooleanConsideration::_ready() {
	initialize_consideration();
}

/**
void NNBooleanConsideration::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			// Entered the tree.
		} break;
		case NOTIFICATION_EXIT_TREE: {
			//_clear_monitoring();
		} break;
	}
}
/**/

// Handling functions.

void NNBooleanConsideration::_evaluate_consideration() {
	_score = sample_activation_curve(get_activation_input_value());
}

float NNBooleanConsideration::sample_activation_curve(float input_value) const {
	if (input_value < 0.5)
		return _false_score_value;
	return _true_score_value;
}
