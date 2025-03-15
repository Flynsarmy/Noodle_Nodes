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
 * Recursively searches children for states to activate using the appropriate
 * activation method based on the `get_child_state_selection_rule()` setting.
 *
 * Only activates up to 1 child for each node (CompoundState).
 */
// void NNSTParallel::evaluate_state_activations(TypedArray<NNSTNodes> *nodes, Variant blackboard, float delta) {
// 	if (get_child_state_selection_rule() == NNStateTreeNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD) {
// 		// Childs are evaluated by using the user-defined on_enter_condition method.
// 		for (unsigned int i = 0; i < _num_child_states; ++i) {
// 			NNSTNodes *stnode = _child_states[i];
// 			if (!stnode->get_is_enabled()) {
// 				continue;
// 			}

// 			if (!stnode->on_enter_condition(blackboard, delta)) {
// 				continue;
// 			}

// 			// Activate the child and evaluate its children
// 			nodes->push_back(stnode);
// 			stnode->evaluate_state_activations(nodes, blackboard, delta);
// 		}
// 	} else if (get_child_state_selection_rule() == NNStateTreeNodeChildStateSelectionRule::UTILITY_SCORING) {
// 		// @TODO run children in order of priority

// 		// Childs are evaluated by using Utility-based scoring.
// 		NNSTNodes *highest_scoring_state_to_activate = nullptr;
// 		float highest_score = -9999999.9999;
// 		for (unsigned int i = 0; i < _num_child_states; ++i) {
// 			NNSTNodes *stnode = _child_states[i];

// 			if (!stnode->get_is_enabled()) {
// 				continue;
// 			}

// 			float score = stnode->evaluate();
// 			if (score > 0) {
// 				// Activate the child and evaluate its children
// 				nodes->push_back(stnode);
// 				stnode->evaluate_state_activations(nodes, blackboard, delta);
// 			}

// 			// float score = stnode->evaluate();
// 			// if (score > highest_score) {
// 			// 	highest_score = score;
// 			// 	highest_scoring_state_to_activate = stnode;
// 			// }
// 		}

// 		// Return the highest scoring state that can activate.
// 		// if (highest_scoring_state_to_activate != nullptr) {
// 		// 	// Activate the child and evaluate its children
// 		// 	nodes->push_back(highest_scoring_state_to_activate);
// 		// 	highest_scoring_state_to_activate->evaluate_state_activations(nodes, blackboard, delta);
// 		// }
// 	}
// }

/**
 * Searches children for states to activate using the appropriate
 * activation method based on the `get_child_state_selection_rule()` setting.
 *
 * Only activates up to 1 child for each node (CompoundState).
 */
TypedArray<NNSTNodes> NNSTParallel::_evaluate_child_activations(Variant blackboard, float delta) {
	TypedArray<NNSTNodes> nodes;

	if (get_child_state_selection_rule() == NNStateTreeNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD) {
		// Childs are evaluated by using the user-defined on_enter_condition method.
		for (unsigned int i = 0; i < _num_child_states; ++i) {
			NNSTNodes *stnode = _child_states[i];
			if (!stnode->get_is_enabled()) {
				continue;
			}

			if (!stnode->on_enter_condition(blackboard, delta)) {
				continue;
			}

			// Activate the child and evaluate its children
			nodes.push_back(stnode);
		}
	} else if (get_child_state_selection_rule() == NNStateTreeNodeChildStateSelectionRule::UTILITY_SCORING) {
		// Childs are evaluated by using Utility-based scoring.
		NNSTNodes *highest_scoring_state_to_activate = nullptr;
		float highest_score = -9999999.9999;
		for (unsigned int i = 0; i < _num_child_states; ++i) {
			NNSTNodes *stnode = _child_states[i];

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
