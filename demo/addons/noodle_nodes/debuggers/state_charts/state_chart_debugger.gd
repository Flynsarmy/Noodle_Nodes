@icon("res://addons/noodle_nodes/icons/debugger.svg")
extends Control

## The initial node that should be watched. Optional, if not set
## then no node will be watched. You can set the node that should
## be watched at runtime by calling debug_node().
@export var initial_node_to_watch: NNSTRoot

## The tree that shows the state chart.
@onready var _tree:Tree = %Tree

var _root: NNSTRoot

func _ready() -> void:
	if initial_node_to_watch:
		debug_node(initial_node_to_watch)

## Sets up the debugger to track the given state chart. If the given node is not
## a state chart, it will search the children for a state chart. If no state chart
## is found, the debugger will be disabled.
func debug_node(root: NNSTRoot) -> bool:
	var success: bool = _debug_node(root)

	# if we have no success, we disable the debugger
	if not success:
		push_warning("No state chart found. Disabling debugger.")
		_root = null

	return success

func _debug_node(root: NNSTRoot) -> bool:
	# if we have no root, we use the scene root
	if not is_instance_valid(root):
		return false

	_root = root
	return true

func _process(delta: float) -> void:
	# Clear contents
	_tree.clear()

	if not is_instance_valid(_root):
		return

	var root = _tree.create_item()
	root.set_text(0, _root.name)

	# walk over the state chart and find all active states
	_collect_active_states(_root, root)

func _collect_active_states(root: NNSTNodes, parent: TreeItem):
	for child in root.get_children():
		if child is NNSTNodes:
			if child.internal_status == 1 and child.is_enabled:
				var state_item: TreeItem = _tree.create_item(parent)
				state_item.set_text(0, child.name)

				_collect_active_states(child, state_item)
