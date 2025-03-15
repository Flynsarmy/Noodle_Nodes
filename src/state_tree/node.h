#pragma once

#include "nodes.h"

namespace godot {

class NNSTNode : public NNSTNodes {
	GDCLASS(NNSTNode, NNSTNodes)

private:
protected:
	static void _bind_methods();

public:
	NNSTNode();
	~NNSTNode();
};

} //namespace godot
