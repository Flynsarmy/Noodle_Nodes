#include "distance_vector3.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNDistanceVector3Sensor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_from_vector", "from_vector"), &NNDistanceVector3Sensor::set_from_vector3);
	ClassDB::bind_method(D_METHOD("get_from_vector"), &NNDistanceVector3Sensor::get_from_vector3);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "from_vector", PROPERTY_HINT_NONE), "set_from_vector", "get_from_vector");

	ClassDB::bind_method(D_METHOD("set_to_vector", "to_vector"), &NNDistanceVector3Sensor::set_to_vector3);
	ClassDB::bind_method(D_METHOD("get_to_vector"), &NNDistanceVector3Sensor::get_to_vector3);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "to_vector", PROPERTY_HINT_NONE), "set_to_vector", "get_to_vector");

	ClassDB::bind_method(D_METHOD("set_max_distance", "max_distance"), &NNDistanceVector3Sensor::set_max_distance);
	ClassDB::bind_method(D_METHOD("get_max_distance"), &NNDistanceVector3Sensor::get_max_distance);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance", PROPERTY_HINT_RANGE, "0.00000001,1000.0,or_greater"), "set_max_distance", "get_max_distance");

	ClassDB::bind_method(D_METHOD("set_is_distance_calculated", "is_distance_calculated"), &NNDistanceVector3Sensor::set_is_distance_calculated);
	ClassDB::bind_method(D_METHOD("get_is_distance_calculated"), &NNDistanceVector3Sensor::get_is_distance_calculated);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_distance_calculated", PROPERTY_HINT_RANGE, "0.00000001,1000.0,or_greater"), "set_is_distance_calculated", "get_is_distance_calculated");

	ClassDB::bind_method(D_METHOD("set_is_direction_vector_calculated", "is_direction_vector_calculated"), &NNDistanceVector3Sensor::set_is_direction_vector_calculated);
	ClassDB::bind_method(D_METHOD("get_is_direction_vector_calculated"), &NNDistanceVector3Sensor::get_is_direction_vector_calculated);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_direction_vector_calculated", PROPERTY_HINT_RANGE, "0.00000001,1000.0,or_greater"), "set_is_direction_vector_calculated", "get_is_direction_vector_calculated");

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_from_to_vector", "from_to_vector"), &NNDistanceVector3Sensor::set_from_to_vector3);
	ClassDB::bind_method(D_METHOD("get_from_to_vector"), &NNDistanceVector3Sensor::get_from_to_vector3);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "from_to_vector", PROPERTY_HINT_NONE), "set_from_to_vector", "get_from_to_vector");

	ClassDB::bind_method(D_METHOD("set_direction_vector", "direction_vector"), &NNDistanceVector3Sensor::set_direction_vector3);
	ClassDB::bind_method(D_METHOD("get_direction_vector"), &NNDistanceVector3Sensor::get_direction_vector3);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "direction_vector", PROPERTY_HINT_NONE), "set_direction_vector", "get_direction_vector");

	ClassDB::bind_method(D_METHOD("set_distance", "distance"), &NNDistanceVector3Sensor::set_distance);
	ClassDB::bind_method(D_METHOD("get_distance"), &NNDistanceVector3Sensor::get_distance);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance", PROPERTY_HINT_RANGE, "0.00000001,1000.0,or_greater"), "set_distance", "get_distance");

	ClassDB::bind_method(D_METHOD("set_distance_squared", "distance_squared"), &NNDistanceVector3Sensor::set_distance_squared);
	ClassDB::bind_method(D_METHOD("get_distance_squared"), &NNDistanceVector3Sensor::get_distance_squared);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_squared", PROPERTY_HINT_RANGE, "0.00000001,1000.0,or_greater"), "set_distance_squared", "get_distance_squared");
}

// Constructor and destructor.

NNDistanceVector3Sensor::NNDistanceVector3Sensor() {
	_distance = 0.0f;
	_distance_squared = 0.0f;
	_max_distance = 100.0f;
	_max_distance_squared = _max_distance * _max_distance;
	_one_over_max_distance_squared = 1.0f / _max_distance_squared;

	_is_distance_calculated = false;
	_is_direction_vector_calculated = false;
}

NNDistanceVector3Sensor::~NNDistanceVector3Sensor() {
}

// Handling functions.

float NNDistanceVector3Sensor::evaluate_sensor_value() {
	_from_to_vector = _from_vector - _to_vector;
	_distance_squared = _from_to_vector.length_squared();
	if (_is_distance_calculated || _is_direction_vector_calculated) {
		_distance = godot::Math::sqrt(_distance_squared);
	}
	if (_is_direction_vector_calculated) {
		_direction_vector = _from_to_vector / _distance;
	}
	/**
	if( get_use_absolute_value() ) {
		if( _is_distance_calculated ) {
			set_sensor_value(_distance );
		} else {
			set_sensor_value(_distance_squared );
		}
	} else {
	/**/
	set_sensor_value(_distance_squared * _one_over_max_distance_squared);
	/*}*/
	return get_sensor_value();
}

// Getters and Setters.

// Configuration values.

void NNDistanceVector3Sensor::set_to_vector3(Vector3 to) {
	_to_vector = to;
}

Vector3 NNDistanceVector3Sensor::get_to_vector3() const {
	return _to_vector;
}

void NNDistanceVector3Sensor::set_from_vector3(Vector3 from) {
	_from_vector = from;
}

Vector3 NNDistanceVector3Sensor::get_from_vector3() const {
	return _from_vector;
}

void NNDistanceVector3Sensor::set_max_distance(float max_distance) {
	_max_distance = max_distance;
	_max_distance_squared = _max_distance * _max_distance;
	_one_over_max_distance_squared = 1.0 / _max_distance_squared;
}

float NNDistanceVector3Sensor::get_max_distance() const {
	return _max_distance;
}

void NNDistanceVector3Sensor::set_is_distance_calculated(bool is_distance_calculated) {
	_is_distance_calculated = is_distance_calculated;
}

bool NNDistanceVector3Sensor::get_is_distance_calculated() const {
	return _is_distance_calculated;
}

void NNDistanceVector3Sensor::set_is_direction_vector_calculated(bool is_direction_vector_calculated) {
	_is_direction_vector_calculated = is_direction_vector_calculated;
}

bool NNDistanceVector3Sensor::get_is_direction_vector_calculated() const {
	return _is_direction_vector_calculated;
}

// Debugging / current values.

void NNDistanceVector3Sensor::set_from_to_vector3(Vector3 from_to) {
	_from_to_vector = from_to;
}

Vector3 NNDistanceVector3Sensor::get_from_to_vector3() const {
	return _from_to_vector;
}

void NNDistanceVector3Sensor::set_direction_vector3(Vector3 direction) {
	_direction_vector = direction;
}

Vector3 NNDistanceVector3Sensor::get_direction_vector3() const {
	return _direction_vector;
}

void NNDistanceVector3Sensor::set_distance(float distance) {
	_distance = distance;
}

float NNDistanceVector3Sensor::get_distance() const {
	return _distance;
}

void NNDistanceVector3Sensor::set_distance_squared(float distance_squared) {
	_distance_squared = distance_squared;
}

float NNDistanceVector3Sensor::get_distance_squared() const {
	return _distance_squared;
}
