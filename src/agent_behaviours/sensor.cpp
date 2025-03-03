#include "sensor.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNSensor::_bind_methods() {
}

// Constructor and destructor.

NNSensor::NNSensor() {
}

NNSensor::~NNSensor() {
}

// Handling functions.

float NNSensor::evaluate_sensor_value() {
	_has_sensor_value_changed = false;
	return get_sensor_value();
}

// Getters and Setters.

// Godot virtuals.
