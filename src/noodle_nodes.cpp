#include "noodle_nodes.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NoodleNodes::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_is_active", "is_active"), &NoodleNodes::set_is_active);
	ClassDB::bind_method(D_METHOD("get_is_active"), &NoodleNodes::get_is_active);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active", PROPERTY_HINT_NONE), "set_is_active", "get_is_active");
}

// Constructor and destructor.

NoodleNodes::NoodleNodes() {
	_is_active = true;
}

NoodleNodes::~NoodleNodes() {
}

// Handling functions.

// Getters and Setters.

void NoodleNodes::set_is_active(bool is_active) {
	_is_active = is_active;
}

bool NoodleNodes::get_is_active() const {
	return _is_active;
}
