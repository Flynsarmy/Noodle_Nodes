#include "run_nqs_query.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// Method binds.

void NNBTRunNQSQuery::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_nqs_search_space", "nqs_search_space"), &NNBTRunNQSQuery::set_nqs_search_space);
	ClassDB::bind_method(D_METHOD("get_nqs_search_space"), &NNBTRunNQSQuery::get_nqs_search_space);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "nqs_search_space", PROPERTY_HINT_NODE_TYPE, "NNNQSSearchSpaces"), "set_nqs_search_space", "get_nqs_search_space");

	ClassDB::bind_method(D_METHOD("set_top_n_to_find", "top_n_to_find"), &NNBTRunNQSQuery::set_top_n_to_find);
	ClassDB::bind_method(D_METHOD("get_top_n_to_find"), &NNBTRunNQSQuery::get_top_n_to_find);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "top_n_to_find", PROPERTY_HINT_RANGE, "1,32,or_greater"), "set_top_n_to_find", "get_top_n_to_find");

	ClassDB::bind_method(D_METHOD("set_is_high_priority", "is_high_priority"), &NNBTRunNQSQuery::set_is_high_priority);
	ClassDB::bind_method(D_METHOD("get_is_high_priority"), &NNBTRunNQSQuery::get_is_high_priority);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_high_priority", PROPERTY_HINT_NONE), "set_is_high_priority", "get_is_high_priority");
}

// Constructor and destructor.

NNBTRunNQSQuery::NNBTRunNQSQuery() {
	_nqs_search_space = nullptr;
	_nqs = nullptr;
	_time_budget_usec = 200;
	_top_n_to_find = 1;
	_query_state = QS_IDLE;
	_is_high_priority = true;
}

NNBTRunNQSQuery::~NNBTRunNQSQuery() {
	_nqs_search_space = nullptr;
	_nqs = nullptr;
}

// Getters and Setters.

void NNBTRunNQSQuery::set_nqs_search_space(NNNQSSearchSpaces *nqs_search_space) {
	_nqs_search_space = nqs_search_space;
	//_nqs_search_space_cache = ObjectID();
}

NNNQSSearchSpaces *NNBTRunNQSQuery::get_nqs_search_space() const {
	return _nqs_search_space;
}

void NNBTRunNQSQuery::set_time_budget_usec(int time_budget_usec) {
	_time_budget_usec = time_budget_usec;
}

int NNBTRunNQSQuery::get_time_budget_usec() const {
	return _time_budget_usec;
}

void NNBTRunNQSQuery::set_top_n_to_find(int top_n_to_find) {
	_top_n_to_find = top_n_to_find;
}

int NNBTRunNQSQuery::get_top_n_to_find() const {
	return _top_n_to_find;
}

void NNBTRunNQSQuery::set_is_high_priority(bool is_high_priority) {
	_is_high_priority = is_high_priority;
}

bool NNBTRunNQSQuery::get_is_high_priority() const {
	return _is_high_priority;
}

// Handling methods.

void NNBTRunNQSQuery::reset_bt_node() {
	_query_state = QS_IDLE;
	_nqs_search_space->reset_query_variables();
}

NNBTNodes::Status NNBTRunNQSQuery::tick(Variant blackboard, float delta) {
	if (_nqs_search_space == nullptr || !UtilityFunctions::is_instance_id_valid(_nqs_search_space->get_instance_id())) {
		_nqs_search_space = nullptr;
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		set_tick_result(Status::FAILURE);
		return Status::FAILURE;
	}
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}

	switch (_query_state) {
		case QS_IDLE: {
			_nqs_search_space->set_top_n_to_find(_top_n_to_find);
			//_nqs_search_space_node->start_query(_time_budget_usec);
			_nqs->post_query(_nqs_search_space, _is_high_priority);
			_query_state = QS_RUNNING;
			set_tick_result(Status::RUNNING);
			//emit_signal("btnode_ticked", blackboard, delta);
			return Status::RUNNING;
		} break;
		case QS_COMPLETED: {
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			set_tick_result(Status::SUCCESS);
			//emit_signal("btnode_exited", blackboard, delta);
			return Status::SUCCESS;
		} break;
		default: {
			set_tick_result(Status::RUNNING);
			//emit_signal("btnode_ticked", blackboard, delta);
			return Status::RUNNING;
		} break;
	}
	// We shouldn't get here.
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	set_tick_result(Status::FAILURE);
	//emit_signal("btnode_exited", blackboard, delta);
	return Status::FAILURE;
}

void NNBTRunNQSQuery::_ready() {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	_nqs = godot::Object::cast_to<NNNodeQuerySystem>(Engine::get_singleton()->get_singleton("NodeQuerySystem"));
	// godot::Object::cast_to<NNNodeQuerySystem>(godot::Engine::get_singleton("NodeQuerySystem"));
}

void NNBTRunNQSQuery::_physics_process(double delta) {
	if (_nqs_search_space == nullptr)
		return;
	if (_nqs == nullptr)
		return;
	if (_query_state == QS_RUNNING) {
		bool is_still_running = _nqs_search_space->get_is_query_still_running(); //->execute_query(_time_budget_usec);
		if (!is_still_running) {
			_query_state = QS_COMPLETED;
		}
	}
}
