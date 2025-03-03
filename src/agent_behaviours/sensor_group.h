#ifndef NNSensorGroup_H_INCLUDED
#define NNSensorGroup_H_INCLUDED

#include "sensors.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNSensorGroup : public NNSensors {
	GDCLASS(NNSensorGroup, NNSensors)

private:
	int _evaluation_method;
	bool _invert_sensor_value;

protected:
	static void _bind_methods();

public:
	NNSensorGroup();
	~NNSensorGroup();

	// Getters and setters for attributes.

	void set_evaluation_method(int evaluation_method);
	int get_evaluation_method() const;

	enum NNSensorGroupEvaluationMethod {
		Sum = 0,
		Min = 1,
		Max = 2,
		Mean = 3,
		Multiply = 4,
		FirstNonZero = 5,
	};

	void set_invert_sensor_value(bool invert_sensor_value);
	bool get_invert_sensor_value() const;

	// Handling functions.

	virtual float evaluate_sensor_value() override;
};

} //namespace godot

#endif
