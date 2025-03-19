#include "node.h"
#include <state_tree/root.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>

using namespace godot;

// Method binds.

void NNSTNode::_bind_methods() {
	// ClassDB::bind_method(D_METHOD("set_root", "total_tick_usec"), &NNSTNode::set_root);
	ClassDB::bind_method(D_METHOD("get_root"), &NNSTNode::get_root);

	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNSTNode::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNSTNode::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE), "set_score", "get_score");

	ClassDB::bind_method(D_METHOD("set_evaluation_method", "evaluation_method"), &NNSTNode::set_evaluation_method);
	ClassDB::bind_method(D_METHOD("get_evaluation_method"), &NNSTNode::get_evaluation_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "evaluation_method", PROPERTY_HINT_ENUM, "Sum:0,Min:1,Max:2,Mean:3,Multiply:4,FirstNonZero:5"), "set_evaluation_method", "get_evaluation_method");

	ClassDB::bind_method(D_METHOD("set_on_entered_condition", "is_entered"), &NNSTNode::set_is_on_entered_condition_true);
	ClassDB::bind_method(D_METHOD("get_on_entered_condition"), &NNSTNode::get_is_on_entered_condition_true);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "child_state_selection_rule", PROPERTY_HINT_ENUM, "OnEnterConditionMethod:0,UtilityScoring:1" ), "set_child_state_selection_rule","get_child_state_selection_rule");

	ClassDB::bind_method(D_METHOD("transition_to", "new_state_nodepath", "blackboard", "delta"), &NNSTNode::transition_to);

	GDVIRTUAL_BIND(on_enter_condition, "blackboard", "delta");
	GDVIRTUAL_BIND(on_enter_state, "blackboard", "delta");
	GDVIRTUAL_BIND(on_exit_state, "blackboard", "delta");
	GDVIRTUAL_BIND(on_tick, "blackboard", "delta");
}

// Constructor and destructor.

NNSTNode::NNSTNode() {
	_is_on_entered_condition_true = true;
	_evaluation_method = NNSTNodeEvaluationMethod::Multiply;
	_score = 0.0;
	_invert_score = false;
}

NNSTNode::~NNSTNode() {
}

// Getters and Setters.

void NNSTNode::set_root(NNSTRoot *p_root) {
	_root = p_root;

	if (!Engine::get_singleton()->is_editor_hint()) {
		for (unsigned int i = 0; i < get_child_count(); ++i) {
			if (NNSTNode *stnode = godot::Object::cast_to<NNSTNode>(get_child(i))) {
				stnode->set_root(p_root);
			}
		}
	}
}

NNSTRoot *NNSTNode::get_root() const {
	return _root;
}

void NNSTNode::set_score(float score) {
	_score = score;
}

float NNSTNode::get_score() const {
	return _score;
}

void NNSTNode::set_evaluation_method(int evaluation_method) {
	_evaluation_method = evaluation_method;
}

int NNSTNode::get_evaluation_method() const {
	return _evaluation_method;
}

void NNSTNode::set_is_on_entered_condition_true(bool is_on_entered_condition_true) {
	_is_on_entered_condition_true = is_on_entered_condition_true;
}

bool NNSTNode::get_is_on_entered_condition_true() const {
	return _is_on_entered_condition_true;
}

// Handling functions.

void NNSTNode::send_event(String name, Variant blackboard, float delta) {
	for (unsigned int i = 0; i < _child_transitions.size(); i++) {
		if (_child_transitions[i]->get_event_name() == name) {
			transition_to(_child_transitions[i]->get_to().slice(1), blackboard, delta);
			return;
		}
	}

	for (unsigned int i = 0; i < _active_states.size(); i++) {
		NNSTNode *stnode = godot::Object::cast_to<NNSTNode>(_active_states[i]);
		print_line("Checking ", stnode->get_name());
		stnode->send_event(name, blackboard, delta);
	}
}

