# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_fail_after_repeats() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTLimiter = NNBTLimiter.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_child.max_repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)

	_root.free()

func test_reset_after_complete() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTLimiter = NNBTLimiter.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_child.max_repeat_times = 1

	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)

	_root.free()

func test_failure_on_no_children() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTLimiter = NNBTLimiter.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	_root.free()
