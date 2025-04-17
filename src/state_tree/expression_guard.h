#pragma once

#include "guard.h"

namespace godot {

class NNSTExpressionGuard : public NNSTGuard {
	GDCLASS(NNSTExpressionGuard, NNSTGuard)

protected:
	static void _bind_methods();
	String expression;

public:
	virtual bool is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) override;
	String get_expression() const;
	void set_expression(const String &p_expression);
};

} //namespace godot
