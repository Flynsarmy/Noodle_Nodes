#pragma once

#include "composite_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTScoreBasedPicker : public NNBTCompositeNodes {
	GDCLASS(NNBTScoreBasedPicker, NNBTCompositeNodes)

private:
	//bool _is_reactive;
	int _current_child_index;

protected:
	static void _bind_methods();

public:
	NNBTScoreBasedPicker();
	~NNBTScoreBasedPicker();

	// Getters and setters for attributes.
	//void set_is_reactive( bool is_reactive );
	//bool get_is_reactive() const;

	// Handling functions.

	virtual void reset_bt_node() override;

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot
