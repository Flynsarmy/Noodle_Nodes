#include "node_reference.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// Method binds.

void NNBTNodeReference::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_node_reference", "node_reference"), &NNBTNodeReference::set_node_reference);
	ClassDB::bind_method(D_METHOD("get_node_reference"), &NNBTNodeReference::get_node_reference);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "node_reference", PROPERTY_HINT_NODE_TYPE, "NNBTNodes"), "set_node_reference", "get_node_reference");

	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTNodeReference::tick);
}

// Constructor and destructor.

NNBTNodeReference::NNBTNodeReference() {
	//_tick_result = 1;
	_node_reference = nullptr;
}

NNBTNodeReference::~NNBTNodeReference() {
}

// Getters and Setters.

void NNBTNodeReference::set_node_reference(NNBTNodes *node_reference) {
	if (_node_reference == node_reference || (node_reference != nullptr && (this == node_reference || node_reference->is_ancestor_of(this) || this->is_ancestor_of(node_reference)))) {
		return;
	}

	_node_reference = node_reference;
	_update_cache();
}

NNBTNodes *NNBTNodeReference::get_node_reference() const {
	return _node_reference;
}

/**
void NNBTNodeReference::set_tick_result( int tick_result ) {
	_tick_result = tick_result;
}


int  NNBTNodeReference::get_tick_result() const {
	return _tick_result;
}
/**/

// Handling methods.

void NNBTNodeReference::_update_cache() {
	_cache = ObjectID();
	if (_node_reference != nullptr) {
		_cache = _node_reference->get_instance_id();
	}
}

void NNBTNodeReference::reset() {
	if (_cache.is_null() || !_cache.is_valid()) {
		return;
	}
	if (!_node_reference) {
		return;
	}
	_node_reference->reset();
}

NNBTNodes::Status NNBTNodeReference::tick(Variant blackboard, float delta) {
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	//if( _is_first_tick ) {
	//    _is_first_tick = false;
	//    emit_signal("btnode_entered", blackboard, delta);
	//}
	if (!UtilityFunctions::is_instance_id_valid(_cache)) {
		//_cache.is_null() || !_cache.is_valid() ) {
		_node_reference = nullptr; // Cache shows that the node reference has become invalid.
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		set_tick_result(Status::FAILURE);
		//emit_signal("btnode_exited", blackboard, delta);
		return Status::FAILURE;
	}
	if (!_node_reference) {
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		set_tick_result(Status::FAILURE);
		//emit_signal("btnode_exited", blackboard, delta);
		return Status::FAILURE;
	}
	Status result = _node_reference->tick(blackboard, delta);
	set_tick_result(result);
	//emit_signal("btnode_ticked", blackboard, delta);
	if (result != Status::RUNNING) {
		set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
		//emit_signal("btnode_exited", blackboard, delta);
	}
	return result;
}
