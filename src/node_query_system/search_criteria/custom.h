#pragma once

#include "nqs.h"

namespace godot {

class NNCustomSearchCriterion : public NNNQSSearchCriteria {
	GDCLASS(NNCustomSearchCriterion, NNNQSSearchCriteria)

private:
protected:
	static void _bind_methods();

public:
	NNCustomSearchCriterion();
	~NNCustomSearchCriterion();

	// Getters and setters for attributes.

	// Handling methods.

	virtual void apply_criterion(Node *node, bool &filter_out, float &score) override;
};

} //namespace godot
