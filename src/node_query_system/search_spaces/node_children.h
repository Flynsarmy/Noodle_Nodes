#pragma once

#include "nqs.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNNodeChildrenSearchSpace : public NNNQSSearchSpaces {
	GDCLASS(NNNodeChildrenSearchSpace, NNNQSSearchSpaces)

private:
	Node *_parent_node;

protected:
	static void _bind_methods();

	virtual void _initialize_search_space() override;
	virtual void _uninitialize_search_space() override;

public:
	NNNodeChildrenSearchSpace();
	~NNNodeChildrenSearchSpace();

	// Getters and setters for attributes.
	void set_parent_node(Node *parent);
	Node *get_parent_node() const;

	virtual TypedArray<Node> get_searchspace_nodes() const override;

	// Godot virtuals.
	//void _ready();
};

} //namespace godot
