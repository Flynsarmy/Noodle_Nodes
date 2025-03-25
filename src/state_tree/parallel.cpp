#include "parallel.h"

using namespace godot;

// Method binds.

void NNSTParallel::_bind_methods() {
}

// Constructor and destructor.

NNSTParallel::NNSTParallel() {
}

NNSTParallel::~NNSTParallel() {
}

// Getters and Setters.

// Handling functions.

void NNSTParallel::_transition_in() {
	// This method is called when the child list changes. If we were already
	// active but had no active children, we don't want to run on_enter_state()
	// again.
	if (_internal_status != ST_INTERNAL_STATUS_ACTIVE) {
		set_internal_status(ST_INTERNAL_STATUS_ACTIVE);
		on_enter_state();
	}

	std::vector<NNSTTickedNodes *> new_active_states;
	_evaluate_child_activations(new_active_states);

	NNSTTickedNodes *cur_active_state;

	// do on_exit for any states no longer active
	for (unsigned int i = 0; i < _num_active_states; i++) {
		cur_active_state = _active_states[i];

		bool found = false;
		for (unsigned int j = 0; j < new_active_states.size(); j++) {
			if (new_active_states[j] == cur_active_state) {
				found = true;
				break;
			}
		}
		if (!found) {
			cur_active_state->_transition_out();
		}
	}

	// And then enter the new states.
	for (unsigned int i = 0; i < new_active_states.size(); i++) {
		cur_active_state = new_active_states[i];

		bool found = false;
		for (unsigned int j = 0; j < _num_active_states; j++) {
			if (_active_states[j] == cur_active_state) {
				found = true;
				break;
			}
		}
		if (!found) {
			cur_active_state->set_root(get_root());
			cur_active_state->_transition_in();
		}
	}

	_active_states = new_active_states;
	_num_active_states = _active_states.size();
};

/**
 * Loops through children attempting to enter them in order.
 */
void NNSTParallel::_evaluate_child_activations(std::vector<NNSTTickedNodes *> &nodes) {
	// Childs are evaluated by using the user-defined on_enter_condition method.
	for (unsigned int i = 0; i < _num_child_states; i++) {
		NNSTTickedNodes *stnode = _child_states[i];
		if (!stnode->get_is_enabled()) {
			continue;
		}

		if (!stnode->on_enter_condition()) {
			continue;
		}

		// Activate the child and evaluate its children
		nodes.push_back(stnode);
	}
}

// Godot virtuals.
