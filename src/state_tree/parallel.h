#pragma once

#include "node.h"
//#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNSTParallel : public NNSTNode {
	GDCLASS(NNSTParallel, NNSTNode)

private:
protected:
	static void _bind_methods();

public:
	NNSTParallel();
	~NNSTParallel();

	// Handling functions.
	// virtual void evaluate_state_activations(TypedArray<NNSTTaskNodes> *nodes, Variant blackboard, float delta) override;
	virtual TypedArray<NNSTNode> _evaluate_child_activations(Variant blackboard, float delta) override;
};

} //namespace godot
