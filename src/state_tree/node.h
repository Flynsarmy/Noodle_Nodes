#pragma once

#include "ticked_nodes.h"
#include "transition.h"

namespace godot {

class NNSTNode : public NNSTTickedNodes {
	GDCLASS(NNSTNode, NNSTTickedNodes)

private:
protected:
	static void _bind_methods();

public:
	NNSTNode();
	~NNSTNode();

	// Getters and setters for attributes.

	// Handling functions.
};

} //namespace godot
