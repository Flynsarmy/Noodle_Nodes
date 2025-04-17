#pragma once

#include "guard.h"

namespace godot {

class NNSTAnyOfGuard : public NNSTGuard {
	GDCLASS(NNSTAnyOfGuard, NNSTGuard)

protected:
	static void _bind_methods();
	TypedArray<NNSTGuard> guards;

public:
	virtual bool is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) override;
	TypedArray<NNSTGuard> get_guards() const;
	void set_guards(const TypedArray<NNSTGuard> &p_guards);
};

} //namespace godot
