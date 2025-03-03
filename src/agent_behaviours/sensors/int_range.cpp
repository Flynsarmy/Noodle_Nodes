#include "int_range.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNIntRangeSensor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_range_value", "range_value"), &NNIntRangeSensor::set_range_value);
	ClassDB::bind_method(D_METHOD("get_range_value"), &NNIntRangeSensor::get_range_value);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "range_value", PROPERTY_HINT_NONE), "set_range_value", "get_range_value");

	ClassDB::bind_method(D_METHOD("set_range_min_value", "range_min_value"), &NNIntRangeSensor::set_range_min_value);
	ClassDB::bind_method(D_METHOD("get_range_min_value"), &NNIntRangeSensor::get_range_min_value);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "range_min_value", PROPERTY_HINT_NONE), "set_range_min_value", "get_range_min_value");

	ClassDB::bind_method(D_METHOD("set_range_max_value", "range_max_value"), &NNIntRangeSensor::set_range_max_value);
	ClassDB::bind_method(D_METHOD("get_range_max_value"), &NNIntRangeSensor::get_range_max_value);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "range_max_value", PROPERTY_HINT_NONE), "set_range_max_value", "get_range_max_value");
}

// Constructor and destructor.

NNIntRangeSensor::NNIntRangeSensor() {
	_range_max_value = 100;
	_range_min_value = 0;
	_range_value = 0;
	_range_length = 100;
	_one_over_range_length = 1.0 / (float)_range_length;
}

NNIntRangeSensor::~NNIntRangeSensor() {
}

// Handling functions.

float NNIntRangeSensor::evaluate_sensor_value() {
	/**
	if( get_use_absolute_value() ) {
		// Absolute value, so make sure that it is within the interval.
		int range_result = _range_value;
		if( range_result < _range_min_value ) {
			range_result = _range_min_value;
		}else if( range_result > _range_max_value ) {
			range_result = _range_max_value;
		}
		set_sensor_value((float)range_result);
		return get_sensor_value();
	}
	/**/
	// Relative value, so calculate the position within the interval.
	float range_result = ((float)(_range_value - _range_min_value)) * _one_over_range_length;
	if (range_result < 0.0) {
		range_result = 0.0;
	} else if (range_result > 1.0) {
		range_result = 1.0;
	}

	set_sensor_value(range_result);
	return get_sensor_value();
}

// Getters and Setters.

void NNIntRangeSensor::set_range_min_value(int range_min_value) {
	if (_range_min_value == _range_max_value) {
		return;
	}
	_range_min_value = range_min_value;
	_range_length = _range_max_value - _range_min_value;
	if (_range_length != 0) {
		_one_over_range_length = 1.0 / (float)_range_length;
	} else {
		_one_over_range_length = 0.0;
	}
}

int NNIntRangeSensor::get_range_min_value() const {
	return _range_min_value;
}

void NNIntRangeSensor::set_range_max_value(int range_max_value) {
	if (_range_min_value == _range_max_value) {
		return;
	}
	_range_max_value = range_max_value;
	_range_length = _range_max_value - _range_min_value;
	if (_range_length != 0) {
		_one_over_range_length = 1.0 / (float)_range_length;
	} else {
		_one_over_range_length = 0.0;
	}
}

int NNIntRangeSensor::get_range_max_value() const {
	return _range_max_value;
}

void NNIntRangeSensor::set_range_value(int range_value) {
	_range_value = range_value;
	if (_range_value < _range_min_value) {
		_range_value = _range_min_value;
	} else if (_range_value > _range_max_value) {
		_range_value = _range_max_value;
	}
}

int NNIntRangeSensor::get_range_value() const {
	return _range_value;
}
