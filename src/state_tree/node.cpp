#include "node.h"

/*#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/time.hpp>
#include "../performance_monitor_singleton.h"
*/

using namespace godot;

// Method binds.

//void NNSTSelector::_bind_methods() {

//ADD_SUBGROUP("Debugging","");

//ClassDB::bind_method(D_METHOD("set_total_tick_usec", "total_tick_usec"), &NNSTSelector::set_total_tick_usec);
//ClassDB::bind_method(D_METHOD("get_total_tick_usec"), &NNSTSelector::get_total_tick_usec);
//ADD_PROPERTY(PropertyInfo(Variant::INT, "total_tick_usec", PROPERTY_HINT_NONE), "set_total_tick_usec","get_total_tick_usec");

//ClassDB::bind_method(D_METHOD("tick", "blackboard", "delta"), &NNSTSelector::tick);
//}

void NNSTNode::_bind_methods() {
}

// Constructor and destructor.

NNSTNode::NNSTNode() {
}

NNSTNode::~NNSTNode() {
}

// Getters and Setters.

// Handling functions.

/**
NNSTNodes* NNSTSelector::_tick(Variant blackboard, double delta) {

	// The selector will only consider the state tree nodes.
	NNSTNodes* result_state = nullptr;
	for( int i = 0; i < get_child_count(); ++i ) {
		if( NNSTNodes* stnode = godot::Object::cast_to<NNSTNodes>(get_child(i)) ) {
			if( stnode->on_enter_condition( blackboard, delta ) ) {
				result_state = stnode->_tick(blackboard, delta);
				if( result_state != nullptr ) {
					return result_state;
				}
			}//endif on_enter_condition returned true
		}// endif valid stnode
	}//endfor child count
	return nullptr; // We shouldn't get here. If we do, there were no child nodes.
}
/**/

// Godot virtuals.
//void NNSTNode::_ready() {
//    if( Engine::get_singleton()->is_editor_hint() ) return;
//    reset();
//}
