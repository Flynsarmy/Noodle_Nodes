#include "nodes.h"
#include "../agent_behaviours/considerations.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTNodes::_bind_methods() {
	BIND_ENUM_CONSTANT(RUNNING);
	BIND_ENUM_CONSTANT(FAILURE);
	BIND_ENUM_CONSTANT(SUCCESS);
	BIND_ENUM_CONSTANT(SKIP);

	ClassDB::bind_method(D_METHOD("set_reset_rule", "reset_rule"), &NNBTNodes::set_reset_rule);
	ClassDB::bind_method(D_METHOD("get_reset_rule"), &NNBTNodes::get_reset_rule);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "reset_rule", PROPERTY_HINT_ENUM, "WhenTicked:0,WhenCompleted:1,WhenTickedAfterBeingCompleted:2,Never:3"), "set_reset_rule", "get_reset_rule");

	ClassDB::bind_method(D_METHOD("set_evaluation_method", "evaluation_method"), &NNBTNodes::set_evaluation_method);
	ClassDB::bind_method(D_METHOD("get_evaluation_method"), &NNBTNodes::get_evaluation_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "evaluation_method", PROPERTY_HINT_ENUM, "Sum:0,Min:1,Max:2,Mean:3,Multiply:4,FirstNonZero:5"), "set_evaluation_method", "get_evaluation_method");

	ClassDB::bind_method(D_METHOD("get_tree_root"), &NNBTNodes::get_tree_root);

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNBTNodes::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNBTNodes::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE), "set_score", "get_score");

	ClassDB::bind_method(D_METHOD("set_tick_result", "tick_result"), &NNBTNodes::set_tick_result);
	ClassDB::bind_method(D_METHOD("get_tick_result"), &NNBTNodes::get_tick_result);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tick_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1,Skip:-2"), "set_tick_result", "get_tick_result");

	ClassDB::bind_method(D_METHOD("set_internal_status", "internal_status"), &NNBTNodes::set_internal_status);
	ClassDB::bind_method(D_METHOD("get_internal_status"), &NNBTNodes::get_internal_status);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "internal_status", PROPERTY_HINT_ENUM, "Unticked:0,Ticked:1,Completed:2"), "set_internal_status", "get_internal_status");

	// ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTNodes::tick);

	// Signals.
	//ADD_SIGNAL(MethodInfo("btnode_entered", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	//ADD_SIGNAL(MethodInfo("btnode_ticked", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
	//ADD_SIGNAL(MethodInfo("btnode_exited", PropertyInfo(Variant::OBJECT, "blackboard"), PropertyInfo(Variant::FLOAT, "delta")));
}

// Constructor and destructor.

NNBTNodes::NNBTNodes() {
	_cached_tree_root_path = NodePath();
	_is_tree_root_path_cached = false;
	_score = 0.0;
	_evaluation_method = NNBTNodesEvaluationMethod::Multiply;
	_invert_score = false;
	_tick_result = Status::SUCCESS;
	_internal_status = BT_INTERNAL_STATUS_UNTICKED;
	_reset_rule = NNBTNodesResetRule::WHEN_TICKED_AFTER_BEING_COMPLETED;
	_has_reset_rule_changed = false;
	//_is_first_tick = true;
	_num_child_btnodes = 0;
	_num_child_considerations = 0;
#ifdef DEBUG_ENABLED
	_last_visited_timestamp = 0;
	_last_evaluated_timestamp = 0;
#endif
}

NNBTNodes::~NNBTNodes() {
}

// Getters and Setters.

void NNBTNodes::set_evaluation_method(int evaluation_method) {
	_evaluation_method = evaluation_method;
}

int NNBTNodes::get_evaluation_method() const {
	return _evaluation_method;
}

void NNBTNodes::set_score(float score) {
	_score = score;
}

float NNBTNodes::get_score() const {
	return _score;
}

void NNBTNodes::set_tick_result(Status tick_result) {
	_tick_result = tick_result;
}

NNBTNodes::Status NNBTNodes::get_tick_result() const {
	return _tick_result;
}

void NNBTNodes::set_internal_status(int internal_status) {
#ifdef DEBUG_ENABLED
	_last_visited_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif
	switch (_reset_rule) {
		case NNBTNodesResetRule::WHEN_COMPLETED: {
			if (internal_status == BT_INTERNAL_STATUS_COMPLETED) {
				reset_bt_node();
				//_is_first_tick = true;
			}
		} break;
		case NNBTNodesResetRule::WHEN_TICKED_AFTER_BEING_COMPLETED: {
			if (_internal_status == BT_INTERNAL_STATUS_COMPLETED &&
					internal_status == BT_INTERNAL_STATUS_TICKED) {
				reset_bt_node();
				//_is_first_tick = true;
			}
		} break;
		case NNBTNodesResetRule::WHEN_TICKED: {
			if (internal_status == BT_INTERNAL_STATUS_TICKED && (_internal_status != BT_INTERNAL_STATUS_UNTICKED || _has_reset_rule_changed)) {
				reset_bt_node();
				//_is_first_tick = true;
			}
		} break;
		default: {
		} break;
	}

	_internal_status = internal_status;
	_has_reset_rule_changed = false;
}

