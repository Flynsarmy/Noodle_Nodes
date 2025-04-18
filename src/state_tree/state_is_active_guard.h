#pragma once

#include "guard.h"
#include "ticked_nodes.h"

namespace godot {

class NNSTStateIsActiveGuard : public NNSTGuard {
	GDCLASS(NNSTStateIsActiveGuard, NNSTGuard)

protected:
	static void _bind_methods();
	NodePath _state_path;

public:
	NodePath get_state() const;
	void set_state(const NodePath &p_state);

	virtual bool is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) override;
};

} //namespace godot
