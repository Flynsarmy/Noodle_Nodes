#include "not_guard.h"

using namespace godot;

void NNSTNotGuard::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_guard"), &NNSTNotGuard::get_guard);
	ClassDB::bind_method(D_METHOD("set_guard", "guard"), &NNSTNotGuard::set_guard);
	ADD_PROPERTY(
			PropertyInfo(
					Variant::ARRAY,
					"guard",
					PROPERTY_HINT_RESOURCE_TYPE,
					"NNSTGuard"),
			"set_guard",
			"get_guard");

	ClassDB::bind_method(D_METHOD("is_satisfied", "p_transition", "p_state"), &NNSTNotGuard::is_satisfied);
}

void NNSTNotGuard::set_guard(const Ref<NNSTGuard> &p_guard) {
	_guard = p_guard;
}

bool NNSTNotGuard::is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) {
	if (!_guard.is_valid()) {
		return true;
	}

	return !_guard->is_satisfied(p_transition, p_state);
}
