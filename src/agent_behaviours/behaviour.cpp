#include "behaviour.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "consideration.h"
#include "consideration_group.h"

using namespace godot;

// Method binds.

void NNBehaviour::_bind_methods() {
	ADD_SUBGROUP("Configuration", "");

	ClassDB::bind_method(D_METHOD("set_behaviour_id", "behaviour_id"), &NNBehaviour::set_behaviour_id);
	ClassDB::bind_method(D_METHOD("get_behaviour_id"), &NNBehaviour::get_behaviour_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "behaviour_id", PROPERTY_HINT_NONE), "set_behaviour_id", "get_behaviour_id");

	ClassDB::bind_method(D_METHOD("set_can_be_interrupted", "can_be_interrupted"), &NNBehaviour::set_can_be_interrupted);
	ClassDB::bind_method(D_METHOD("get_can_be_interrupted"), &NNBehaviour::get_can_be_interrupted);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_be_interrupted", PROPERTY_HINT_NONE), "set_can_be_interrupted", "get_can_be_interrupted");

	ClassDB::bind_method(D_METHOD("set_cooldown_seconds", "cooldown_seconds"), &NNBehaviour::set_cooldown_seconds);
	ClassDB::bind_method(D_METHOD("get_cooldown_seconds"), &NNBehaviour::get_cooldown_seconds);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cooldown_seconds", PROPERTY_HINT_RANGE, "0.0,3600.0,or_greater,suffix:seconds"), "set_cooldown_seconds", "get_cooldown_seconds");

	ClassDB::bind_method(D_METHOD("set_cooldown_turns", "cooldown_turns"), &NNBehaviour::set_cooldown_turns);
	ClassDB::bind_method(D_METHOD("get_cooldown_turns"), &NNBehaviour::get_cooldown_turns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cooldown_turns", PROPERTY_HINT_RANGE, "0,64,or_greater,suffix:turns"), "set_cooldown_turns", "get_cooldown_turns");

	ClassDB::bind_method(D_METHOD("set_considerations", "considerations"), &NNBehaviour::set_considerations);
	ClassDB::bind_method(D_METHOD("get_considerations"), &NNBehaviour::get_considerations);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "considerations", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "NNConsiderationResources")), "set_considerations", "get_considerations");

	ADD_SUBGROUP("Debugging", "");

	//ClassDB::bind_method(D_METHOD("set_score", "score"), &NNBehaviour::set_score);
	//ClassDB::bind_method(D_METHOD("get_score"), &NNBehaviour::get_score);
	//ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_RANGE,"-100.0,100.0"), "set_score","get_score");

	ClassDB::bind_method(D_METHOD("set_current_action_index", "current_action_index"), &NNBehaviour::set_current_action_index);
	ClassDB::bind_method(D_METHOD("get_current_action_index"), &NNBehaviour::get_current_action_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_action_index", PROPERTY_HINT_RANGE, "0,256"), "set_current_action_index", "get_current_action_index");

	// Signals.
	ADD_SIGNAL(MethodInfo("behaviour_entered"));
	ADD_SIGNAL(MethodInfo("physics_frame_tick", PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("idle_frame_tick", PropertyInfo(Variant::FLOAT, "delta")));
	ADD_SIGNAL(MethodInfo("behaviour_exited"));
}

// Constructor and destructor.

NNBehaviour::NNBehaviour() {
	//_current_action_node = nullptr;
	_score = 0.0f;
	_cooldown_seconds = 0.0f;
	_current_cooldown_seconds = 0.0f;
	_cooldown_turns = 0;
	_current_cooldown_turns = 0;
	_current_action_index = 0;
	_can_be_interrupted = true;
	_behaviour_id = 0;
	_num_child_considerations = 0;
}

NNBehaviour::~NNBehaviour() {
}

// Handling functions.

// Getters and Setters.

void NNBehaviour::set_behaviour_id(int behaviour_id) {
	_behaviour_id = behaviour_id;
}

int NNBehaviour::get_behaviour_id() const {
	return _behaviour_id;
}

void NNBehaviour::set_can_be_interrupted(bool can_be_interrupted) {
	_can_be_interrupted = can_be_interrupted;
}

bool NNBehaviour::get_can_be_interrupted() const {
	return _can_be_interrupted;
}

/*
void NNBehaviour::set_score( float score ) {
	_score = score;
}

float NNBehaviour::get_score() const {
	return _score;
}
*/
void NNBehaviour::set_cooldown_seconds(float cooldown_seconds) {
	_cooldown_seconds = cooldown_seconds;
}

float NNBehaviour::get_cooldown_seconds() const {
	return _cooldown_seconds;
}

void NNBehaviour::set_cooldown_turns(int cooldown_turns) {
	_cooldown_turns = cooldown_turns;
}

int NNBehaviour::get_cooldown_turns() const {
	return _cooldown_turns;
}

void NNBehaviour::set_current_action_index(int current_action_index) {
	_current_action_index = current_action_index;
}

int NNBehaviour::get_current_action_index() const {
	return _current_action_index;
}

/**
void NNBehaviour::set_considerations( TypedArray<NNConsiderationResources> considerations ) {
	_considerations = considerations;
}

TypedArray<NNConsiderationResources> NNBehaviour::get_considerations() const {
	return _considerations;
}
/**/

// Godot virtuals.
/**
void NNBehaviour::_notification(int p_what) {

	if( p_what == NOTIFICATION_CHILD_ORDER_CHANGED ) {
		_child_considerations.clear();
		int num_children = get_child_count();
		for( int i = 0; i < num_children; ++i ) {
			if( NNConsiderations* cons = godot::Object::cast_to<NNConsiderations>(get_child(i))) {
				_child_considerations.push_back(cons);
			}
		}//endfor child nodes
		_num_child_considerations = (unsigned int)_child_considerations.size();
	}
	/**
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			// Entered the tree.
		} break;
		case NOTIFICATION_EXIT_TREE: {

		} break;
	}

}
/**/

void NNBehaviour::_ready() {
	if (!get_is_enabled())
		return;
	if (Engine::get_singleton()->is_editor_hint())
		return;

	// Get the considerations and actions.
}

void NNBehaviour::_process(double delta) {
	if (!get_is_enabled())
		return;
	if (Engine::get_singleton()->is_editor_hint())
		return;
	if (_current_cooldown_seconds > 0.0f) {
		_current_cooldown_seconds -= delta;
		if (_current_cooldown_seconds < 0.0f)
			_current_cooldown_seconds = 0.0f;
	}
}

// Handling functions.

float NNBehaviour::evaluate() {
#ifdef DEBUG_ENABLED
	_last_evaluated_timestamp = godot::Time::get_singleton()->get_ticks_usec();
#endif

	// If the behaviour is on cooldown, it cannot be chosen.
	if (_current_cooldown_seconds > 0.0f)
		return 0.0f;
	if (_current_cooldown_turns > 0) {
		--_current_cooldown_turns;
		return 0.0f;
	}

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
		float child_score = consideration_resource->evaluate(has_vetoed, this);
		if (has_vetoed) {
			return 0.0f; // A consideration vetoed.
		}
		switch (_evaluation_method) {
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::Min: {
				if (i == 0)
					_score = child_score;
				if (child_score < _score)
					_score = child_score;
			} break;
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::Max: {
				if (i == 0)
					_score = child_score;
				if (child_score > _score)
					_score = child_score;
			} break;
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::Multiply: {
				if (i == 0)
					_score = child_score;
				else
					_score *= child_score;
				// If after multiplication we are at 0.0, then none of the
				// other considerations will ever change the result, so bail.
				if (_score == 0.0) {
					return 0.0;
				}
			} break;
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::FirstNonZero: {
				if (child_score > 0.0) {
					_score = child_score;
					return _score;
				}
			} break;
			default:
				_score += child_score;
		} //end switch evaluation method
	}

	// Evaluate the child nodes.
	//int num_children = get_child_count();
	//for( int i = 0; i < num_children; ++i ) {
	//NNConsiderations* considerationNode = godot::Object::cast_to<NNConsiderations>(get_child(i));
	float child_score = 0.0f;
	for (unsigned int i = 0; i < _num_child_considerations; ++i) {
		NNConsiderations *considerationNode = _child_considerations[i];
		//if( considerationNode == nullptr ) continue;
		if (!considerationNode->get_is_enabled())
			continue;
		child_score = considerationNode->evaluate();
		if (considerationNode->get_has_vetoed()) {
			_score = 0.0f;
			return 0.0f; // The consideration vetoed this behaviour.
		}
		switch (_evaluation_method) {
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::Min: {
				if (i == 0)
					_score = child_score;
				if (child_score < _score)
					_score = child_score;
			} break;
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::Max: {
				if (i == 0)
					_score = child_score;
				if (child_score > _score)
					_score = child_score;
			} break;
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::Multiply: {
				if (i == 0)
					_score = child_score;
				else
					_score *= child_score;
				// If after multiplication we are at 0.0, then none of the
				// other considerations will ever change the result, so bail.
				if (_score == 0.0) {
					return 0.0;
				}
			} break;
			case NNConsiderationGroup::NNConsiderationGroupEvaluationMethod::FirstNonZero: {
				if (child_score > 0.0) {
					_score = child_score;
					return _score;
				}
			} break;
			default:
				_score += child_score;
		} //end switch evaluation method
	} //endfor children

	return _score;
}

