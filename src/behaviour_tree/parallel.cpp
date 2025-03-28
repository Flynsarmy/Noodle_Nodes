#include "parallel.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTParallel::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_is_reactive", "is_reactive"), &NNBTParallel::set_is_reactive);
	//ClassDB::bind_method(D_METHOD("get_is_reactive"), &NNBTParallel::get_is_reactive);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_reactive", PROPERTY_HINT_NONE), "set_is_reactive","get_is_reactive");

	/*
	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNBTParallel::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNBTParallel::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_NONE ), "set_score","get_score");
	/**/
	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTParallel::tick);
}

// Constructor and destructor.

NNBTParallel::NNBTParallel() {
	_current_child_index = 0;
	//_is_reactive = true;
}

NNBTParallel::~NNBTParallel() {
}

// Handling functions.

// Getters and Setters.

/**
void NNBTParallel::set_is_reactive( bool is_reactive ) {
	_is_reactive = is_reactive;
}

bool NNBTParallel::get_is_reactive() const {
	return _is_reactive;
}
/**/

NNBTNodes::Status NNBTParallel::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	Status parallelresult = Status::SUCCESS;
	for (unsigned int i = 0; i < _num_child_btnodes; i++) {
		NNBTNodes *btnode = _child_btnodes[i];
		//if( btnode != nullptr ) {
		if (!btnode->get_is_enabled()) {
			continue;
		}
		Status result = btnode->tick(blackboard, delta);
		if (result == Status::FAILURE) {
			parallelresult = Status::FAILURE;
		} else if (result == Status::RUNNING) {
			parallelresult = Status::RUNNING;
		}
		//}//endif node was of correct type
	} //endfor btnodes
	//emit_signal("btnode_ticked", blackboard, delta);
	if (parallelresult != Status::RUNNING) {
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		//emit_signal("btnode_exited", blackboard, delta);
	}
	set_tick_result(parallelresult);
	return parallelresult;
}
