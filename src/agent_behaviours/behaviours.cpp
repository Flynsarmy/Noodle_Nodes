#include "behaviours.h"

using namespace godot;

void NNBehaviours::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_evaluation_method", "evaluation_method"), &NNBehaviours::set_evaluation_method);
	ClassDB::bind_method(D_METHOD("get_evaluation_method"), &NNBehaviours::get_evaluation_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "evaluation_method", PROPERTY_HINT_ENUM, "Sum:0,Min:1,Max:2,Mean:3,Multiply:4,FirstNonZero:5"), "set_evaluation_method", "get_evaluation_method");

	ClassDB::bind_method(D_METHOD("set_score", "score"), &NNBehaviours::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &NNBehaviours::get_score);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score", PROPERTY_HINT_RANGE, "-100.0,100.0"), "set_score", "get_score");
}

// Godot virtuals.

void NNBehaviours::_notification(int p_what) {
	if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED) {
		_child_considerations.clear();
		_child_behaviours.clear();
		int num_children = get_child_count();
		for (int i = 0; i < num_children; i++) {
			if (NNConsiderations *cons = godot::Object::cast_to<NNConsiderations>(get_child(i))) {
				_child_considerations.push_back(cons);
			} else if (NNBehaviours *beh = godot::Object::cast_to<NNBehaviours>(get_child(i))) {
				_child_behaviours.push_back(beh);
			}
		} //endfor child nodes
		_num_child_considerations = (unsigned int)_child_considerations.size();
		_num_child_behaviours = (unsigned int)_child_behaviours.size();
	}
}

// Getters and setters.

void NNBehaviours::set_evaluation_method(int evaluation_method) {
	_evaluation_method = evaluation_method;
}

int NNBehaviours::get_evaluation_method() const {
	return _evaluation_method;
}
