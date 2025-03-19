#include "nodes.h"
#include "../agent_behaviours/considerations.h"
#include <state_tree/transition.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNSTNodes::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_child_state_selection_rule", "child_state_selection_rule"), &NNSTNodes::set_child_state_selection_rule);
	ClassDB::bind_method(D_METHOD("get_child_state_selection_rule"), &NNSTNodes::get_child_state_selection_rule);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "child_state_selection_rule", PROPERTY_HINT_ENUM, "OnEnterConditionMethod:0,UtilityScoring:1"), "set_child_state_selection_rule", "get_child_state_selection_rule");

	ClassDB::bind_method(D_METHOD("set_evaluation_method", "evaluation_method"), &NNSTNodes::set_evaluation_method);
	ClassDB::bind_method(D_METHOD("get_evaluation_method"), &NNSTNodes::get_evaluation_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "evaluation_method", PROPERTY_HINT_ENUM, "Sum:0,Min:1,Max:2,Mean:3,Multiply:4,FirstNonZero:5"), "set_evaluation_method", "get_evaluation_method");

	ClassDB::bind_method(D_METHOD("set_on_entered_condition", "is_entered"), &NNSTNodes::set_is_on_entered_condition_true);
	ClassDB::bind_method(D_METHOD("get_on_entered_condition"), &NNSTNodes::get_is_on_entered_condition_true);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "child_state_selection_rule", PROPERTY_HINT_ENUM, "OnEnterConditionMethod:0,UtilityScoring:1" ), "set_child_state_selection_rule","get_child_state_selection_rule");

	ClassDB::bind_method(D_METHOD("get_tree_root"), &NNSTNodes::get_tree_root);

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNSTNodes::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNSTNodes::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE), "set_score", "get_score");

	ClassDB::bind_method(D_METHOD("set_internal_status", "internal_status"), &NNSTNodes::set_internal_status);
	ClassDB::bind_method(D_METHOD("get_internal_status"), &NNSTNodes::get_internal_status);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "internal_status", PROPERTY_HINT_ENUM, "Inactive:0,Active:1"), "set_internal_status", "get_internal_status");

	ClassDB::bind_method(D_METHOD("transition_to", "new_state_nodepath", "blackboard", "delta"), &NNSTNodes::transition_to);
	ClassDB::bind_method(D_METHOD("send_event", "name", "blackboard", "delta"), &NNSTNodes::send_event);

	GDVIRTUAL_BIND(on_input, "event");
	GDVIRTUAL_BIND(on_enter_condition, "blackboard", "delta");
	GDVIRTUAL_BIND(on_enter_state, "blackboard", "delta");
	GDVIRTUAL_BIND(on_exit_state, "blackboard", "delta");
	GDVIRTUAL_BIND(on_tick, "blackboard", "delta");
	// GDVIRTUAL_BIND(transition_to, "path_to_node", "blackboard", "delta");

	ADD_SIGNAL(MethodInfo("state_check_enter_condition", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("state_entered", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("state_ticked", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("state_exited", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
}

// Constructor and destructor.

NNSTNodes::NNSTNodes() {
	_active_states.clear();
	_internal_status = ST_INTERNAL_STATUS_INACTIVE;
	_score = 0.0;
	_evaluation_method = NNSTNodesEvaluationMethod::Multiply;
	_invert_score = false;
	_tree_root_node = nullptr;
	_child_state_selection_rule = NNStateTreeNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD;
	_is_on_entered_condition_true = true;

	_num_child_states = 0;
	_num_child_considerations = 0;
	_num_child_transitions = 0;

#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = 0;
	_last_visited_timestamp = 0;
#endif
}

NNSTNodes::~NNSTNodes() {
	_tree_root_node = nullptr;
}

// Getters and Setters.

void NNSTNodes::set_is_on_entered_condition_true(bool is_on_entered_condition_true) {
	_is_on_entered_condition_true = is_on_entered_condition_true;
}

bool NNSTNodes::get_is_on_entered_condition_true() const {
	return _is_on_entered_condition_true;
}

void NNSTNodes::set_evaluation_method(int evaluation_method) {
	_evaluation_method = evaluation_method;
}

int NNSTNodes::get_evaluation_method() const {
	return _evaluation_method;
}

void NNSTNodes::set_score(float score) {
	_score = score;
}

float NNSTNodes::get_score() const {
	return _score;
}

NNSTNodes *NNSTNodes::get_tree_root() const {
	return _tree_root_node;
}

void NNSTNodes::set_child_state_selection_rule(int child_state_selection_rule) {
	_child_state_selection_rule = child_state_selection_rule;
}

int NNSTNodes::get_child_state_selection_rule() const {
	return _child_state_selection_rule;
}

void NNSTNodes::set_root_node(NNSTNodes *tree_root_node) {
	_tree_root_node = tree_root_node;
	for (int i = 0; i < get_child_count(); ++i) {
		if (NNSTNodes *stnode = godot::Object::cast_to<NNSTNodes>(get_child(i))) {
			stnode->set_root_node(tree_root_node);
		}
	} //endfor children
}

/**
Dictionary NNSTNodes::get_child_nodes_as_dictionary(NNSTNodes* tree_root_node ) {
	_tree_root_node = tree_root_node;
	Dictionary results;
	for( int i = 0; i < get_child_count(); ++i ) {
		if( NNSTNodes* stnode = godot::Object::cast_to<NNSTNodes>(get_child(i)) ) {
			results[stnode->get_name()] = stnode;
			results.merge( stnode->get_child_nodes_as_dictionary(tree_root_node) );
		}
	}
	return results;
}
/**/

void NNSTNodes::set_internal_status(int internal_status) {
#ifdef DEBUG_ENABLED
	_last_visited_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
	if (internal_status != ST_INTERNAL_STATUS_ACTIVE && internal_status != ST_INTERNAL_STATUS_INACTIVE) {
		internal_status = ST_INTERNAL_STATUS_INACTIVE;
	}

	_internal_status = internal_status;
}

int NNSTNodes::get_internal_status() const {
	return _internal_status;
}

// Handling methods.

float NNSTNodes::evaluate() {
//if( !get_is_enabled() ) return 0.0f;
//if( Engine::get_singleton()->is_editor_hint() ) return 0.0f;
#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif

	_score = 0.0f;

	// Evaluate the children.
	if (_num_child_considerations == 0) {
		return _score;
	}
	float child_score = 0.0;

	//for( int i = 0; i < num_children; ++i ) {
	//    Node* node = get_child(i);
	//    if( node == nullptr ) continue;
	//    NNConsiderations* considerationNode = godot::Object::cast_to<NNConsiderations>(node);
	//    if( considerationNode == nullptr ) continue;
	for (unsigned int i = 0; i < _num_child_considerations; ++i) {
		NNConsiderations *considerationNode = _child_considerations[i];
		if (!considerationNode->get_is_enabled())
			continue;
		child_score = considerationNode->evaluate();
		if (considerationNode->get_has_vetoed()) {
			_score = 0.0f;
			return 0.0f; // Veto zeroes out the score for the entire group.
		}

		switch (_evaluation_method) {
			case NNSTNodesEvaluationMethod::Min: {
				if (i == 0)
					_score = child_score;
				if (child_score < _score)
					_score = child_score;
			} break;
			case NNSTNodesEvaluationMethod::Max: {
				if (i == 0)
					_score = child_score;
				if (child_score > _score)
					_score = child_score;
			} break;
			case NNSTNodesEvaluationMethod::Multiply: {
				if (i == 0)
					_score = child_score;
				else
					_score *= child_score;
				// If after multiplication we are at 0.0, then none of the
				// other considerations will ever change the result, so bail.
				if (_score == 0.0f) {
					if (_invert_score) {
						_score = 1.0f;
						return 1.0f;
					}
					_score = 0.0f;
					return 0.0f;
				}
			} break;
			case NNSTNodesEvaluationMethod::FirstNonZero: {
				if (child_score > 0.0f) {
					if (_invert_score) {
						_score = 1.0f - child_score;
					} else {
						_score = child_score;
					}
					return _score;
				}
			} break;
			default:
				_score += child_score;
		} //end switch evaluation method

	} //endfor children

	if (_evaluation_method == NNSTNodesEvaluationMethod::Mean) {
		_score = _score / ((float)_num_child_considerations);
	}

	if (_invert_score) {
		_score = 1.0f - _score;
	}

	return _score;
}

void NNSTNodes::on_input(const Ref<InputEvent> &event) {
	if (has_method("on_input")) {
		call("on_input", event);
	}
}

void NNSTNodes::on_unhandled_input(const Ref<InputEvent> &event) {
	if (has_method("on_unhandled_input")) {
		call("on_unhandled_input", event);
	}
}

void NNSTNodes::on_unhandled_key_input(const Ref<InputEvent> &event) {
	if (has_method("on_unhandled_key_input")) {
		call("on_unhandled_key_input", event);
	}
}

bool NNSTNodes::on_enter_condition(Variant blackboard, float delta) {
	if (has_method("on_enter_condition")) {
		return call("on_enter_condition", blackboard, delta);
	}
	emit_signal("state_check_enter_condition", blackboard, delta);
	return _is_on_entered_condition_true;
}

void NNSTNodes::on_enter_state(Variant blackboard, float delta) {
	call("on_enter_state", blackboard, delta);
	emit_signal("state_entered", blackboard, delta);
}

void NNSTNodes::on_exit_state(Variant blackboard, float delta) {
	call("on_exit_state", blackboard, delta);
	emit_signal("state_exited", blackboard, delta);
}

void NNSTNodes::on_tick(Variant blackboard, float delta) {
	call("on_tick", blackboard, delta);
	emit_signal("state_ticked", blackboard, delta);

	for (unsigned int i = 0; i < _active_states.size(); i++) {
		NNSTNodes *stnode = godot::Object::cast_to<NNSTNodes>(_active_states[i]);

		stnode->on_tick(blackboard, delta);
	}

#ifdef DEBUG_ENABLED
	_last_visited_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
}

void NNSTNodes::send_event(String name, Variant blackboard, float delta) {
	for (unsigned int i = 0; i < _child_transitions.size(); i++) {
		if (_child_transitions[i]->get_event_name() == name) {
			transition_to(_child_transitions[i]->get_to().slice(1), blackboard, delta);
			return;
		}
	}

	for (unsigned int i = 0; i < _active_states.size(); i++) {
		NNSTNodes *stnode = godot::Object::cast_to<NNSTNodes>(_active_states[i]);
		print_line("Checking ", stnode->get_name());
		stnode->send_event(name, blackboard, delta);
	}
}

void NNSTNodes::transition_to(NodePath path_to_node, Variant blackboard, float delta) {
	// if (_tree_root_node == nullptr) {
	// 	return;
	// }
	// _tree_root_node->transition_to(path_to_node, blackboard, delta);

	NNSTNodes *parent = godot::Object::cast_to<NNSTNodes>(get_parent());
	if (!parent) {
		return;
	}

	NNSTNodes *to_state = get_node<NNSTNodes>(path_to_node);
	if (to_state == nullptr) {
		return;
	}

	parent->_handle_transition(this, to_state, blackboard, delta);
}

void NNSTNodes::_transition_out(Variant blackboard, float delta) {
	if (_internal_status == 0) {
		return;
	}

	for (int i = _active_states.size() - 1; i >= 0; i--) {
		NNSTNodes *cur_active_state = godot::Object::cast_to<NNSTNodes>(_active_states[i]);
		cur_active_state->_transition_out(blackboard, delta);
	}

	on_exit_state(blackboard, delta);
	set_internal_status(ST_INTERNAL_STATUS_INACTIVE);
	_active_states.clear();
};

void NNSTNodes::_transition_in(Variant blackboard, float delta) {
	if (_active_states.size() > 0) {
		return;
	}

	set_internal_status(ST_INTERNAL_STATUS_ACTIVE);
	on_enter_state(blackboard, delta);

	TypedArray<NNSTNodes> new_active_states = _evaluate_child_activations(blackboard, delta);

	NNSTNodes *cur_active_state;

	// do on_exit for any states no longer active
	for (int i = 0; i < _active_states.size(); ++i) {
		cur_active_state = godot::Object::cast_to<NNSTNodes>(_active_states[i]);

		if (!new_active_states.has(cur_active_state)) {
			cur_active_state->_transition_out(blackboard, delta);
		}
	}

	// And then enter the new states.
	for (int i = 0; i < new_active_states.size(); ++i) {
		cur_active_state = godot::Object::cast_to<NNSTNodes>(new_active_states[i]);

		if (!_active_states.has(new_active_states[i])) {
			cur_active_state->_transition_in(blackboard, delta);
		}
	}

	_active_states = new_active_states;
};

/**
 * Searches children for states to activate using the appropriate
 * activation method based on the `get_child_state_selection_rule()` setting.
 *
 * Only activates up to 1 child for each node (CompoundState).
 */
TypedArray<NNSTNodes> NNSTNodes::_evaluate_child_activations(Variant blackboard, float delta) {
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

			// Only 1 child gets activated
			break;
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

/**
 * Called by a child state's transition_to() method. Transitions out of the old state and into the new.
 */
void NNSTNodes::_handle_transition(NNSTNodes *from_state, NNSTNodes *to_state, Variant blackboard, float delta) {
	if (!_can_transition_to(from_state, to_state)) {
		return;
	}

	from_state->_transition_out(blackboard, delta);
	// Remove the from_state from our list of active states
	for (unsigned int i = 0; i < _active_states.size(); i++) {
		if (Object::cast_to<NNSTNodes>(_active_states[i]) == from_state) {
			_active_states.remove_at(i);
			break;
		}
	}

	to_state->_transition_in(blackboard, delta);
	_active_states.push_back(to_state);
}

bool NNSTNodes::_can_transition_to(NNSTNodes *from_state, NNSTNodes *to_state) {
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

void NNSTNodes::_input(const Ref<InputEvent> &p_event) {
	if (get_internal_status() == 1) {
		on_input(p_event);
	}
}

void NNSTNodes::_unhandled_input(const Ref<InputEvent> &p_event) {
	if (get_internal_status() == 1) {
		on_unhandled_input(p_event);
	}
}

void NNSTNodes::_unhandled_key_input(const Ref<InputEvent> &p_event) {
	if (get_internal_status() == 1) {
		on_unhandled_key_input(p_event);
	}
}

void NNSTNodes::_notification(int p_what) {
	if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		_child_states.clear();
		_child_considerations.clear();
		int num_children = get_child_count();
		for (int i = 0; i < num_children; ++i) {
			if (NNConsiderations *cons = godot::Object::cast_to<NNConsiderations>(get_child(i))) {
				_child_considerations.push_back(cons);
			} else if (NNSTTransition *tns = godot::Object::cast_to<NNSTTransition>(get_child(i))) {
				_child_transitions.push_back(tns);
			} else if (NNSTNodes *stnode = godot::Object::cast_to<NNSTNodes>(get_child(i))) {
				_child_states.push_back(stnode);
			}
		} //endfor child nodes
		_num_child_states = (unsigned int)_child_states.size();
		_num_child_considerations = (unsigned int)_child_considerations.size();
		_num_child_transitions = (unsigned int)_child_transitions.size();
	}
}
