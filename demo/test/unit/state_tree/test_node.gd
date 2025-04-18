# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## Confirm on_* methods are called when they should be
func test_on_method_state_calls() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()
	_root.blackboard = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_child)
	_root.tick(0.1)
	assert_eq(_root.blackboard['on_enter_condition'], 1)
	assert_eq(_root.blackboard['on_tick'], 1)
	assert_eq(_root.blackboard['on_enter_state'], 1)
	assert_eq(_root.blackboard['on_exit_state'], 0)

	_root.tick(0.1)
	assert_eq(_root.blackboard['on_enter_condition'], 1)
	assert_eq(_root.blackboard['on_tick'], 2)
	assert_eq(_root.blackboard['on_enter_state'], 1)
	assert_eq(_root.blackboard['on_exit_state'], 0)

## No on_* methods should fire if on_enter_condition returns false
func test_on_enter_condition_fail_checks() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_fail.gd").new()
	_root.blackboard = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_child)
	_root.tick(0.1)
	# root will have tried entering on ready. tick tries again.
	assert_eq(_root.blackboard['on_enter_condition'], 2)
	assert_eq(_root.blackboard['on_tick'], 0)
	assert_eq(_root.blackboard['on_enter_state'], 0)
	assert_eq(_root.blackboard['on_exit_state'], 0)

	_root.tick(0.1)
	assert_eq(_root.blackboard['on_enter_condition'], 3)
	assert_eq(_root.blackboard['on_tick'], 0)
	assert_eq(_root.blackboard['on_enter_state'], 0)
	assert_eq(_root.blackboard['on_exit_state'], 0)

func test_grandchildren_on_method_calls() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()
	var _grandchild: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()
	_root.blackboard = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_child.add_child(_grandchild)
	_root.add_child(_child)
	_root.tick(0.1)
	assert_eq(_root.blackboard['on_enter_condition'], 2) # 1 for child, 1 for grandchild
	assert_eq(_root.blackboard['on_tick'], 2)
	assert_eq(_root.blackboard['on_enter_state'], 2)
	assert_eq(_root.blackboard['on_exit_state'], 0)

	_root.tick(0.1)
	assert_eq(_root.blackboard['on_enter_condition'], 2)
	assert_eq(_root.blackboard['on_tick'], 4)
	assert_eq(_root.blackboard['on_enter_state'], 2)
	assert_eq(_root.blackboard['on_exit_state'], 0)

func test_get_root_works_when_child_changed() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _grandchild: NNSTNode = NNSTNode.new()

	_child.add_child(_grandchild)
	_root.add_child(_child)

	assert_eq(_child.get_root(), _root)
	assert_eq(_grandchild.get_root(), _root)

func test_get_root_works_when_grandchild_changed() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _grandchild: NNSTNode = NNSTNode.new()

	_root.add_child(_child)
	_child.add_child(_grandchild)

	assert_eq(_child.get_root(), _root)
	assert_eq(_grandchild.get_root(), _root)

func test_initial_state_works() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child: NNSTNode = NNSTNode.new()
	var _grandchild: NNSTNode = NNSTNode.new()
	var _grandchild2: NNSTNode = NNSTNode.new()

	_grandchild2.name = '_grandchild2'
	_child.initial_state = NodePath('_grandchild2')

	# Hierarchy needs to be set up with initial_state property before
	# root is added to the tree.
	_root.add_child(_child)
	_child.add_child(_grandchild)
	_child.add_child(_grandchild2)
	add_child_autofree(_root)

	assert_eq(_grandchild.get_is_active(), false)
	assert_eq(_grandchild2.get_is_active(), true)

func test_initial_state_only_accepts_immediate_children() -> void:
	var _node: NNSTNode = autofree(NNSTNode.new())

	_node.initial_state = NodePath('foo')
	assert_eq(_node.initial_state, NodePath('foo'))

	_node.initial_state = NodePath('foo/bar')
	assert_eq(_node.initial_state, NodePath(''))

	_node.initial_state = NodePath('.')
	assert_eq(_node.initial_state, NodePath(''))
