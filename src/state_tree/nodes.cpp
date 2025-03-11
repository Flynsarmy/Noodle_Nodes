#include "nodes.h"
#include "../agent_behaviours/considerations.h"
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

	ClassDB::bind_method(D_METHOD("set_considerations", "considerations"), &NNSTNodes::set_considerations);
	ClassDB::bind_method(D_METHOD("get_considerations"), &NNSTNodes::get_considerations);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "considerations", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "NNConsiderationResources")), "set_considerations", "get_considerations");

	ClassDB::bind_method(D_METHOD("set_on_entered_condition", "is_entered"), &NNSTNodes::set_is_on_entered_condition_true);
	ClassDB::bind_method(D_METHOD("get_on_entered_condition"), &NNSTNodes::get_is_on_entered_condition_true);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "child_state_selection_rule", PROPERTY_HINT_ENUM, "OnEnterConditionMethod:0,UtilityScoring:1" ), "set_child_state_selection_rule","get_child_state_selection_rule");

	ClassDB::bind_method(D_METHOD("get_tree_root"), &NNSTNodes::get_tree_root);

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNSTNodes::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNSTNodes::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE), "set_score", "get_score");

	ClassDB::bind_method(D_METHOD("transition_to", "new_state_nodepath", "blackboard", "delta"), &NNSTNodes::transition_to);

	GDVIRTUAL_BIND(on_enter_condition, "blackboard", "delta");
	GDVIRTUAL_BIND(on_enter_state, "blackboard", "delta");
	GDVIRTUAL_BIND(on_exit_state, "blackboard", "delta");
	GDVIRTUAL_BIND(on_tick, "blackboard", "delta");
	GDVIRTUAL_BIND(transition_to, "path_to_node", "blackboard", "delta");

	ADD_SIGNAL(MethodInfo("state_check_enter_condition", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("state_entered", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("state_ticked", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("state_exited", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
}

// Constructor and destructor.

NNSTNodes::NNSTNodes() {
	_score = 0.0;
	_evaluation_method = NNSTNodesEvaluationMethod::Multiply;
	_invert_score = false;
	_tree_root_node = nullptr;
	_child_state_selection_rule = NNStateTreeNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD;
	_is_on_entered_condition_true = true;

	_num_child_states = 0;
	_num_child_considerations = 0;

#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = 0;
	_last_visited_timestamp = 0;
#endif
}

NNSTNodes::~NNSTNodes() {
	_tree_root_node = nullptr;
}

// Getters and Setters.

void NNSTNodes::set_considerations(TypedArray<NNConsiderationResources> considerations) {
	_considerations = considerations;
}

TypedArray<NNConsiderationResources> NNSTNodes::get_considerations() const {
	return _considerations;
}

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

// Handling methods.

float NNSTNodes::evaluate() {
//if( !get_is_enabled() ) return 0.0f;
//if( Engine::get_singleton()->is_editor_hint() ) return 0.0f;
#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif

	_score = 0.0f;
	bool has_vetoed = false;
	// Evaluate the consideration resources (if any).
	int num_resources = _considerations.size();
	for (int i = 0; i < num_resources; ++i) {
		NNConsiderationResources *consideration_resource = godot::Object::cast_to<NNConsiderationResources>(_considerations[i]);
		if (consideration_resource == nullptr) {
			continue;
		}
		if (!consideration_resource->get_is_enabled()) {
			continue;
		}
		float score = consideration_resource->evaluate(has_vetoed, this);
		if (has_vetoed) {
			_score = 0.0f;
			return 0.0f; // A consideration vetoed.
		}
		_score += score;
	}

	// Evaluate the children.
	int num_children = get_child_count();
	if (num_children < 1) {
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
		_score = _score / ((float)num_children);
	}

	if (_invert_score) {
		_score = 1.0f - _score;
	}

	return _score;
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
#ifdef DEBUG_ENABLED
	_last_visited_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
}

void NNSTNodes::transition_to(NodePath path_to_node, Variant blackboard, float delta) {
	if (_tree_root_node == nullptr) {
		return;
	}
	_tree_root_node->transition_to(path_to_node, blackboard, delta);
}

NNSTNodes *NNSTNodes::evaluate_state_activation(Variant blackboard, float delta) {
	unsigned int num_state_tree_childs = 0;

	if (get_child_state_selection_rule() == NNStateTreeNodeChildStateSelectionRule::ON_ENTER_CONDITION_METHOD) {
		// Childs are evaluated by using the user-defined on_enter_condition method.
		//for( int i = 0; i < get_child_count(); ++i ) {
		//    if( NNSTNodes* stnode = godot::Object::cast_to<NNSTNodes>(get_child(i)) ) {
		for (unsigned int i = 0; i < _num_child_states; ++i) {
			NNSTNodes *stnode = _child_states[i];
			if (!stnode->get_is_enabled()) {
				continue;
			}

			++num_state_tree_childs;
			if (!stnode->on_enter_condition(blackboard, delta)) {
				continue;
			}

			if (NNSTNodes *result = stnode->evaluate_state_activation(blackboard, delta)) {
				return result;
			} //endif result is not nullptr
			//}//endif valid node type
		} //endfor child nodes
	} else {
		// Childs are evaluated by using Utility-based scoring.
		NNSTNodes *highest_scoring_state_to_activate = nullptr;
		float highest_score = -9999999.9999;
		//for( int i = 0; i < get_child_count(); ++i ) {
		//if( NNSTNodes* stnode = godot::Object::cast_to<NNSTNodes>(get_child(i)) ) {
		for (unsigned int i = 0; i < _num_child_states; ++i) {
			NNSTNodes *stnode = _child_states[i];

			if (!stnode->get_is_enabled()) {
				continue;
			}

			++num_state_tree_childs;
			float score = stnode->evaluate();
			if (score > highest_score) {
				if (NNSTNodes *result = stnode->evaluate_state_activation(blackboard, delta)) {
					highest_score = score;
					highest_scoring_state_to_activate = result;
				} //endif result is not nullptr
			} //endif score is higher than current highest

			//}//endif valid node type
		} //endfor child nodes
		// Return the highest scoring state that can activate.
		if (highest_scoring_state_to_activate != nullptr) {
			return highest_scoring_state_to_activate;
		}
	} //endif state selection method

	if (num_state_tree_childs > 0) {
		return nullptr;
	}
	return this; // This has no state tree children, so it is a leaf node.
}

void NNSTNodes::_notification(int p_what) {
	if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		_child_states.clear();
		_child_considerations.clear();
		int num_children = get_child_count();
		for (int i = 0; i < num_children; ++i) {
			if (NNSTNodes *stnode = godot::Object::cast_to<NNSTNodes>(get_child(i))) {
				_child_states.push_back(stnode);
			}
			if (NNConsiderations *cons = godot::Object::cast_to<NNConsiderations>(get_child(i))) {
				_child_considerations.push_back(cons);
			}
		} //endfor child nodes
		_num_child_states = (unsigned int)_child_states.size();
		_num_child_considerations = (unsigned int)_child_considerations.size();
	}
}
