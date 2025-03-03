#ifndef NNBTInverter_H_INCLUDED
#define NNBTInverter_H_INCLUDED

#include "decorator_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTInverter : public NNBTDecoratorNodes {
	GDCLASS(NNBTInverter, NNBTDecoratorNodes)

private:
	//int _tick_result;

protected:
	static void _bind_methods();

public:
	NNBTInverter();
	~NNBTInverter();

	// Getters and setters for attributes.

	//void set_tick_result( int tick_result );
	//int  get_tick_result() const;

	// Handling functions.

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot

#endif
