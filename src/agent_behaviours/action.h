#ifndef NNAction_H_INCLUDED
#define NNAction_H_INCLUDED

#include "actions.h"

#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNAction : public NNActions {
	GDCLASS(NNAction, NNActions)

private:
	int _action_id;

protected:
	static void _bind_methods();

public:
	NNAction();
	~NNAction();

	// Getters and setters for attributes.

	void set_action_id(int action_id);
	int get_action_id() const;

	// Handling functions.

	virtual bool start_action() override;
	virtual bool end_action() override;
	virtual Node *step_actions() override;
};

} //namespace godot

#endif
