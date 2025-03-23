#pragma once

#include "ticked_nodes.h"
//#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNSTParallel : public NNSTTickedNodes {
	GDCLASS(NNSTParallel, NNSTTickedNodes)

private:
protected:
	static void _bind_methods();

public:
	NNSTParallel();
	~NNSTParallel();

	// Handling functions.
	virtual void _transition_in() override;
	virtual void _evaluate_child_activations(std::vector<NNSTTickedNodes *> &nodes) override;
};

} //namespace godot
