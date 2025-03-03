#include "boolean.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBooleanSensor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_boolean_value", "boolean_value"), &NNBooleanSensor::set_boolean_value);
	ClassDB::bind_method(D_METHOD("get_boolean_value"), &NNBooleanSensor::get_boolean_value);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "boolean_value", PROPERTY_HINT_NONE), "set_boolean_value", "get_boolean_value");
}

// Constructor and destructor.

NNBooleanSensor::NNBooleanSensor() {
	_boolean_value = false;
}

NNBooleanSensor::~NNBooleanSensor() {
}

// Handling functions.

float NNBooleanSensor::evaluate_sensor_value() {
	// Here the absolute and relative are the same, so no calculation needed.
	set_sensor_value((float)_boolean_value);
	return get_sensor_value();
}

// Getters and Setters.

void NNBooleanSensor::set_boolean_value(bool boolean_value) {
	_has_sensor_value_changed = (_boolean_value != boolean_value);
	_boolean_value = boolean_value;
}

bool NNBooleanSensor::get_boolean_value() const {
	return _boolean_value;
}
