#include "consideration_resources.h"

using namespace godot;

NNConsiderationResources::NNConsiderationResources() {
	_is_active = true;
	//_has_vetoed = false;
}

NNConsiderationResources::~NNConsiderationResources() {
}

void NNConsiderationResources::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_is_active", "is_active"), &NNConsiderationResources::set_is_active);
	ClassDB::bind_method(D_METHOD("get_is_active"), &NNConsiderationResources::get_is_active);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active", PROPERTY_HINT_NONE), "set_is_active", "get_is_active");

	//ClassDB::bind_method(D_METHOD("set_has_vetoed", "has_vetoed"), &NNConsiderationResources::set_has_vetoed);
	//ClassDB::bind_method(D_METHOD("get_has_vetoed"), &NNConsiderationResources::get_has_vetoed);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_vetoed", PROPERTY_HINT_NONE), "set_has_vetoed","get_has_vetoed");
}

// Getters and setters.

void NNConsiderationResources::set_is_active(bool is_active) {
	_is_active = is_active;
}

bool NNConsiderationResources::get_is_active() const {
	return _is_active;
}

/**
void  NNConsiderationResources::set_has_vetoed( bool has_vetoed ) {
	_has_vetoed = has_vetoed;
}
bool  NNConsiderationResources::get_has_vetoed() const {
	return _has_vetoed;
}
/**/

// Handling methods.

float NNConsiderationResources::evaluate(bool &has_vetoed, Node *parent_node) {
	has_vetoed = false;
	return 0.0;
}
