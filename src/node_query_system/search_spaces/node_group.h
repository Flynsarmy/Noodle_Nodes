#pragma once

#include "nqs.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNNodeGroupSearchSpace : public NNNQSSearchSpaces {
	GDCLASS(NNNodeGroupSearchSpace, NNNQSSearchSpaces)

private:
	StringName _group_name;

protected:
	static void _bind_methods();

public:
	NNNodeGroupSearchSpace();
	~NNNodeGroupSearchSpace();

	// Getters and setters for attributes.
	void set_group_name(StringName group_name);
	StringName get_group_name() const;

	virtual TypedArray<Node> get_searchspace_nodes() const override;
};

} //namespace godot
