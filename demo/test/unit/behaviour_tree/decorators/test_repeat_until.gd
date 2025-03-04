# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_failure_after_repeats() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTRepeatUntil = NNBTRepeatUntil.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_child.expected_tick_result = NNBTNodes.SUCCESS
	_child.max_repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)
	assert_eq(_child.get_current_repeat_times(), 1)
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	assert_eq(_child.get_current_repeat_times(), 1)
	# This node never resets. always fails
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	assert_eq(_child.get_current_repeat_times(), 1)

	_root.free()

func test_immediate_fail() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTRepeatUntil = NNBTRepeatUntil.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.FAILURE
	_child.expected_tick_result = NNBTNodes.SUCCESS
	_child.max_repeat_times = 2

	#@TODO look into this. Seems wrong
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.RUNNING)
	assert_eq(_child.get_current_repeat_times(), 1)
	_root.free()

#func test_reset_after_complete() -> void:
	#var _root: NNBTRoot = NNBTRoot.new()
	#var _child: NNBTRepeatUntil = NNBTRepeatUntil.new()
	#var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	#var blackboard: Dictionary = {}
	#_child.add_child(_grandchild)
	#_root.add_child(_child)
#
	#_grandchild.fixed_result = NNBTNodes.SUCCESS
	#_child.expected_tick_result = NNBTNodes.SUCCESS
	#_child.max_repeat_times = 2
#
	#_root.tick(blackboard, 0.1)
	#_root.tick(blackboard, 0.1)
	## After max_repeats is reached, reset counters
	#_root.tick(blackboard, 0.1)
	#assert_eq(_root.tick_result, NNBTNodes.RUNNING)
	#assert_eq(_child.get_times_repeated(), 1)
#
	#_root.free()


func test_failure_on_no_children() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTRepeatUntil = NNBTRepeatUntil.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.max_repeat_times = 2

	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	assert_eq(_child.get_current_repeat_times(), 0)
	_root.free()
