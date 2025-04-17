#include "all_of_guard.h"

using namespace godot;

void NNSTAllOfGuard::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_guards"), &NNSTAllOfGuard::get_guards);
	ClassDB::bind_method(D_METHOD("set_guards", "guards"), &NNSTAllOfGuard::set_guards);
	ADD_PROPERTY(
			PropertyInfo(
					Variant::ARRAY,
					"guards",
					PROPERTY_HINT_ARRAY_TYPE,
					vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "NNSTGuard")),
			"set_guards",
			"get_guards");

	ClassDB::bind_method(D_METHOD("is_satisfied", "p_transition", "p_state"), &NNSTAllOfGuard::is_satisfied);
}

TypedArray<NNSTGuard> NNSTAllOfGuard::get_guards() const {
	return guards;
}

void NNSTAllOfGuard::set_guards(const TypedArray<NNSTGuard> &p_guards) {
	guards = p_guards;
	;
}

bool NNSTAllOfGuard::is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) {
	for (int i = 0; i < guards.size(); i++) {
		Ref<NNSTGuard> guard = guards[i];
		if (guard.is_null()) {
			continue;
		}
		if (!guard->is_satisfied(p_transition, p_state)) {
			return false;
		}
	}

	return true;
}
