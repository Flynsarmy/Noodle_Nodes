#include "transition.h"

using namespace godot;

// Method binds.

void NNSTTransition::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_to", "to"), &NNSTTransition::set_to);
	ClassDB::bind_method(D_METHOD("get_to"), &NNSTTransition::get_to);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "to", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "NNSTBranchNodes"), "set_to", "get_to");

	ClassDB::bind_method(D_METHOD("set_event_name", "event_name"), &NNSTTransition::set_event_name);
	ClassDB::bind_method(D_METHOD("get_event_name"), &NNSTTransition::get_event_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "event_name", PROPERTY_HINT_NONE), "set_event_name", "get_event_name");

	ClassDB::bind_method(D_METHOD("get_guard"), &NNSTTransition::get_guard);
	ClassDB::bind_method(D_METHOD("set_guard", "guard"), &NNSTTransition::set_guard);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "guard", PROPERTY_HINT_RESOURCE_TYPE, "NNSTGuard"), "set_guard", "get_guard");
}

// Constructor and destructor.

NNSTTransition::NNSTTransition() {
}

NNSTTransition::~NNSTTransition() {
}

// Getters and Setters.
NodePath NNSTTransition::get_to() const {
	return _to;
}

void NNSTTransition::set_to(const NodePath &p_to) {
	_to = p_to;
}

String NNSTTransition::get_event_name() const {
	return _event_name;
}

void NNSTTransition::set_event_name(const String &p_event_name) {
	_event_name = p_event_name;
}

Ref<NNSTGuard> NNSTTransition::get_guard() const {
	return guard;
}

void NNSTTransition::set_guard(const Ref<NNSTGuard> &p_guard) {
	guard = p_guard;
}

// Handling functions.
bool NNSTTransition::_can_transition() {
	if (guard.is_valid()) {
		return guard->is_satisfied(this, Object::cast_to<NNSTTickedNodes>(get_parent()));
	}
	return true;
}

// Godot virtuals.
