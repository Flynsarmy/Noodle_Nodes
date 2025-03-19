#pragma once

#include "noodle_nodes.h"

namespace godot {

class NNSTNodes : public NoodleNodes {
	GDCLASS(NNSTNodes, NoodleNodes)

private:
protected:
	static void _bind_methods();

public:
	NNSTNodes();
	~NNSTNodes();

	// Getters and setters for attributes.

	// Handling functions.
};

} //namespace godot