int NNBTNodes::get_internal_status() const {
	return _internal_status;
}

void NNBTNodes::set_reset_rule(int reset_rule) {
	_has_reset_rule_changed = (reset_rule != _reset_rule);
	_reset_rule = reset_rule;
}

int NNBTNodes::get_reset_rule() const {
	return _reset_rule;
}

NodePath NNBTNodes::get_tree_root() const {
	if (_is_tree_root_path_cached) {
		return _cached_tree_root_path;
	}

	Node *current_node = const_cast<NNBTNodes *>(this);
	NodePath path;

	while (current_node) {
		path = this->get_path_to(current_node);
		current_node = current_node->get_parent();
		if (!Object::cast_to<NNBTNodes>(current_node)) {
			break;
		}
	}

	_is_tree_root_path_cached = true;
	_cached_tree_root_path = path;
	return path;
}

// Handling methods.

void NNBTNodes::reset() {
	_internal_status = BT_INTERNAL_STATUS_UNTICKED;
	for (int i = 0; i < _num_child_btnodes; i++) {
		_child_btnodes[i]->reset();
	}
	/**
	for( int i = 0; i < get_child_count(); i++ ) {
		if( NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(i)) ) {
			btnode->reset();
		}
	}
	/**/
}

/**
void NNBTNodes::reset_for_looping() {
	_internal_status = BT_INTERNAL_STATUS_UNTICKED;
	for( int i = 0; i < get_child_count(); i++ ) {
		if( NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(get_child(i)) ) {
			btnode->reset_for_looping();
		}
	}
}
/**/

float NNBTNodes::evaluate() {
//if( !get_is_enabled() ) return 0.0f;
//if( Engine::get_singleton()->is_editor_hint() ) return 0.0f;
#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif

	_score = 0.0f;

	// Evaluate the child considerations.
	if (_num_child_considerations < 1) {
		return _score;
	}
	//int num_children = get_child_count();
	//if( num_children < 1 ) {
	//    return _score;
	//}
	float child_score = 0.0f;
	for (unsigned int i = 0; i < _num_child_considerations; i++) {
		//Node* node = get_child(i);
		//if( node == nullptr ) continue;
		//NNConsiderations* considerationNode = godot::Object::cast_to<NNConsiderations>(node);
		//if( considerationNode == nullptr ) continue;
		NNConsiderations *considerationNode = _child_considerations[i];
		if (!considerationNode->get_is_enabled())
			continue;
		child_score = considerationNode->evaluate();
		if (considerationNode->get_has_vetoed()) {
			_score = 0.0f;
			return 0.0f; // Veto zeroes out the score for the entire group.
		}

		switch (_evaluation_method) {
			case NNBTNodesEvaluationMethod::Min: {
				if (i == 0)
					_score = child_score;
				if (child_score < _score)
					_score = child_score;
			} break;
			case NNBTNodesEvaluationMethod::Max: {
				if (i == 0)
					_score = child_score;
				if (child_score > _score)
					_score = child_score;
			} break;
			case NNBTNodesEvaluationMethod::Multiply: {
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
			case NNBTNodesEvaluationMethod::FirstNonZero: {
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

	if (_evaluation_method == NNBTNodesEvaluationMethod::Mean) {
		_score = _score / ((float)_num_child_considerations);
	}

	if (_invert_score) {
		_score = 1.0f - _score;
	}

	return _score;
}

NNBTNodes::Status NNBTNodes::tick(Variant blackboard, float delta) {
	return Status::RUNNING;
}

// Godot virtuals.

void NNBTNodes::_notification(int p_what) {
	if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		// Update the child pointers if the child nodes are changed
		// somehow. This caching of the pointers is done to avoid
		// the expensive godot::Object::cast_to calls later.
		update_child_vectors();

		_is_tree_root_path_cached = false;
	}
}

void NNBTNodes::update_child_vectors() {
	_child_btnodes.clear();
	_child_considerations.clear();
	for (int i = 0; i < get_child_count(); i++) {
		if (NNBTNodes *btnode = godot::Object::cast_to<NNBTNodes>(get_child(i))) {
			_child_btnodes.push_back(btnode);
		} else if (NNConsiderations *consideration = godot::Object::cast_to<NNConsiderations>(get_child(i))) {
			_child_considerations.push_back(consideration);
		}
	}
	_num_child_btnodes = (unsigned int)_child_btnodes.size();
	_num_child_considerations = (unsigned int)_child_considerations.size();
}
