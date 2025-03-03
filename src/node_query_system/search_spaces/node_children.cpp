#include "node_children.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

NNNodeChildrenSearchSpace::NNNodeChildrenSearchSpace() {
	_parent_node = nullptr;
}

NNNodeChildrenSearchSpace::~NNNodeChildrenSearchSpace() {
}

void NNNodeChildrenSearchSpace::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_parent_node", "parent_node"), &NNNodeChildrenSearchSpace::set_parent_node);
	ClassDB::bind_method(D_METHOD("get_parent_node"), &NNNodeChildrenSearchSpace::get_parent_node);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "parent_node", PROPERTY_HINT_NODE_TYPE, "Node"), "set_parent_node", "get_parent_node");
}

void NNNodeChildrenSearchSpace::_initialize_search_space() {
}

void NNNodeChildrenSearchSpace::_uninitialize_search_space() {
	_parent_node = nullptr;
}

// Getters and setters.

void NNNodeChildrenSearchSpace::set_parent_node(Node *parent_node) {
	_parent_node = parent_node;
}

Node *NNNodeChildrenSearchSpace::get_parent_node() const {
	return _parent_node;
}

TypedArray<Node> NNNodeChildrenSearchSpace::get_searchspace_nodes() const {
	if (_parent_node == nullptr || !UtilityFunctions::is_instance_id_valid(_parent_node->get_instance_id())) {
		TypedArray<Node> emptyarray;
		return emptyarray;
	}
	return _parent_node->get_children();
}

/**
void NNNodeChildrenSearchSpace::_ready() {
	//if( !get_is_active() ) return;
	if( Engine::get_singleton()->is_editor_hint() ) return;

	//_parent_node = get_node_or_null(_parent_node_path);
}
/**/
