#pragma once

#include "nqs.h"

namespace godot {

class NNDistanceToVector3SearchCriterion : public NNNQSSearchCriteria {
	GDCLASS(NNDistanceToVector3SearchCriterion, NNNQSSearchCriteria)

private:
	float _min_distance;
	float _max_distance;

	float _min_distance_squared;
	float _max_distance_squared;
	float _span_length;
	float _one_over_span_length;

	Vector3 _distance_to_vector;

protected:
	static void _bind_methods();
	virtual void _initialize_criterion() override;

public:
	NNDistanceToVector3SearchCriterion();
	~NNDistanceToVector3SearchCriterion();

	// Getters and setters for attributes.

	void set_distance_to_vector(Vector3 distance_to_vector);
	Vector3 get_distance_to_vector() const;

	void set_min_distance(float min_distance);
	float get_min_distance() const;

	void set_max_distance(float max_distance);
	float get_max_distance() const;

	// Handling methods.

	virtual void apply_criterion(Node *node, bool &filter_out, float &score) override;
};

} //namespace godot
