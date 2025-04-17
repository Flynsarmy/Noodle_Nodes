#pragma once

#include "branch_nodes.h"
#include "guard.h"

namespace godot {

class NNSTGuard; // Forward declaration

class NNSTTransition : public NNSTNodes {
	GDCLASS(NNSTTransition, NNSTNodes)

private:
	String _event_name;
	NodePath _to;

protected:
	static void _bind_methods();
	Ref<NNSTGuard> guard;

public:
	NNSTTransition();
	~NNSTTransition();

	// Getters and setters for attributes.
	NodePath get_to() const;
	void set_to(const NodePath &p_to);

	String get_event_name() const;
	void set_event_name(const String &p_event_name);

	Ref<NNSTGuard> get_guard() const;
	void set_guard(const Ref<NNSTGuard> &p_guard);

	// Handling functions.
};

} //namespace godot
