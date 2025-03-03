#ifndef NNDotProductVector3SearchCriterion_H_INCLUDED
#define NNDotProductVector3SearchCriterion_H_INCLUDED

#include "nqs.h"

namespace godot {

class NNDotProductVector3SearchCriterion : public NNNQSSearchCriteria {
	GDCLASS(NNDotProductVector3SearchCriterion, NNNQSSearchCriteria)

private:
	Vector3 _dot_product_direction_vector;
	float _filtering_value;
	int _filtering_rule;

protected:
	static void _bind_methods();
	virtual void _initialize_criterion() override;

public:
	NNDotProductVector3SearchCriterion();
	~NNDotProductVector3SearchCriterion();

	enum NNDotProductVector3SearchCriteriaFilteringRule {
		LessThan = 0,
		LessOrEqual = 1,
		Equal = 2,
		MoreOrEqual = 3,
		MoreThan = 4,
	};

	// Getters and setters for attributes.

	void set_dot_product_direction_vector(Vector3 dot_product_direction_vector);
	Vector3 get_dot_product_direction_vector() const;

	void set_filtering_value(float filtering_value);
	float get_filtering_value() const;

	void set_filtering_rule(int filtering_rule);
	int get_filtering_rule() const;

	// Handling methods.

	virtual void apply_criterion(Node *node, bool &filter_out, float &score) override;
};

} //namespace godot

#endif
