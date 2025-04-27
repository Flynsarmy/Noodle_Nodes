#include "node.h"

using namespace godot;

// Method binds.

void NNSTNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_initial_state", "initial_state"), &NNSTNode::set_initial_state);
	ClassDB::bind_method(D_METHOD("get_initial_state"), &NNSTNode::get_initial_state);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "initial_state", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "NNSTTickedNodes"), "set_initial_state", "get_initial_state");
}

// Constructor and destructor.

NNSTNode::NNSTNode() {
	_initial_state = NodePath("");
}

NNSTNode::~NNSTNode() {
}

// Getters and Setters.
NodePath NNSTNode::get_initial_state() const {
	return _initial_state;
}

void NNSTNode::set_initial_state(const NodePath &p_initial_state) {
	// If a valid NodePath was sent but it's not a direct child, set to an empty NodePath
	if (!p_initial_state.is_empty() && (p_initial_state.get_name_count() != 1 || p_initial_state == NodePath("."))) {
		_initial_state = NodePath("");
	} else {
		_initial_state = p_initial_state;
	}
}

// Handling functions.

/**
 * Searches children for states to activate using the appropriate
 * activation method based on the `get_child_state_selection_rule()` setting.
 *
 * Only activates up to 1 child for each node (CompoundState).
 */
void NNSTNode::_evaluate_child_activations(std::vector<NNSTTickedNodes *> &nodes) {
	// If we have an initial state set, reorder children so it's first
	if (!_initial_state.is_empty() && _num_child_states > 1) {
		Node *initial_state_node = get_node_or_null(_initial_state);
		if (initial_state_node != nullptr) {
			for (unsigned int i = 1; i < _num_child_states; i++) {
				if (_child_states[i] == initial_state_node) {
					NNSTTickedNodes *stnode = _child_states[i];
					_child_states.erase(_child_states.begin() + i);
					_child_states.insert(_child_states.begin(), stnode);
					break;
				}
			}
		}
	}

	if (get_child_state_selection_rule() == NNSTNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD) {
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

			// Only 1 child gets activated
			break;
		}
	} else if (get_child_state_selection_rule() == NNSTNodeChildStateSelectionRule::UTILITY_SCORING) {
		// Childs are evaluated by using Utility-based scoring.
		NNSTTickedNodes *highest_scoring_state_to_activate = nullptr;
		float highest_score = -9999999.9999;
		for (unsigned int i = 0; i < _num_child_states; i++) {
			NNSTTickedNodes *stnode = _child_states[i];

			if (!stnode->get_is_enabled()) {
				continue;
			}

			float score = stnode->evaluate();
			if (score > highest_score) {
				highest_score = score;
				highest_scoring_state_to_activate = stnode;
			}
		}

		// Return the highest scoring state that can activate.
		if (highest_scoring_state_to_activate != nullptr) {
			// Activate the child and evaluate its children
			nodes.push_back(highest_scoring_state_to_activate);
		}
	}
}

void NNSTNode::_transition_in() {
	// This method is called when the child list changes. If we were already
	// active but had no active children, we don't want to run on_enter_state()
	// again.
	if (_internal_status != ST_INTERNAL_STATUS_ACTIVE) {
		set_internal_status(ST_INTERNAL_STATUS_ACTIVE);
		on_enter_state();
	}

	if (_num_active_states > 0) {
		return;
	}

	NNSTTickedNodes *cur_active_state;
	std::vector<NNSTTickedNodes *> new_active_states;
	_evaluate_child_activations(new_active_states);

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

// Godot virtuals.
