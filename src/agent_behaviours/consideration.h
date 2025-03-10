#pragma once

#include "considerations.h"
#include "sensors.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNConsideration : public NNConsiderations {
	GDCLASS(NNConsideration, NNConsiderations)

private:
protected:
	static void _bind_methods();

	NNSensors *_input_sensor;
	Ref<Curve> _activation_curve;
	float _activation_input_value;
	bool _has_custom_evaluation_method;
	bool _has_activation_input_changed;

	virtual void _evaluate_consideration();

public:
	NNConsideration();
	~NNConsideration();

	// Getters and setters for attributes.

	void set_input_sensor(NNSensors *input_sensor);
	virtual NNSensors *get_input_sensor() const;

	void set_activation_curve(Ref<Curve> activation_curve);
	Ref<Curve> get_activation_curve() const;

	void set_activation_input_value(float activation_input_value);
	float get_activation_input_value() const;

	// Godot virtuals.
	virtual void _notification(int p_what);

	// Handling functions.

	virtual float evaluate() override;
	virtual float sample_activation_curve(float input_value) const;
};

} //namespace godot
