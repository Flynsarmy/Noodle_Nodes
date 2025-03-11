#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class NoodleNodes : public Node {
	GDCLASS(NoodleNodes, Node)

private:
	bool _is_enabled;

protected:
	static void _bind_methods();

public:
	NoodleNodes();
	~NoodleNodes();

	// Getters and setters for attributes.
	void set_is_enabled(bool is_enabled);
	bool get_is_enabled() const;
};

} //namespace godot
