# GdUnit generated TestSuite
class_name bt_passthrough_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_returns_child_result() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTPassThrough = NNBTPassThrough.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.SUCCESS)

	_grandchild.fixed_result = NNBTNodes.RUNNING
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.RUNNING)

	_grandchild.fixed_result = NNBTNodes.FAILURE
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.FAILURE)

	_root.free()

func test_returns_own_result_when_childless() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTPassThrough = load("res://test/behaviour_tree/decorators/scripts/fixed_result_pass_through.gd").new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.fixed_result = NNBTNodes.SUCCESS
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.SUCCESS)

	_child.fixed_result = NNBTNodes.RUNNING
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.RUNNING)

	_child.fixed_result = NNBTNodes.FAILURE
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.FAILURE)

	_root.free()
