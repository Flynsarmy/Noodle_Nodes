#include "task_nodes.h"
#include "../agent_behaviours/considerations.h"
#include <state_tree/node.h>
#include <state_tree/transition.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNSTTaskNodes::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_child_state_selection_rule", "child_state_selection_rule"), &NNSTTaskNodes::set_child_state_selection_rule);
	ClassDB::bind_method(D_METHOD("get_child_state_selection_rule"), &NNSTTaskNodes::get_child_state_selection_rule);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "child_state_selection_rule", PROPERTY_HINT_ENUM, "OnEnterConditionMethod:0,UtilityScoring:1"), "set_child_state_selection_rule", "get_child_state_selection_rule");

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_internal_status", "internal_status"), &NNSTTaskNodes::set_internal_status);
	ClassDB::bind_method(D_METHOD("get_internal_status"), &NNSTTaskNodes::get_internal_status);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "internal_status", PROPERTY_HINT_ENUM, "Inactive:0,Active:1"), "set_internal_status", "get_internal_status");

	ClassDB::bind_method(D_METHOD("send_event", "name"), &NNSTTaskNodes::send_event);

	GDVIRTUAL_BIND(on_input, "event");
}

// Constructor and destructor.

NNSTTaskNodes::NNSTTaskNodes() {
	_active_states.clear();
	_internal_status = ST_INTERNAL_STATUS_INACTIVE;
	_child_state_selection_rule = NNSTNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD;

	_num_child_states = 0;
	_num_child_considerations = 0;
	_num_child_transitions = 0;

#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = 0;
	_last_visited_timestamp = 0;
#endif
}

NNSTTaskNodes::~NNSTTaskNodes() {
}

// Getters and Setters.

void NNSTTaskNodes::set_child_state_selection_rule(int child_state_selection_rule) {
	_child_state_selection_rule = child_state_selection_rule;
}

int NNSTTaskNodes::get_child_state_selection_rule() const {
	return _child_state_selection_rule;
}

/**
Dictionary NNSTTaskNodes::get_child_nodes_as_dictionary(NNSTTaskNodes* tree_root_node ) {
	_tree_root_node = tree_root_node;
	Dictionary results;
	for( int i = 0; i < get_child_count(); ++i ) {
		if( NNSTTaskNodes* stnode = godot::Object::cast_to<NNSTTaskNodes>(get_child(i)) ) {
			results[stnode->get_name()] = stnode;
			results.merge( stnode->get_child_nodes_as_dictionary(tree_root_node) );
		}
	}
	return results;
}
/**/

void NNSTTaskNodes::set_internal_status(int internal_status) {
#ifdef DEBUG_ENABLED
	_last_visited_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
	if (internal_status != ST_INTERNAL_STATUS_ACTIVE && internal_status != ST_INTERNAL_STATUS_INACTIVE) {
		internal_status = ST_INTERNAL_STATUS_INACTIVE;
	}

	_internal_status = internal_status;
}

int NNSTTaskNodes::get_internal_status() const {
	return _internal_status;
}

// Handling methods.

void NNSTTaskNodes::on_input(const Ref<InputEvent> &event) {
	if (has_method("on_input")) {
		call("on_input", event);
	}
}

void NNSTTaskNodes::on_unhandled_input(const Ref<InputEvent> &event) {
	if (has_method("on_unhandled_input")) {
		call("on_unhandled_input", event);
	}
}

void NNSTTaskNodes::on_unhandled_key_input(const Ref<InputEvent> &event) {
	if (has_method("on_unhandled_key_input")) {
		call("on_unhandled_key_input", event);
	}
}

/**
 * Called by a child state's transition_to() method. Transitions out of the old state and into the new.
 */
void NNSTTaskNodes::_handle_transition(NNSTNode *from_state, NNSTNode *to_state) {
	if (!_can_transition_to(from_state, to_state)) {
		return;
	}

	from_state->_transition_out();
	// Remove the from_state from our list of active states
	for (unsigned int i = 0; i < _active_states.size(); i++) {
		if (Object::cast_to<NNSTNode>(_active_states[i]) == from_state) {
			_active_states.remove_at(i);
			break;
		}
	}

	to_state->_transition_in();
	_active_states.push_back(to_state);
}

bool NNSTTaskNodes::_can_transition_to(NNSTNode *from_state, NNSTNode *to_state) {
	if (to_state == nullptr) {
		return false;
	}

	if (from_state == to_state) {
		return false;
	}

	if (from_state->is_ancestor_of(to_state)) {
		return false;
	}

	return true;
}

/**
 * Searches children for states to activate using the appropriate
 * activation method based on the `get_child_state_selection_rule()` setting.
 *
 * Only activates up to 1 child for each node (CompoundState).
 */
TypedArray<NNSTNode> NNSTTaskNodes::_evaluate_child_activations() {
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

			// Only 1 child gets activated
			break;
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

void NNSTTaskNodes::_input(const Ref<InputEvent> &p_event) {
	if (get_internal_status() == 1) {
		on_input(p_event);
	}
}

void NNSTTaskNodes::_unhandled_input(const Ref<InputEvent> &p_event) {
	if (get_internal_status() == 1) {
		on_unhandled_input(p_event);
	}
}

void NNSTTaskNodes::_unhandled_key_input(const Ref<InputEvent> &p_event) {
	if (get_internal_status() == 1) {
		on_unhandled_key_input(p_event);
	}
}

void NNSTTaskNodes::_notification(int p_what) {
	if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		_child_states.clear();
		_child_considerations.clear();
		int num_children = get_child_count();
		for (int i = 0; i < num_children; ++i) {
			if (NNConsiderations *cons = godot::Object::cast_to<NNConsiderations>(get_child(i))) {
				_child_considerations.push_back(cons);
			} else if (NNSTTransition *tns = godot::Object::cast_to<NNSTTransition>(get_child(i))) {
				_child_transitions.push_back(tns);
			} else if (NNSTNode *stnode = godot::Object::cast_to<NNSTNode>(get_child(i))) {
				_child_states.push_back(stnode);
			}
		} //endfor child nodes
		_num_child_states = (unsigned int)_child_states.size();
		_num_child_considerations = (unsigned int)_child_considerations.size();
		_num_child_transitions = (unsigned int)_child_transitions.size();
	}
}
