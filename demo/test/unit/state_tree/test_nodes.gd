# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_internal_status_inactive_before_tick() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	_root.add_child(_child)

	assert_eq(_root.internal_status, 0)
	assert_eq(_child.internal_status, 0)

func test_internal_status_active_after_tick() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	_root.add_child(_child)

	_root.tick(_root, 0.1)
	assert_eq(_root.internal_status, 1)
	assert_eq(_child.internal_status, 1)

func test_internal_status_inactive_on_enter_condition_fail() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_fail.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}
	_root.add_child(_child)

	_root.tick(blackboard, 0.1)
	assert_eq(_root.internal_status, 1)
	assert_eq(_child.internal_status, 0)

func test_internal_status_active_only_on_first_child_of_non_parallel_state() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child1: NNSTNode = NNSTNode.new()
	var _child2: NNSTNode = NNSTNode.new()

	_root.add_child(_child1)
	_root.add_child(_child2)

	_root.tick(_root, 0.1)
	assert_eq(_root.internal_status, 1)
	assert_eq(_child1.internal_status, 1)
	assert_eq(_child2.internal_status, 0)

func test_internal_status_updates_on_transition() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child1: NNSTNode = NNSTNode.new()
	var _child2: NNSTNode = NNSTNode.new()

	_root.add_child(_child1)
	_root.add_child(_child2)

	_root.tick(_root, 0.1)
	_child1.transition_to(_child1.get_path_to(_child2), _root, 0.1)
	assert_eq(_root.internal_status, 1)
	assert_eq(_child1.internal_status, 0)
	assert_eq(_child2.internal_status, 1)

func test_internal_status_active_on_grandchild() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _grandchild: NNSTNode = NNSTNode.new()

	_root.add_child(_child)
	_child.add_child(_grandchild)

	_root.tick(_root, 0.1)
	assert_eq(_root.internal_status, 1)
	assert_eq(_child.internal_status, 1)
	assert_eq(_grandchild.internal_status, 1)

func test_node_methods_called_in_correct_order() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _node: NNSTNode = autofree(load("res://test/unit/state_tree/node_on_call_logger.gd").new())
	var blackboard: Array[String] = []

	var _child1: NNSTNode = _node.duplicate()
	var _child2: NNSTNode = _node.duplicate()
	var _grandchild: NNSTNode = _node.duplicate()
	_child1.name = '_child1'
	_child2.name = '_child2'
	_grandchild.name = '_grandchild'
	_root.add_child(_child1)
	_root.add_child(_child2)
	_child1.add_child(_grandchild)

	# Activation order
	_root.tick(blackboard, 0.1)
	assert_eq(blackboard, [
		"_child1 on_enter_condition",
		"_child1 on_enter_state",
		"_grandchild on_enter_condition",
		"_grandchild on_enter_state",
		"_child1 on_tick",
		"_grandchild on_tick"
	])

	# Transition order
	blackboard = []
	_child1.transition_to(_child1.get_path_to(_child2), blackboard, 0.1)
	assert_eq(blackboard, [
		"_grandchild on_exit_state",
		"_child1 on_exit_state",
		"_child2 on_enter_state"
	])
