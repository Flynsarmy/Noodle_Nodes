#ifndef NNBTLeaf_H_INCLUDED
#define NNBTLeaf_H_INCLUDED

#include "task_nodes.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/method_ptrcall.hpp>

namespace godot {

class NNBTLeaf : public NNBTTaskNodes {
	GDCLASS(NNBTLeaf, NNBTTaskNodes)

private:
	//Callable _tick_method;
	//int _tick_result;

protected:
	static void _bind_methods();

public:
	NNBTLeaf();
	~NNBTLeaf();

	// Getters and setters for attributes.

	//void set_tick_result( int tick_result );
	//int  get_tick_result() const;

	// Handling functions.

	virtual Status tick(Variant blackboard, float delta) override;

	// Godot virtuals.

	void _notification(int p_what) override;
};

} //namespace godot

#endif
