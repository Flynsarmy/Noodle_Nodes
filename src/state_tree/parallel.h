#pragma once

#include "nodes.h"
//#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNSTParallel : public NNSTNodes {
	GDCLASS(NNSTParallel, NNSTNodes)

private:
protected:
	static void _bind_methods();

public:
	NNSTParallel();
	~NNSTParallel();

	// Handling functions.
	virtual void evaluate_state_activations(TypedArray<NNSTNodes> *nodes, Variant blackboard, float delta) override;
};

} //namespace godot
