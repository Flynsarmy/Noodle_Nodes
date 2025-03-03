#ifndef NNBTLimiter_H_INCLUDED
#define NNBTLimiter_H_INCLUDED

#include "decorator_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTLimiter : public NNBTDecoratorNodes {
	GDCLASS(NNBTLimiter, NNBTDecoratorNodes)

private:
	int _max_repeat_times;
	int _current_repeat_times;
	int _tick_result;

protected:
	static void _bind_methods();

public:
	NNBTLimiter();
	~NNBTLimiter();

	// Getters and setters for attributes.

	int get_max_repeat_times() const;
	void set_max_repeat_times(int repeat_times);
	//int get_tick_result() const;
	//void set_tick_result( int tick_result );

	// Handling functions.
	virtual void reset_bt_node() override;

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot

#endif
