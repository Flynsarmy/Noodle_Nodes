#pragma once

#include "consideration_resources.h"

//#include <godot_cpp/classes/resource.hpp>

namespace godot {

class NNConsiderationGroupResource : public NNConsiderationResources {
	GDCLASS(NNConsiderationGroupResource, NNConsiderationResources)

private:
	TypedArray<NNConsiderationResources> _child_nodes;
	int _evaluation_method;
	bool _invert_score;

protected:
	static void _bind_methods();

public:
	NNConsiderationGroupResource();
	~NNConsiderationGroupResource();

	// Getters and setters for attributes.

	void set_evaluation_method(int evaluation_method);
	int get_evaluation_method() const;

	enum NNConsiderationGroupResourceEvaluationMethod {
		Sum = 0,
		Min = 1,
		Max = 2,
		Mean = 3,
		Multiply = 4,
		FirstNonZero = 5,
	};

	void set_invert_score(bool invert_score);
	bool get_invert_score() const;

	void set_child_nodes(TypedArray<NNConsiderationResources> child_nodes);
	TypedArray<NNConsiderationResources> get_child_nodes() const;

	// Handling functions.

	virtual float evaluate(bool &has_vetoed, Node *parent_node) override;
};

} //namespace godot
