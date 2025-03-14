#pragma once

#include "composite_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTRandomSequence : public NNBTCompositeNodes {
	GDCLASS(NNBTRandomSequence, NNBTCompositeNodes)

private:
	//bool _is_reactive;
	int _current_child_index;
	TypedArray<int> _child_node_order;

protected:
	static void _bind_methods();

public:
	NNBTRandomSequence();
	~NNBTRandomSequence();

	// Getters and setters for attributes.
	//void set_is_reactive( bool is_reactive );
	//bool get_is_reactive() const;

	// Handling functions.

	virtual void reset_bt_node() override;

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot
