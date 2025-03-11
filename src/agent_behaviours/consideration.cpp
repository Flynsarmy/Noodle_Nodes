#include "consideration.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNConsideration::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_input_sensor", "input_sensor"), &NNConsideration::set_input_sensor);
	ClassDB::bind_method(D_METHOD("get_input_sensor"), &NNConsideration::get_input_sensor);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_sensor", PROPERTY_HINT_NODE_TYPE, "NNSensors"), "set_input_sensor", "get_input_sensor");

	ClassDB::bind_method(D_METHOD("set_activation_curve", "activation_curve"), &NNConsideration::set_activation_curve);
	ClassDB::bind_method(D_METHOD("get_activation_curve"), &NNConsideration::get_activation_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "activation_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_activation_curve", "get_activation_curve");

	ClassDB::bind_method(D_METHOD("set_activation_input_value", "activation_input_value"), &NNConsideration::set_activation_input_value);
	ClassDB::bind_method(D_METHOD("get_activation_input_value"), &NNConsideration::get_activation_input_value);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "activation_input_value", PROPERTY_HINT_RANGE, "0.0,1.0,or_less,or_greater"), "set_activation_input_value", "get_activation_input_value");

	ClassDB::bind_method(D_METHOD("sample_activation_curve", "input_value"), &NNConsideration::sample_activation_curve);
}

// Constructor and destructor.

NNConsideration::NNConsideration() {
	_activation_input_value = 0.0001f;
	_input_sensor = nullptr;
	_has_custom_evaluation_method = false;
	_has_activation_input_changed = true;
}

NNConsideration::~NNConsideration() {
	_input_sensor = nullptr;
}

// Getters and Setters.

void NNConsideration::set_input_sensor(NNSensors *input_sensor) {
	_has_activation_input_changed = _has_activation_input_changed || (_input_sensor != input_sensor);
	_input_sensor = input_sensor;
}

NNSensors *NNConsideration::get_input_sensor() const {
	return _input_sensor;
}

void NNConsideration::set_activation_curve(Ref<Curve> activation_curve) {
	_has_activation_input_changed = _has_activation_input_changed || (_activation_curve != activation_curve);
	_activation_curve = activation_curve;
}

Ref<Curve> NNConsideration::get_activation_curve() const {
	return _activation_curve;
}

void NNConsideration::set_activation_input_value(float activation_input_value) {
	_has_activation_input_changed = _has_activation_input_changed || (_activation_input_value != activation_input_value);
	_activation_input_value = activation_input_value;
}

float NNConsideration::get_activation_input_value() const {
	return _activation_input_value;
}

/**
void NNConsideration::_notification(int p_what) {
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

// Handling methods.

float NNConsideration::evaluate() {
#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
	//if( !get_is_enabled() ) return 0.0;
	//if( Engine::get_singleton()->is_editor_hint() ) return 0.0;
	if (get_has_vetoed())
		return 0.0f;

	if (_input_sensor != nullptr) {
		// If there's no change to the sensor input value, just return the
		// earlier result.
		//if( !_input_sensor->get_has_sensor_value_changed() ) {
		//    return _score;
		//}
		float input_value = _input_sensor->get_sensor_value();
		if (input_value != _activation_input_value || _score == 0.0f) {
			_activation_input_value = input_value;
			_has_activation_input_changed = true;
		}
	}

	if (!_has_activation_input_changed) {
		return _score;
	}
	//_score = 0.0f;
	_evaluate_consideration();
	_has_activation_input_changed = false;
	return _score;
}

float NNConsideration::sample_activation_curve(float input_value) const {
	if (_activation_curve.is_valid()) {
		return _activation_curve->sample(input_value);
	}
	return 0.0f;
}

void NNConsideration::_evaluate_consideration() {
	// If the consideration has been extended with a custom evaluation method,
	// that is used instead of sampling the curve.
	if (_has_custom_evaluation_method) {
		call("eval");
		return;
	}

	if (_activation_curve.is_valid()) {
		_score = _activation_curve->sample(_activation_input_value);
	} else {
		_score = _activation_input_value;
	}
}

void NNConsideration::_notification(int p_what) {
	if (p_what != NOTIFICATION_POST_ENTER_TREE) {
		return;
	}

	_has_custom_evaluation_method = has_method("eval");
}
