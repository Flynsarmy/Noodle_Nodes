#include "distance_to_node3d.h"
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

NNDistanceToNode3DSearchCriterion::NNDistanceToNode3DSearchCriterion() {
	_min_distance = 0.0f;
	_max_distance = 100.0f;
	_min_distance_squared = 0.0f;
	_max_distance_squared = _max_distance * _max_distance;
	_span_length = _max_distance_squared - _min_distance_squared;
	_one_over_span_length = 1.0f / _span_length;
	_distance_to = nullptr;
}

NNDistanceToNode3DSearchCriterion::~NNDistanceToNode3DSearchCriterion() {
	_distance_to = nullptr;
}

void NNDistanceToNode3DSearchCriterion::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_distance_to", "distance_to"), &NNDistanceToNode3DSearchCriterion::set_distance_to);
	ClassDB::bind_method(D_METHOD("get_distance_to"), &NNDistanceToNode3DSearchCriterion::get_distance_to);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "distance_to", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_distance_to", "get_distance_to");

	ClassDB::bind_method(D_METHOD("set_min_distance", "min_distance"), &NNDistanceToNode3DSearchCriterion::set_min_distance);
	ClassDB::bind_method(D_METHOD("get_min_distance"), &NNDistanceToNode3DSearchCriterion::get_min_distance);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_distance", PROPERTY_HINT_NONE), "set_min_distance", "get_min_distance");

	ClassDB::bind_method(D_METHOD("set_max_distance", "max_distance"), &NNDistanceToNode3DSearchCriterion::set_max_distance);
	ClassDB::bind_method(D_METHOD("get_max_distance"), &NNDistanceToNode3DSearchCriterion::get_max_distance);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance", PROPERTY_HINT_NONE), "set_max_distance", "get_max_distance");
}

void NNDistanceToNode3DSearchCriterion::_initialize_criterion() {
}

// Getters and setters.

void NNDistanceToNode3DSearchCriterion::set_distance_to(Node3D *distance_to) {
	_distance_to = distance_to;
}

Node3D *NNDistanceToNode3DSearchCriterion::get_distance_to() const {
	return _distance_to;
}

void NNDistanceToNode3DSearchCriterion::set_min_distance(float min_distance) {
	if (_min_distance < 0.0f)
		return;
	if (_min_distance >= _max_distance)
		return;

	_min_distance = min_distance;
	_min_distance_squared = _min_distance * _min_distance;

	_span_length = _max_distance_squared - _min_distance_squared;
	_one_over_span_length = 1.0f / _span_length;
}

float NNDistanceToNode3DSearchCriterion::get_min_distance() const {
	return _min_distance;
}

void NNDistanceToNode3DSearchCriterion::set_max_distance(float max_distance) {
	if (max_distance <= _min_distance)
		return;
	_max_distance = max_distance;
	_max_distance_squared = _max_distance * _max_distance;

	_span_length = _max_distance_squared - _min_distance_squared;
	_one_over_span_length = 1.0f / _span_length;
}

float NNDistanceToNode3DSearchCriterion::get_max_distance() const {
	return _max_distance;
}

// Handing methods.

void NNDistanceToNode3DSearchCriterion::apply_criterion(Node *node, bool &filter_out, float &score) {
	if (_distance_to == nullptr)
		return;
	Node3D *node3d = godot::Object::cast_to<Node3D>(node);
	if (node3d == nullptr)
		return;

	_is_filtered = false;
	_score = 1.0f;

	Vector3 from_to = node3d->get_global_position() - _distance_to->get_global_position();
	float distance_squared = from_to.length_squared();

	if (get_use_for_filtering()) {
		_is_filtered = (distance_squared < _min_distance_squared || distance_squared > _max_distance_squared);
	} //endif do filtering

	if (get_use_for_scoring()) {
		if (distance_squared >= _max_distance_squared) {
			_score = 1.0f;
		} else if (distance_squared <= _min_distance_squared) {
			_score = 0.0f;
		} else {
			float distance_from_lower_limit = distance_squared - _min_distance_squared;
			_score = distance_from_lower_limit * _one_over_span_length;
		}
		if (get_activation_curve().is_valid()) {
			_score = sample_activation_curve(_score);
		}
	} //endif do scoring

	filter_out = _is_filtered;
	score = _score;
}