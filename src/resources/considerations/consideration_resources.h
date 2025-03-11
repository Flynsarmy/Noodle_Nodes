#pragma once

#include "../../agent_behaviours/sensors.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace godot {

class NNConsiderationResources : public Resource {
	GDCLASS(NNConsiderationResources, Resource)

private:
	bool _is_enabled;
	//bool _has_vetoed;
protected:
	static void _bind_methods();

public:
	NNConsiderationResources();
	~NNConsiderationResources();

	// Getters and setters for attributes.
	/**/
	void set_is_enabled(bool is_enabled);
	bool get_is_enabled() const;

	//void  set_has_vetoed( bool has_vetoed );
	//bool  get_has_vetoed() const;

	// Handling functions.

	virtual float evaluate(bool &has_vetoed, Node *parent_node);
};

} //namespace godot
