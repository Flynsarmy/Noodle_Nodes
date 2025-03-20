# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## _get_active_states() should be 0 before first tick
func test_inactive_until_ticked() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	assert_eq(_root.internal_status, 0)
	_root.tick(0.1)
	assert_eq(_root.internal_status, 1)

## _get_active_states() should be 1 after a tick
func test_1_active_child_on_ticked_root() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _child2: NNSTNode = NNSTNode.new()
	_root.add_child(_child)
	_root.add_child(_child2)
	# before first tick it'll be 0
	assert_eq(_child.internal_status, 0)
	_root.tick(0.1);
	assert_eq(_child.internal_status, 1)
	assert_eq(_child2.internal_status, 0)

## _get_child_sensors() should match number of active children
func test_child_sensor_count() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child1: NNAngleVector2Sensor = NNAngleVector2Sensor.new()
	var _child2: NNAngleVector2Sensor = NNAngleVector2Sensor.new()
	var _child3: NNAngleVector2Sensor = NNAngleVector2Sensor.new()
#
	assert_eq(_root.get_child_sensors().size(), 0)
	_root.add_child(_child1)
	assert_eq(_root.get_child_sensors().size(), 1)
	_root.add_child(_child2)
	assert_eq(_root.get_child_sensors().size(), 2)
	## inactive children still get counted
	_child3.is_enabled = false
	_root.add_child(_child3)
	assert_eq(_root.get_child_sensors().size(), 3)

## when the first child fails its _on_enter_condition() check, the second child should be entered
func test_on_enter_condition_works_on_children() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _child1: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_fail.gd").new()
	var _child2: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()
	_root.blackboard = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}
	_root.add_child(_child1)
	_root.add_child(_child2)

	_root.tick(0.1)
	assert_eq(_child1.internal_status, 0)
	assert_eq(_child2.internal_status, 1)
