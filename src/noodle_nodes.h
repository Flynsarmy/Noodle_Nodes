#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class NoodleNodes : public Node {
	GDCLASS(NoodleNodes, Node)

private:
	bool _is_active;

protected:
	static void _bind_methods();

public:
	NoodleNodes();
	~NoodleNodes();

	// Getters and setters for attributes.
	void set_is_active(bool is_active);
	bool get_is_active() const;
};

} //namespace godot
