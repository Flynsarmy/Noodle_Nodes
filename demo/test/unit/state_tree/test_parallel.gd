# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## Confirm on_* methods are called when they should be
func test_all_children_ticked() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _parallel: NNSTParallel = NNSTParallel.new()
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()

	_root.add_child(_parallel)
	_parallel.add_child(_node1)
	_parallel.add_child(_node2)
	_root.tick(0.1)

	assert_eq(_parallel.get_is_active(), true)
	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node2.get_is_active(), true)

func test_children_failing_condition_dont_tick() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _parallel: NNSTParallel = NNSTParallel.new()
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_fail.gd").new()
	_root.blackboard = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_parallel)
	_parallel.add_child(_node1)
	_parallel.add_child(_node2)
	_root.tick(0.1)

	assert_eq(_parallel.get_is_active(), true)
	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

func test_transitions_away_in_correct_order() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _parallel: NNSTParallel = load("res://test/unit/state_tree/parallel_on_call_logger.gd").new()
	var _grandchild1: NNSTNode = load("res://test/unit/state_tree/node_on_call_logger.gd").new()
	var _grandchild2: NNSTNode = load("res://test/unit/state_tree/node_on_call_logger.gd").new()
	var _node: NNSTNode = load("res://test/unit/state_tree/node_on_call_logger.gd").new()
	_root.blackboard['log'] = []

	# - root
	#   - parallel
	#     - child1
	#       - grandchild11
	#       - grandchild12
	#   - node
	_root.add_child(_parallel)
	_parallel.add_child(_grandchild1)
	_parallel.add_child(_grandchild2)
	_root.add_child(_node)
	_root.name = '_root'
	_parallel.name = '_parallel'
	_grandchild1.name = '_grandchild1'
	_grandchild2.name = '_grandchild2'
	_node.name = '_node';

	_root.tick(0.1)
	_root.blackboard['log'] = []
	_parallel.transition_to(_parallel.get_path_to(_node))

	assert_eq(_parallel.get_is_active(), false)
	assert_eq(_grandchild1.get_is_active(), false)
	assert_eq(_grandchild2.get_is_active(), false)
	assert_eq(_node.get_is_active(), true)
	assert_eq(_root.blackboard['log'], [
		"_grandchild2 on_exit_state",
		"_grandchild1 on_exit_state",
		"_parallel on_exit_state",
		"_node on_enter_state"
	])

func test_parallel_branches_tick_correct_nodes() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _parallel: NNSTParallel = NNSTParallel.new()
	var _child1: NNSTNode = NNSTNode.new()
	var _grandchild11: NNSTNode = NNSTNode.new()
	var _grandchild12: NNSTNode = NNSTNode.new()
	var _child2: NNSTNode = NNSTNode.new()
	var _grandchild21: NNSTNode = NNSTNode.new()
	var _grandchild22: NNSTNode = NNSTNode.new()

	# - root
	#   - parallel
	#     - child1
	#       - grandchild11
	#       - grandchild12
	#     - child2
	#       - grandchild21
	#       - grandchild22
	_root.add_child(_parallel)
	_parallel.add_child(_child1)
	_child1.add_child(_grandchild11)
	_child1.add_child(_grandchild12)
	_parallel.add_child(_child2)
	_child2.add_child(_grandchild21)
	_child2.add_child(_grandchild22)

	_root.tick(0.1)
	assert_eq(_parallel.get_is_active(), true)
	assert_eq(_child1.get_is_active(), true)
	assert_eq(_grandchild11.get_is_active(), true)
	assert_eq(_grandchild12.get_is_active(), false)
	assert_eq(_child2.get_is_active(), true)
	assert_eq(_grandchild21.get_is_active(), true)
	assert_eq(_grandchild22.get_is_active(), false)
