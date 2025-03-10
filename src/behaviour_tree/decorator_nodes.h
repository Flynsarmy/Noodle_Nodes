#pragma once

#include "task_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTDecoratorNodes : public NNBTTaskNodes {
	GDCLASS(NNBTDecoratorNodes, NNBTTaskNodes)

private:
protected:
	static void _bind_methods();

public:
	NNBTDecoratorNodes();
	~NNBTDecoratorNodes();
};

} //namespace godot
