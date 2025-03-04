# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_fail_until_repeats() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTCooldownTicks = NNBTCooldownTicks.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_child.cooldown_ticks = 2
	_child.cooldown_return_value = NNBTNodes.FAILURE

	# First tick should work
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)
	assert_eq(_child.current_cooldown_ticks, 0)
	# Second tick it's on cooldown
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	assert_eq(_child.current_cooldown_ticks, 1)
	# Third tick it should still be on cooldown
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	assert_eq(_child.current_cooldown_ticks, 2)
	# Fourth tick it should be off cooldown again
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)
	assert_eq(_child.current_cooldown_ticks, 0)

	_root.free()

func test_dont_cooldown_on_running_child() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTCooldownTicks = NNBTCooldownTicks.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.RUNNING
	_child.cooldown_ticks = 2
	_child.cooldown_return_value = NNBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.RUNNING)
	assert_eq(_child.current_cooldown_ticks, 2)
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.RUNNING)
	assert_eq(_child.current_cooldown_ticks, 2)

	# Make grandhcild return success and confirm we go on cooldown
	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.SUCCESS)
	assert_eq(_child.current_cooldown_ticks, 0)
	_root.free()

func test_failure_on_no_children() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTCooldownTicks = NNBTCooldownTicks.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.cooldown_ticks = 2
	_child.cooldown_return_value = NNBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert_eq(_root.tick_result, NNBTNodes.FAILURE)
	assert_eq(_child.current_cooldown_ticks, 0)
	_root.free()
