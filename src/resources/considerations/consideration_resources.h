#pragma once

#include "../../agent_behaviours/sensors.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace godot {

class NNConsiderationResources : public Resource {
	GDCLASS(NNConsiderationResources, Resource)

private:
	bool _is_active;
	//bool _has_vetoed;
protected:
	static void _bind_methods();

public:
	NNConsiderationResources();
	~NNConsiderationResources();

	// Getters and setters for attributes.
	/**/
	void set_is_active(bool is_active);
	bool get_is_active() const;

	//void  set_has_vetoed( bool has_vetoed );
	//bool  get_has_vetoed() const;

	// Handling functions.

	virtual float evaluate(bool &has_vetoed, Node *parent_node);
};

} //namespace godot
