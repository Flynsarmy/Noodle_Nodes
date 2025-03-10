#pragma once

#include "task_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTNodeReference : public NNBTTaskNodes {
	GDCLASS(NNBTNodeReference, NNBTTaskNodes)

private:
	//int _tick_result;
	NNBTNodes *_node_reference;
	ObjectID _cache;

protected:
	static void _bind_methods();

	void _update_cache();

public:
	NNBTNodeReference();
	~NNBTNodeReference();

	// Getters and setters for attributes.

	void set_node_reference(NNBTNodes *node_reference);
	NNBTNodes *get_node_reference() const;
	//void set_tick_result( int tick_result );
	//int  get_tick_result() const;

	// Handling functions.
	virtual void reset() override;
	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot
