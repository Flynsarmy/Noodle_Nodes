#include "state_is_active_guard.h"

using namespace godot;

void NNSTStateIsActiveGuard::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_state"), &NNSTStateIsActiveGuard::get_state);
	ClassDB::bind_method(D_METHOD("set_state", "guard"), &NNSTStateIsActiveGuard::set_state);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "state", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "NNSTTickedNodes"), "set_state", "get_state");

	ClassDB::bind_method(D_METHOD("is_satisfied", "p_transition", "p_state"), &NNSTStateIsActiveGuard::is_satisfied);
}

NodePath NNSTStateIsActiveGuard::get_state() const {
	return _state_path;
}

void NNSTStateIsActiveGuard::set_state(const NodePath &p_to) {
	_state_path = p_to;
}

bool NNSTStateIsActiveGuard::is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) {
	if (_state_path.is_empty()) {
		return true;
	}

	NNSTTickedNodes *guard_state = p_transition->get_node<NNSTTickedNodes>(_state_path);
	if (guard_state == nullptr) {
		print_error("State path is not valid. Please check the path to the state node.");
		return false;
	}

	return guard_state->get_is_active();
}
