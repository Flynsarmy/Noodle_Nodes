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
	virtual void _transition_in() override;
	virtual void _evaluate_child_activations(std::vector<NNSTNode *> &nodes) override;
};

} //namespace godot
