#pragma once

#include "root.h"
#include "transition.h"

namespace godot {

class NNSTGuard : public Resource {
	GDCLASS(NNSTGuard, Resource)

protected:
	static void _bind_methods();

public:
	virtual bool is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state);

	Variant evaluate_expression(String context, NNSTRoot *state_chart, String expression, Variant default_value) const;
	String path_of(Node *node) const;
};

} //namespace godot
