#ifndef UTILITYAIINTRANGESENSOR_H_INCLUDED
#define UTILITYAIINTRANGESENSOR_H_INCLUDED

#include "../sensor.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNIntRangeSensor : public NNSensor {
	GDCLASS(NNIntRangeSensor, NNSensor)

private:
	int _range_min_value;
	int _range_max_value;
	int _range_length;
	float _one_over_range_length;
	int _range_value;

protected:
	static void _bind_methods();

public:
	NNIntRangeSensor();
	~NNIntRangeSensor();

	// Handling functions.

	virtual float evaluate_sensor_value() override;

	// Getters and setters for attributes.

	void set_range_min_value(int range_min_value);
	int get_range_min_value() const;

	void set_range_max_value(int range_max_value);
	int get_range_max_value() const;

	void set_range_value(int range_value);
	int get_range_value() const;

	//void   set_range_length( int range_length );
	//int get_range_length() const;
};

} //namespace godot

#endif
