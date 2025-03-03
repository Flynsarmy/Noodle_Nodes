# GdUnit generated TestSuite
class_name stnode_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## Confirm on_* methods are called when they should be
func test_on_method_state_calls() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child = load("res://test/state_tree/node_example_1.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	assert(blackboard['on_enter_condition'] == 1)
	assert(blackboard['on_tick'] == 1)
	assert(blackboard['on_enter_state'] == 1)
	assert(blackboard['on_exit_state'] == 0)

	_root.tick(blackboard, 0.1)
	assert(blackboard['on_enter_condition'] == 1)
	assert(blackboard['on_tick'] == 2)
	assert(blackboard['on_enter_state'] == 1)
	assert(blackboard['on_exit_state'] == 0)

	_root.free()

## No on_* methods should fire if on_enter_condition returns false
func test_on_enter_condition_fail_checks() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child = load("res://test/state_tree/node_example_2.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	assert(blackboard['on_enter_condition'] == 1)
	assert(blackboard['on_tick'] == 0)
	assert(blackboard['on_enter_state'] == 0)
	assert(blackboard['on_exit_state'] == 0)

	_root.tick(blackboard, 0.1)
	assert(blackboard['on_enter_condition'] == 2)
	assert(blackboard['on_tick'] == 0)
	assert(blackboard['on_enter_state'] == 0)
	assert(blackboard['on_exit_state'] == 0)
	_root.free()

func test_grandchildren_on_method_calls() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child = load("res://test/state_tree/node_example_1.gd").new()
	var _grandchild = load("res://test/state_tree/node_example_1.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	_child.add_child(_grandchild)
	_root.add_child(_child)
	_root.tick(blackboard, 0.1)
	assert(blackboard['on_enter_condition'] == 2) # 1 for child, 1 for grandchild
	assert(blackboard['on_tick'] == 2)
	assert(blackboard['on_enter_state'] == 2)
	assert(blackboard['on_exit_state'] == 0)

	_root.tick(blackboard, 0.1)
	assert(blackboard['on_enter_condition'] == 2)
	assert(blackboard['on_tick'] == 4)
	assert(blackboard['on_enter_state'] == 2)
	assert(blackboard['on_exit_state'] == 0)

	_root.free()
