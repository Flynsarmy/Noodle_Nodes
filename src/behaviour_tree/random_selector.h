#pragma once

#include "composite_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTRandomSelector : public NNBTCompositeNodes {
	GDCLASS(NNBTRandomSelector, NNBTCompositeNodes)

private:
	TypedArray<int> _child_node_order;
	int _current_child_index;

protected:
	static void _bind_methods();

public:
	NNBTRandomSelector();
	~NNBTRandomSelector();

	// Getters and setters for attributes.

	// Handling functions.

	virtual void reset_bt_node() override;

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot
