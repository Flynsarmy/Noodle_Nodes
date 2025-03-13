# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## Confirm on_* methods are called when they should be
func test_all_children_ticked() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _parallel: NNSTParallel = NNSTParallel.new()
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()

	_root.add_child(_parallel)
	_parallel.add_child(_node1)
	_parallel.add_child(_node2)
	_root.tick(_root, 0.1)

	assert_eq(_parallel.internal_status, 1)
	assert_eq(_node1.internal_status, 1)
	assert_eq(_node2.internal_status, 1)

	_root.free()

func test_children_failing_condition_dont_tick() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _parallel: NNSTParallel = NNSTParallel.new()
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_fail.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_parallel)
	_parallel.add_child(_node1)
	_parallel.add_child(_node2)
	_root.tick(blackboard, 0.1)

	assert_eq(_parallel.internal_status, 1)
	assert_eq(_node1.internal_status, 1)
	assert_eq(_node2.internal_status, 0)

	_root.free()

func test_transitions_away_in_correct_order() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _parallel: NNSTParallel = load("res://test/unit/state_tree/parallel_on_call_logger.gd").new()
	var _grandchild1: NNSTNode = load("res://test/unit/state_tree/node_on_call_logger.gd").new()
	var _grandchild2: NNSTNode = load("res://test/unit/state_tree/node_on_call_logger.gd").new()
	var _node: NNSTNode = load("res://test/unit/state_tree/node_on_call_logger.gd").new()
	var blackboard: Array[String] = []

	_root.add_child(_parallel)
	_parallel.add_child(_grandchild1)
	_parallel.add_child(_grandchild2)
	_root.add_child(_node)
	_root.name = '_root'
	_parallel.name = '_parallel'
	_grandchild1.name = '_grandchild1'
	_grandchild2.name = '_grandchild2'
	_node.name = '_node';

	_root.tick(blackboard, 0.1)
	blackboard = []
	_parallel.transition_to(_root.get_path_to(_node), blackboard, 0.1)

	assert_eq(_parallel.internal_status, 0)
	assert_eq(_grandchild1.internal_status, 0)
	assert_eq(_grandchild2.internal_status, 0)
	assert_eq(_node.internal_status, 1)
	assert_eq(blackboard, [
		"_grandchild2 on_exit_state",
		"_grandchild1 on_exit_state",
		"_parallel on_exit_state",
		"_node on_enter_state"
	])

	_root.free()
