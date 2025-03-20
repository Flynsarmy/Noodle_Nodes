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

/**
 * Searches children for states to activate using the appropriate
 * activation method based on the `get_child_state_selection_rule()` setting.
 *
 * Only activates up to 1 child for each node (CompoundState).
 */
TypedArray<NNSTNode> NNSTParallel::_evaluate_child_activations() {
	TypedArray<NNSTNode> nodes;

	if (get_child_state_selection_rule() == NNSTNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD) {
		// Childs are evaluated by using the user-defined on_enter_condition method.
		for (unsigned int i = 0; i < _num_child_states; ++i) {
			NNSTNode *stnode = _child_states[i];
			if (!stnode->get_is_enabled()) {
				continue;
			}

			if (!stnode->on_enter_condition()) {
				continue;
			}

			// Activate the child and evaluate its children
			nodes.push_back(stnode);
		}
	} else if (get_child_state_selection_rule() == NNSTNodeChildStateSelectionRule::UTILITY_SCORING) {
		// Childs are evaluated by using Utility-based scoring.
		NNSTNode *highest_scoring_state_to_activate = nullptr;
		float highest_score = -9999999.9999;
		for (unsigned int i = 0; i < _num_child_states; ++i) {
			NNSTNode *stnode = _child_states[i];

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

	return nodes;
}

// Godot virtuals.
