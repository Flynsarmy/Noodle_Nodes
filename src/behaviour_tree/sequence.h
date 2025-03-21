#pragma once

#include "composite_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTSequence : public NNBTCompositeNodes {
	GDCLASS(NNBTSequence, NNBTCompositeNodes)

private:
	int _current_child_index;

protected:
	static void _bind_methods();

public:
	NNBTSequence();
	~NNBTSequence();

	// Handling functions.

	virtual void reset_bt_node() override;

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot
