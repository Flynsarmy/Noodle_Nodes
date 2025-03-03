#include "considerations.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNConsiderations::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_has_vetoed", "has_vetoed"), &NNConsiderations::set_has_vetoed);
	ClassDB::bind_method(D_METHOD("get_has_vetoed"), &NNConsiderations::get_has_vetoed);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_vetoed", PROPERTY_HINT_NONE), "set_has_vetoed", "get_has_vetoed");

	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNConsiderations::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNConsiderations::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE), "set_score", "get_score");
}

// Constructor and destructor.

NNConsiderations::NNConsiderations() {
	_has_vetoed = false;
	_score = 0.0f;
}

NNConsiderations::~NNConsiderations() {
}

// Handling functions.

// Getters and Setters.

void NNConsiderations::set_has_vetoed(bool has_vetoed) {
	_has_vetoed = has_vetoed;
}

bool NNConsiderations::get_has_vetoed() const {
	return _has_vetoed;
}

void NNConsiderations::set_score(float score) {
	_score = score;
}

float NNConsiderations::get_score() const {
	return _score;
}

float NNConsiderations::evaluate() {
	return 0.0f;
}
