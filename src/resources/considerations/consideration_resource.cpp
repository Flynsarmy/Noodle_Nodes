#include "consideration_resource.h"
//#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/window.hpp"

using namespace godot;

NNConsiderationResource::NNConsiderationResource() {
	_input_sensor = nullptr;
	set_local_to_scene(true);
}

NNConsiderationResource::~NNConsiderationResource() {
	_input_sensor = nullptr;
}

void NNConsiderationResource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_input_sensor_nodepath", "input_sensor_nodepath"), &NNConsiderationResource::set_input_sensor_nodepath);
	ClassDB::bind_method(D_METHOD("get_input_sensor_nodepath"), &NNConsiderationResource::get_input_sensor_nodepath);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "input_sensor_nodepath", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "NNSensors"), "set_input_sensor_nodepath", "get_input_sensor_nodepath");

	ClassDB::bind_method(D_METHOD("set_activation_curve", "activation_curve"), &NNConsiderationResource::set_activation_curve);
	ClassDB::bind_method(D_METHOD("get_activation_curve"), &NNConsiderationResource::get_activation_curve);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "activation_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_activation_curve", "get_activation_curve");

	ClassDB::bind_method(D_METHOD("sample_activation_curve", "input_value"), &NNConsiderationResource::sample_activation_curve);
}

// Getters and setters.

void NNConsiderationResource::set_input_sensor_nodepath(NodePath input_sensor_nodepath) {
	_input_sensor_nodepath = input_sensor_nodepath;
}

NodePath NNConsiderationResource::get_input_sensor_nodepath() const {
	return _input_sensor_nodepath;
}

void NNConsiderationResource::set_activation_curve(Ref<Curve> activation_curve) {
	_activation_curve = activation_curve;
}

Ref<Curve> NNConsiderationResource::get_activation_curve() const {
	return _activation_curve;
}

// Handling methods.

float NNConsiderationResource::evaluate(bool &has_vetoed, Node *parent_node) {
	// An ugly fix to sensor data fetching, but should work until a better one is found.
	NNSensors *input_sensor = nullptr;
	if (!_input_sensor_nodepath.is_empty()) {
		input_sensor = parent_node->get_node<NNSensors>(_input_sensor_nodepath);
	}
	//if( _input_sensor == nullptr ) {
	// godot::Object::cast_to<NNSensors>(parent_node->get_node_or_null(_input_sensor_nodepath));
	//}

	float input_value = 0.0;
	if (input_sensor != nullptr) {
		input_value = input_sensor->get_sensor_value();
	}

	float score = 0.0;
	if (has_method("eval")) {
		Variant return_value = call("eval", input_value);
		if (return_value.get_type() == Variant::Type::ARRAY) {
			Array retarray = return_value;
			if (retarray.size() > 0) {
				score = retarray[0];
			}
			if (retarray.size() > 1) {
				has_vetoed = retarray[1];
			}
		} else if (return_value.get_type() == Variant::Type::FLOAT) {
			score = return_value;
		}
		return score;
	}

	if (_activation_curve.is_valid()) {
		score = _activation_curve->sample(input_value);
	} else {
		score = input_value;
	}

	return score;
}

float NNConsiderationResource::sample_activation_curve(float input_value) const {
	if (_activation_curve.is_valid()) {
		return _activation_curve->sample(input_value);
	}
	return 0.0; //input_value;
}
