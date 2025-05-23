#pragma once

#include "../sensor.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBooleanSensor : public NNSensor {
	GDCLASS(NNBooleanSensor, NNSensor)

private:
	bool _boolean_value;

protected:
	static void _bind_methods();

public:
	NNBooleanSensor();
	~NNBooleanSensor();

	// Handling functions.

	virtual float evaluate_sensor_value() override;

	// Getters and setters for attributes.

	void set_boolean_value(bool boolean_value);
	bool get_boolean_value() const;
};

} //namespace godot
