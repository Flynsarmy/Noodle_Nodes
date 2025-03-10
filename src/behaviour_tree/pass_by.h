#pragma once

#include "task_nodes.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/method_ptrcall.hpp>

namespace godot {

class NNBTPassBy : public NNBTTaskNodes {
	GDCLASS(NNBTPassBy, NNBTTaskNodes)

private:
	//int _tick_result;
	bool _has_on_tick_method;

protected:
	static void _bind_methods();

public:
	NNBTPassBy();
	~NNBTPassBy();

	// Getters and setters for attributes.

	//void set_tick_result( int tick_result );
	//int  get_tick_result() const;

	// Handling functions.

	virtual Status tick(Variant blackboard, float delta) override;

	// Godot virtuals.
	void _notification(int p_what) override;
};

} //namespace godot
