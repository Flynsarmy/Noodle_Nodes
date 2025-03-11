# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

## _get_active_states() should be 0 before first tick
func test_no_active_children_on_unticked_root() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	assert_eq(_root.get_active_states().size(), 0)
	_root.free()

## _get_active_states() should be 1 after a tick
func test_active_child_on_ticked_root() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child: NNSTNode = NNSTNode.new()
	_root.add_child(_child)
	# before first tick it'll be 0
	assert_eq(_root.get_active_states().size(), 0)
	_root.tick({}, 0.1);
	assert_eq(_root.get_active_states().size(), 1)

	_root.free()

## _get_active_states() includes grandchildren
func test_multiple_active_children_on_ticked_root() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child1: NNSTNode = NNSTNode.new()
	var _child1_1: NNSTNode = NNSTNode.new()
	_child1.add_child(_child1_1)
	_root.add_child(_child1)
	_root.tick({}, 0.1);
	assert_eq(_root.get_active_states().size(), 2)
	_root.free()

## _get_active_states() only changes then transition_to() is called.
func test_active_children_count_changes_on_transition() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child: NNSTNode = NNSTNode.new()
	_root.add_child(_child)
	_root.tick({}, 0.1);
	# Adding a grandchild won't affect active state count even when ticked
	var _grandchild: NNSTNode = NNSTNode.new()
	_child.add_child(_grandchild)
	_root.tick({}, 0.1);
	assert_eq(_root.get_active_states().size(), 1)
	# Transitionining triggers a recount
	_root.transition_to('./', {}, 0.1)
	assert_eq(_root.get_active_states().size(), 2)
	_root.free()


## _get_child_sensors() should match number of active children
func test_child_sensor_count() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child1: NNAngleVector2Sensor = NNAngleVector2Sensor.new()
	var _child2: NNAngleVector2Sensor = NNAngleVector2Sensor.new()
	var _child3: NNAngleVector2Sensor = NNAngleVector2Sensor.new()
#
	assert_eq(_root.get_active_states().size(), 0)
	_root.add_child(_child1)
	assert_eq(_root.get_child_sensors().size(), 1)
	_root.add_child(_child2)
	assert_eq(_root.get_child_sensors().size(), 2)
	## inactive children still get counted
	_child3.is_enabled = false
	_root.add_child(_child3)
	assert_eq(_root.get_child_sensors().size(), 3)
	_root.free()

## when the first child fails its _on_enter_condition() check, the second child should be entered
func test_second_child_selected() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child1 = load("res://test/unit/state_tree/node_example_2.gd").new()
	var _child2 = load("res://test/unit/state_tree/node_example_1.gd").new()
	var blackboard: Dictionary = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}
	_root.add_child(_child1)
	_root.add_child(_child2)

	_root.tick(blackboard, 0.1)
	assert_eq(_root.get_active_states().size(), 1)
	_root.free()
