#include "metadata.h"

using namespace godot;

NNMetadataSearchCriterion::NNMetadataSearchCriterion() {
}

NNMetadataSearchCriterion::~NNMetadataSearchCriterion() {
}

void NNMetadataSearchCriterion::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_metadata_name", "metadata_name"), &NNMetadataSearchCriterion::set_metadata_name);
	ClassDB::bind_method(D_METHOD("get_metadata_name"), &NNMetadataSearchCriterion::get_metadata_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "metadata_name", PROPERTY_HINT_NONE), "set_metadata_name", "get_metadata_name");

	ClassDB::bind_method(D_METHOD("set_metadata_value", "metadata_value"), &NNMetadataSearchCriterion::set_metadata_value);
	ClassDB::bind_method(D_METHOD("get_metadata_value"), &NNMetadataSearchCriterion::get_metadata_value);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "metadata_value", PROPERTY_HINT_NONE), "set_metadata_value", "get_metadata_value");
}

// Getters and setters.

void NNMetadataSearchCriterion::set_metadata_name(StringName metadata_name) {
	_metadata_name = metadata_name;
}

StringName NNMetadataSearchCriterion::get_metadata_name() const {
	return _metadata_name;
}

void NNMetadataSearchCriterion::set_metadata_value(Variant metadata_value) {
	_metadata_value = metadata_value;
}

Variant NNMetadataSearchCriterion::get_metadata_value() const {
	return _metadata_value;
}

void NNMetadataSearchCriterion::apply_criterion(Node *node, bool &filter_out, float &score) {
	_score = 1.0f;
	_is_filtered = false;
	godot::Variant metavar = node->get_meta(_metadata_name);
	_is_filtered = (metavar != _metadata_value);
	filter_out = _is_filtered;
	score = _score;
}