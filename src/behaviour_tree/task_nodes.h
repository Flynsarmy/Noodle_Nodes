#ifndef NNBTTaskNodes_H_INCLUDED
#define NNBTTaskNodes_H_INCLUDED

#include "nodes.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {

class NNBTTaskNodes : public NNBTNodes {
	GDCLASS(NNBTTaskNodes, NNBTNodes)

private:
protected:
	static void _bind_methods();

	virtual Status on_tick(Variant blackboard, double delta) { return Status::FAILURE; };

	GDVIRTUAL2R(Status, on_tick, Variant, double);

public:
	NNBTTaskNodes();
	~NNBTTaskNodes();
};

} //namespace godot

#endif
