#include "custom_property.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNCustomPropertyConsideration::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_node_with_property", "node_with_property"), &NNCustomPropertyConsideration::set_node_with_property);
	ClassDB::bind_method(D_METHOD("get_node_with_property"), &NNCustomPropertyConsideration::get_node_with_property);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "node_with_property", PROPERTY_HINT_NODE_TYPE, "Node"), "set_node_with_property", "get_node_with_property");

	ClassDB::bind_method(D_METHOD("set_property_name", "property_name"), &NNCustomPropertyConsideration::set_property_name);
	ClassDB::bind_method(D_METHOD("get_property_name"), &NNCustomPropertyConsideration::get_property_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "property_name", PROPERTY_HINT_NONE), "set_property_name", "get_property_name");

	ClassDB::bind_method(D_METHOD("set_property_max_value", "property_max_value"), &NNCustomPropertyConsideration::set_property_max_value);
	ClassDB::bind_method(D_METHOD("get_property_max_value"), &NNCustomPropertyConsideration::get_property_max_value);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "property_max_value", PROPERTY_HINT_NONE), "set_property_max_value", "get_property_max_value");

	ClassDB::bind_method(D_METHOD("sample_activation_curve", "input_value"), &NNCustomPropertyConsideration::sample_activation_curve);
}

// Constructor and destructor.

NNCustomPropertyConsideration::NNCustomPropertyConsideration() {
	_property_max_value = 1.0;
	_one_over_property_max_value = 1.0;
	_node_with_property = nullptr;
}

NNCustomPropertyConsideration::~NNCustomPropertyConsideration() {
}

// Getters and Setters.

void NNCustomPropertyConsideration::set_activation_curve(Ref<Curve> activation_curve) {
	_activation_curve = activation_curve;
}

Ref<Curve> NNCustomPropertyConsideration::get_activation_curve() const {
	return _activation_curve;
}

void NNCustomPropertyConsideration::set_node_with_property(Node *node_with_property) {
	_node_with_property = node_with_property;
}
Node *NNCustomPropertyConsideration::get_node_with_property() const {
	return _node_with_property;
}

void NNCustomPropertyConsideration::set_property_max_value(float property_max_value) {
	if (_property_max_value == 0.0)
		return;
	_property_max_value = property_max_value;
	_one_over_property_max_value = 1.0 / _property_max_value;
}

float NNCustomPropertyConsideration::get_property_max_value() const {
	return _property_max_value;
}

void NNCustomPropertyConsideration::set_property_name(StringName property_name) {
	_property_name = property_name;
}

StringName NNCustomPropertyConsideration::get_property_name() const {
	return _property_name;
}

// Godot virtuals.

void NNCustomPropertyConsideration::_ready() {
	if (Engine::get_singleton()->is_editor_hint())
		return;

	initialize_consideration();
}

/**
void NNCustomPropertyConsideration::_notification(int p_what) {
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

void NNCustomPropertyConsideration::initialize_consideration() {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	if (_property_max_value != 0.0) {
		_one_over_property_max_value = _property_max_value;
	}
}

// Handling functions.

/**
void NNCustomPropertyConsideration::_evaluate_consideration() {
	//_score = sample_activation_curve(get_activation_input_value());
}
/**/

float NNCustomPropertyConsideration::evaluate() {
	if (Engine::get_singleton()->is_editor_hint())
		return 0.0;
	if (!get_is_enabled())
		return 0.0;
	if (_node_with_property == nullptr)
		return 0.0;
	if (get_has_vetoed())
		return 0.0;

	_property_variant_value = _node_with_property->get(_property_name);
	//.can_convert(_property_variant_value.get_type(), godot::Variant::FLOAT)
	_score = (float)_property_variant_value * _one_over_property_max_value;

	if (_activation_curve.is_valid()) {
		_score = _activation_curve->sample(_score);
	}
	return _score;
}

float NNCustomPropertyConsideration::sample_activation_curve(float input_value) const {
	if (_activation_curve.is_valid()) {
		return _activation_curve->sample(input_value);
	}
	return 0.0;
}
