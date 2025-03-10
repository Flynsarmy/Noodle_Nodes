#pragma once

#include "decorator_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTFixedResult : public NNBTDecoratorNodes {
	GDCLASS(NNBTFixedResult, NNBTDecoratorNodes)

private:
	Status _fixed_result;

protected:
	static void _bind_methods();

public:
	NNBTFixedResult();
	~NNBTFixedResult();

	// Getters and setters for attributes.

	void set_fixed_result(Status fixed_result);
	Status get_fixed_result() const;

	// Handling functions.

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot
