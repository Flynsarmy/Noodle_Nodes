# GdUnit generated TestSuite
class_name bt_cooldown_msec_test
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_fail_until_cooldown_expires() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTCooldownMsec = NNBTCooldownMsec.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_child.cooldown_msec = 100
	_child.cooldown_return_value = NNBTNodes.FAILURE

	# First tick should work
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.SUCCESS)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.FAILURE)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.FAILURE)
	await get_tree().create_timer(.2).timeout
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.SUCCESS)


	_root.free()

func test_dont_cooldown_on_running_child() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTCooldownMsec = NNBTCooldownMsec.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()
	var blackboard: Dictionary = {}
	_child.add_child(_grandchild)
	_root.add_child(_child)

	_grandchild.fixed_result = NNBTNodes.RUNNING
	_child.cooldown_msec = 100
	_child.cooldown_return_value = NNBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.RUNNING)
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.RUNNING)

	# Make grandhcild return success and confirm we go on cooldown
	_grandchild.fixed_result = NNBTNodes.SUCCESS
	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.SUCCESS)
	_root.free()

func test_failure_on_no_children() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTCooldownMsec = NNBTCooldownMsec.new()
	var blackboard: Dictionary = {}
	_root.add_child(_child)

	_child.cooldown_msec = 100
	_child.cooldown_return_value = NNBTNodes.FAILURE

	_root.tick(blackboard, 0.1)
	assert(_root.tick_result == NNBTNodes.FAILURE)
	_root.free()
