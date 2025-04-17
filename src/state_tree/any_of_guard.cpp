#include "any_of_guard.h"

using namespace godot;

void NNSTAnyOfGuard::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_guards"), &NNSTAnyOfGuard::get_guards);
	ClassDB::bind_method(D_METHOD("set_guards", "guards"), &NNSTAnyOfGuard::set_guards);
	ADD_PROPERTY(
			PropertyInfo(
					Variant::ARRAY,
					"guards",
					PROPERTY_HINT_ARRAY_TYPE,
					vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "NNSTGuard")),
			"set_guards",
			"get_guards");

	ClassDB::bind_method(D_METHOD("is_satisfied", "p_transition", "p_state"), &NNSTAnyOfGuard::is_satisfied);
}

TypedArray<NNSTGuard> NNSTAnyOfGuard::get_guards() const {
	return guards;
}

void NNSTAnyOfGuard::set_guards(const TypedArray<NNSTGuard> &p_guards) {
	guards = p_guards;
	;
}

bool NNSTAnyOfGuard::is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) {
	int64_t size = guards.size();

	for (int i = 0; i < size; i++) {
		Ref<NNSTGuard> guard = guards[i];
		if (guard.is_null()) {
			continue;
		}
		if (guard->is_satisfied(p_transition, p_state)) {
			return true;
		}
	}

	// We have guards but they all failed
	if (size > 0) {
		return false;
	}

	return true;
}