void NNBehaviour::start_behaviour() {
	_current_cooldown_seconds = _cooldown_seconds;
	_current_cooldown_turns = _cooldown_turns;
	_current_action_index = 0;
	//_current_action_node = nullptr;
	//_current_action_node = update_behaviour();
	emit_signal("behaviour_entered");
	update_behaviour();
}

void NNBehaviour::end_behaviour() {
	emit_signal("behaviour_exited");
	_current_action_index = 0;
}

NNAction *NNBehaviour::update_behaviour() {
	//WARN_PRINT("AIBehaviour::update_behaviour(): stepping actions. " + get_name());
	return godot::Object::cast_to<NNAction>(step_actions());
}

Node *NNBehaviour::step_actions() {
	//WARN_PRINT("NNBehaviour::step_actions() " + get_name());
	if (_current_action_index >= get_child_count())
		return nullptr;

	// Check if the node is an action.
	Node *current_node = get_child(_current_action_index);
	if (NNAction *current_action = godot::Object::cast_to<NNAction>(current_node)) {
		if (!current_action->get_is_finished()) {
			return current_node;
		}
		// If the action fails, clean up and end stepping.
		if (current_action->get_has_failed()) {
			current_action->end_action();
			return nullptr;
		}
		current_action->end_action();
	} else {
		//WARN_PRINT("NNBehaviour::step_actions(): Checking if the current index is an action group.");
		// Check if the current index has an action group and if that group has a follow-up action.
		if (NNActionGroup *current_action_group = godot::Object::cast_to<NNActionGroup>(current_node)) {
			//WARN_PRINT("NNBehaviour::step_actions(): Yes, current action is an action group. Stepping...");
			current_action = godot::Object::cast_to<NNAction>(current_action_group->step_actions());
			//if( _current_action_node == nullptr ) WARN_PRINT("NNBehaviour::step_actions(): group sent back a NULL action pointer.");
			if (current_action != nullptr)
				return current_action;
			//WARN_PRINT("NNBehaviour::step_actions(): Action has completed, ending action for the action group.");
			// If the group fails, clean up and end stepping.
			if (current_action_group->get_has_failed()) {
				current_action_group->end_action();
				return nullptr;
			}
			current_action_group->end_action();
			//WARN_PRINT("NNBehaviour::step_actions(): Action group action ended.");
		} //endif current node index is action group
	} //endif current node index is an action
	//WARN_PRINT("NNBehaviour::step_actions(): Finding a new action...");

	++_current_action_index;
	while (_current_action_index < get_child_count()) {
		current_node = get_child(_current_action_index);
		if (current_node != nullptr) {
			if (NNAction *action_node = godot::Object::cast_to<NNAction>(current_node)) {
				//WARN_PRINT("NNBehaviour::step_actions(): Found an action, starting the action...");
				if (action_node->get_is_enabled()) {
					//_current_action_node = action_node;
					bool action_start_result = action_node->start_action();
					//if( !action_start_result ) return nullptr;
					//WARN_PRINT("NNBehaviour::step_actions(): Done, returning action node.");
					return action_node;
				} //endif action is active
			} else if (NNActionGroup *action_group = godot::Object::cast_to<NNActionGroup>(current_node)) {
				if (action_group->get_is_enabled()) {
					//WARN_PRINT("NNBehaviour::step_actions(): Found an action group, starting the action group." + get_name());
					bool action_group_start_result = action_group->start_action();
					//if( !action_group_start_result ) return nullptr;
					//WARN_PRINT("NNBehaviour::step_actions(): Stepping it to find the sub action...");
					action_node = godot::Object::cast_to<NNAction>(action_group->step_actions());
					//if( _current_action_node == nullptr ) {
					//WARN_PRINT("NNBehaviour::step_actions(): Stepping function returned NULL action pointer.");
					//}
					if (action_node != nullptr) {
						//WARN_PRINT("NNBehaviour::step_actions(): Stepping function returned a valid action pointer." + _current_action_node->get_name());
						action_node->start_action();
						return action_node;
					}
				} //endif action group is active
			} // endif is action or action_group
		} // endif node is not nullptr
		++_current_action_index;
	} //endwhile action index in bounds

	//WARN_PRINT("NNBehaviour::step_actions(): NO ACTION FOUND! " + get_name());
	return nullptr;
}
