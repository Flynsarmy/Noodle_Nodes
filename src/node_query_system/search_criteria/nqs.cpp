#include "nqs.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

NNNQSSearchCriteria::NNNQSSearchCriteria() {
	_is_filtered = true;
	_use_for_scoring = true;

	_use_for_filtering = false;
	_score = 0.0;
}

NNNQSSearchCriteria::~NNNQSSearchCriteria() {
}

void NNNQSSearchCriteria::_ready() {
	//if( !get_is_enabled() ) return;
	if (Engine::get_singleton()->is_editor_hint())
		return;

	_initialize_criterion();
}

void NNNQSSearchCriteria::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_use_for_scoring", "use_for_scoring"), &NNNQSSearchCriteria::set_use_for_scoring);
	ClassDB::bind_method(D_METHOD("get_use_for_scoring"), &NNNQSSearchCriteria::get_use_for_scoring);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_for_scoring", PROPERTY_HINT_NONE), "set_use_for_scoring", "get_use_for_scoring");

	ClassDB::bind_method(D_METHOD("set_use_for_filtering", "use_for_filtering"), &NNNQSSearchCriteria::set_use_for_filtering);
	ClassDB::bind_method(D_METHOD("get_use_for_filtering"), &NNNQSSearchCriteria::get_use_for_filtering);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_for_filtering", PROPERTY_HINT_NONE), "set_use_for_filtering", "get_use_for_filtering");

	ClassDB::bind_method(D_METHOD("set_activation_curve", "activation_curve"), &NNNQSSearchCriteria::set_activation_curve);
	ClassDB::bind_method(D_METHOD("get_activation_curve"), &NNNQSSearchCriteria::get_activation_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "activation_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_activation_curve", "get_activation_curve");

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNNQSSearchCriteria::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNNQSSearchCriteria::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE), "set_score", "get_score");

	ClassDB::bind_method(D_METHOD("set_is_filtered", "is_filtered"), &NNNQSSearchCriteria::set_is_filtered);
	ClassDB::bind_method(D_METHOD("get_is_filtered"), &NNNQSSearchCriteria::get_is_filtered);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_filtered", PROPERTY_HINT_NONE), "set_is_filtered", "get_is_filtered");
}

// Getters and setters.

void NNNQSSearchCriteria::set_activation_curve(Ref<Curve> activation_curve) {
	_activation_curve = activation_curve;
}

Ref<Curve> NNNQSSearchCriteria::get_activation_curve() const {
	return _activation_curve;
}

void NNNQSSearchCriteria::set_is_filtered(bool is_filtered) {
	_is_filtered = is_filtered;
}

bool NNNQSSearchCriteria::get_is_filtered() const {
	return _is_filtered;
}

void NNNQSSearchCriteria::set_use_for_scoring(bool use_for_scoring) {
	_use_for_scoring = use_for_scoring;
}

bool NNNQSSearchCriteria::get_use_for_scoring() const {
	return _use_for_scoring;
}

void NNNQSSearchCriteria::set_use_for_filtering(bool use_for_filtering) {
	_use_for_filtering = use_for_filtering;
}

bool NNNQSSearchCriteria::get_use_for_filtering() const {
	return _use_for_filtering;
}

void NNNQSSearchCriteria::set_score(float score) {
	_score = score;
}

float NNNQSSearchCriteria::get_score() const {
	return _score;
}

// Handing methods.

void NNNQSSearchCriteria::apply_criterion(Node *node, bool &filter_out, float &score) {
	//if( node == nullptr ) return;
}

float NNNQSSearchCriteria::sample_activation_curve(float input_value) const {
	if (_activation_curve.is_valid()) {
		return _activation_curve->sample(input_value);
	}
	return input_value; //0.0f;
}
