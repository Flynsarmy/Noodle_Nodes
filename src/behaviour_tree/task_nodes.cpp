#include "task_nodes.h"

using namespace godot;

NNBTTaskNodes::NNBTTaskNodes() {
}

NNBTTaskNodes::~NNBTTaskNodes() {
}

void NNBTTaskNodes::_bind_methods() {
	GDVIRTUAL_BIND(on_tick, "blackboard", "delta");
}
