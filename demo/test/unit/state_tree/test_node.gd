# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## Confirm on_* methods are called when they should be
func test_on_method_state_calls() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	assert_eq(blackboard['on_enter_condition'], 1)
	assert_eq(blackboard['on_tick'], 1)
	assert_eq(blackboard['on_enter_state'], 1)
	assert_eq(blackboard['on_exit_state'], 0)

	_root.tick(blackboard, 0.1)
	assert_eq(blackboard['on_enter_condition'], 1)
	assert_eq(blackboard['on_tick'], 2)
	assert_eq(blackboard['on_enter_state'], 1)
	assert_eq(blackboard['on_exit_state'], 0)

	_root.free()

## No on_* methods should fire if on_enter_condition returns false
func test_on_enter_condition_fail_checks() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_fail.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	assert_eq(blackboard['on_enter_condition'], 1)
	assert_eq(blackboard['on_tick'], 0)
	assert_eq(blackboard['on_enter_state'], 0)
	assert_eq(blackboard['on_exit_state'], 0)

	_root.tick(blackboard, 0.1)
	assert_eq(blackboard['on_enter_condition'], 2)
	assert_eq(blackboard['on_tick'], 0)
	assert_eq(blackboard['on_enter_state'], 0)
	assert_eq(blackboard['on_exit_state'], 0)
	_root.free()

func test_grandchildren_on_method_calls() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()
	var _grandchild: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_child.add_child(_grandchild)
	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	assert_eq(blackboard['on_enter_condition'], 2) # 1 for child, 1 for grandchild
	assert_eq(blackboard['on_tick'], 2)
	assert_eq(blackboard['on_enter_state'], 2)
	assert_eq(blackboard['on_exit_state'], 0)

	_root.tick(blackboard, 0.1)
	assert_eq(blackboard['on_enter_condition'], 2)
	assert_eq(blackboard['on_tick'], 4)
	assert_eq(blackboard['on_enter_state'], 2)
	assert_eq(blackboard['on_exit_state'], 0)

	_root.free()
