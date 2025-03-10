#pragma once

#include "sensors.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNSensor : public NNSensors {
	GDCLASS(NNSensor, NNSensors)

private:
protected:
	static void _bind_methods();

public:
	NNSensor();
	~NNSensor();

	// Handling functions.

	virtual float evaluate_sensor_value() override;

	// Getters and setters for attributes.
};

} //namespace godot