float NNSTNode::evaluate() {
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
			case NNSTNodeEvaluationMethod::Min: {
				if (i == 0)
					_score = child_score;
				if (child_score < _score)
					_score = child_score;
			} break;
			case NNSTNodeEvaluationMethod::Max: {
				if (i == 0)
					_score = child_score;
				if (child_score > _score)
					_score = child_score;
			} break;
			case NNSTNodeEvaluationMethod::Multiply: {
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
			case NNSTNodeEvaluationMethod::FirstNonZero: {
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

	if (_evaluation_method == NNSTNodeEvaluationMethod::Mean) {
		_score = _score / ((float)_num_child_considerations);
	}

	if (_invert_score) {
		_score = 1.0f - _score;
	}

	return _score;
}

void NNSTNode::transition_to(NodePath path_to_node, Variant blackboard, float delta) {
	// if (_tree_root_node == nullptr) {
	// 	return;
	// }
	// _tree_root_node->transition_to(path_to_node, blackboard, delta);

	NNSTTaskNodes *parent = godot::Object::cast_to<NNSTTaskNodes>(get_parent());
	if (!parent) {
		return;
	}

	NNSTNode *to_state = get_node<NNSTNode>(path_to_node);
	if (to_state == nullptr) {
		return;
	}

	parent->_handle_transition(this, to_state, blackboard, delta);
}

void NNSTNode::_transition_in(Variant blackboard, float delta) {
	if (_active_states.size() > 0) {
		return;
	}

	set_internal_status(ST_INTERNAL_STATUS_ACTIVE);
	on_enter_state(blackboard, delta);

	TypedArray<NNSTNode> new_active_states = _evaluate_child_activations(blackboard, delta);

	NNSTNode *cur_active_state;

	// do on_exit for any states no longer active
	for (unsigned int i = 0; i < _active_states.size(); ++i) {
		cur_active_state = godot::Object::cast_to<NNSTNode>(_active_states[i]);

		if (!new_active_states.has(cur_active_state)) {
			cur_active_state->_transition_out(blackboard, delta);
		}
	}

	// And then enter the new states.
	for (unsigned int i = 0; i < new_active_states.size(); ++i) {
		cur_active_state = godot::Object::cast_to<NNSTNode>(new_active_states[i]);

		if (!_active_states.has(new_active_states[i])) {
			cur_active_state->_transition_in(blackboard, delta);
		}
	}

	_active_states = new_active_states;
};

void NNSTNode::_transition_out(Variant blackboard, float delta) {
	if (_internal_status == 0) {
		return;
	}

	for (int i = _active_states.size() - 1; i >= 0; i--) {
		NNSTNode *cur_active_state = godot::Object::cast_to<NNSTNode>(_active_states[i]);
		cur_active_state->_transition_out(blackboard, delta);
	}

	on_exit_state(blackboard, delta);
	set_internal_status(ST_INTERNAL_STATUS_INACTIVE);
	_active_states.clear();
};

bool NNSTNode::on_enter_condition(Variant blackboard, float delta) {
	if (has_method("on_enter_condition")) {
		return call("on_enter_condition", blackboard, delta);
	}
	emit_signal("state_check_enter_condition", blackboard, delta);
	return _is_on_entered_condition_true;
}

void NNSTNode::on_enter_state(Variant blackboard, float delta) {
	call("on_enter_state", blackboard, delta);
	emit_signal("state_entered", blackboard, delta);
}

void NNSTNode::on_exit_state(Variant blackboard, float delta) {
	call("on_exit_state", blackboard, delta);
	emit_signal("state_exited", blackboard, delta);
}

void NNSTNode::on_tick(Variant blackboard, float delta) {
	call("on_tick", blackboard, delta);
	emit_signal("state_ticked", blackboard, delta);

	for (unsigned int i = 0; i < _active_states.size(); i++) {
		NNSTNode *stnode = godot::Object::cast_to<NNSTNode>(_active_states[i]);

		stnode->on_tick(blackboard, delta);
	}

#ifdef DEBUG_ENABLED
	_last_visited_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
}

// Godot virtuals.
