#include "pass_through.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// Method binds.

void NNBTPassThrough::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("set_tick_result", "tick_result"), &NNBTPassThrough::set_tick_result);
	//ClassDB::bind_method(D_METHOD("get_tick_result"), &NNBTPassThrough::get_tick_result);
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "tick_result", PROPERTY_HINT_ENUM, "Running:0,Success:1,Failure:-1" ), "set_tick_result","get_tick_result");

	//ClassDB::bind_method(D_METHOD("_tick", "blackboard", "delta"), &NNBTPassThrough::tick);
}

// Constructor and destructor.

NNBTPassThrough::NNBTPassThrough() {
	_tick_result = Status::SUCCESS;
	_has_on_tick_method = false;
}

NNBTPassThrough::~NNBTPassThrough() {
}

// Getters and Setters.

void NNBTPassThrough::set_tick_result(Status tick_result) {
	_tick_result = tick_result;
}

NNBTNodes::Status NNBTPassThrough::get_tick_result() const {
	return _tick_result;
}

// Handling methods.

NNBTNodes::Status NNBTPassThrough::tick(Variant blackboard, float delta) {
	// The passthrough node just calls its tick and then ticks the first
	// behaviour tree node child and returns the result of the child.
	// Otherwise it returns what ever is set as the tick result property.
	set_internal_status(BT_INTERNAL_STATUS_TICKED);
	// //if( _is_first_tick ) {
	// //    _is_first_tick = false;
	// //    emit_signal("btnode_entered", blackboard, delta);
	// //}
	// if (_has_on_tick_method) {
	// 	godot::Variant return_value = call("on_tick", blackboard, delta);
	// 	if (return_value.get_type() == godot::Variant::Type::INT) {
	// 		_tick_result = (int)return_value;
	// 		if (_tick_result > 1) {
	// 			_tick_result = 1;
	// 		} else if (_tick_result < -1) {
	// 			_tick_result = -1;
	// 		}
	// 	}
	// }
	// //emit_signal("btnode_ticked", blackboard, delta);

	godot::Variant result;
	result = call("on_tick", blackboard, delta);
	Status return_value = static_cast<Status>((int64_t)result); // or result.operator int64_t();

	//for( int i = 0; i < get_child_count(); ++i ) {
	//    Node* node = get_child(i);
	//    if( NNBTNodes* btnode = godot::Object::cast_to<NNBTNodes>(node) ) {
	for (unsigned int i = 0; i < _num_child_btnodes; ++i) {
		NNBTNodes *btnode = _child_btnodes[i];
		if (!btnode->get_is_enabled()) {
			continue;
		}

		Status result = btnode->tick(blackboard, delta);
		if (result != Status::RUNNING) {
			set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
			//emit_signal("btnode_exited", blackboard, delta);
		}
		return result;
	} //endfor child ndoes

	//}
	set_internal_status(BT_INTERNAL_STATUS_COMPLETED);
	//emit_signal("btnode_exited", blackboard, delta);
	// return _tick_result;
	return return_value;
}

void NNBTPassThrough::_notification(int p_what) {
	if (p_what == NOTIFICATION_POST_ENTER_TREE) {
		_has_on_tick_method = has_method("on_tick");

	} else if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		update_child_vectors();
	}
}
