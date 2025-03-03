#ifndef NNBTParallel_H_INCLUDED
#define NNBTParallel_H_INCLUDED

#include "composite_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTParallel : public NNBTCompositeNodes {
	GDCLASS(NNBTParallel, NNBTCompositeNodes)

private:
	//bool _is_reactive;
	int _current_child_index;

protected:
	static void _bind_methods();

public:
	NNBTParallel();
	~NNBTParallel();

	// Getters and setters for attributes.
	//void set_is_reactive( bool is_reactive );
	//bool get_is_reactive() const;

	// Handling functions.

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot

#endif
