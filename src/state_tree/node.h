#pragma once

#include "ticked_nodes.h"
#include "transition.h"

namespace godot {

class NNSTNode : public NNSTTickedNodes {
	GDCLASS(NNSTNode, NNSTTickedNodes)

private:
	NodePath _initial_state;

protected:
	static void _bind_methods();

public:
	NNSTNode();
	~NNSTNode();

	// Getters and setters for attributes.
	void set_initial_state(const NodePath &initial_state);
	NodePath get_initial_state() const;

	// Handling functions.
	virtual void _evaluate_child_activations(std::vector<NNSTTickedNodes *> &nodes) override;
	virtual void _transition_in() override;
};

} //namespace godot
