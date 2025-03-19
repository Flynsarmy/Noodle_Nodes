#pragma once

#include "nodes.h"

namespace godot {

class NNSTTransition : public NNSTNodes {
	GDCLASS(NNSTTransition, NNSTNodes)

private:
	String _event_name;
	NodePath _to;

protected:
	static void _bind_methods();

public:
	NNSTTransition();
	~NNSTTransition();

	NodePath get_to() const;
	void set_to(const NodePath &p_to);

	String get_event_name() const;
	void set_event_name(const String &p_event_name);

	virtual void send_event(String name, Variant blackboard, float delta) override;
};

} //namespace godot
