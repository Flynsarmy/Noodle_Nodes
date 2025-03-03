#include "sensors.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNSensors::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_use_absolute_value", "use_absolute_value"), &NNSensors::set_use_absolute_value);
	//ClassDB::bind_method(D_METHOD("get_use_absolute_value"), &NNSensors::get_use_absolute_value);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_absolute_value", PROPERTY_HINT_NONE), "set_use_absolute_value","get_use_absolute_value");

	ClassDB::bind_method(D_METHOD("set_sensor_value", "sensor_value"), &NNSensors::set_sensor_value);
	ClassDB::bind_method(D_METHOD("get_sensor_value"), &NNSensors::get_sensor_value);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sensor_value", PROPERTY_HINT_RANGE, "0.0,1.0,or_less,or_greater"), "set_sensor_value", "get_sensor_value");
}

// Constructor and destructor.

NNSensors::NNSensors() {
	_sensor_value = 0.0001f; // We start with a miniscule non-zero value.
	_has_sensor_value_changed = true;
	//_use_absolute_value = true;
}

NNSensors::~NNSensors() {
}

// Godot virtuals.

void NNSensors::_ready() {
	initialize_sensor();
}

void NNSensors::_exit_tree() {
	uninitialize_sensor();
}

// Handling methods.

void NNSensors::initialize_sensor() {
}

float NNSensors::evaluate_sensor_value() {
	_has_sensor_value_changed = false;
	return 0.0f;
}

// Getters and Setters.

void NNSensors::set_sensor_value(float sensor_value) {
	_has_sensor_value_changed = (_sensor_value != sensor_value);
	_sensor_value = sensor_value;
}

float NNSensors::get_sensor_value() const {
	return _sensor_value;
}

/**
void NNSensors::set_use_absolute_value( bool use_absolute_value ) {
	_use_absolute_value = use_absolute_value;
}

bool NNSensors::get_use_absolute_value() const {
	return _use_absolute_value;
}
/**/
