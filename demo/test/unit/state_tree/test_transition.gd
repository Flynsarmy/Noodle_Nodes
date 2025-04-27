# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_state_transitions_successfully() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()

	_root.add_child(_node1)
	_root.add_child(_node2)
	_node1.add_child(_transition)

	_transition.event_name = 'test'
	_transition.to = _transition.get_path_to(_node2)

	_root.tick(0.1)
	_root.send_event('test')

	assert_eq(_node1.get_is_active(), false)
	assert_eq(_node2.get_is_active(), true)


func test_events_are_case_sensitive() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()

	_root.add_child(_node1)
	_root.add_child(_node2)
	_node1.add_child(_transition)

	_transition.event_name = 'test'
	_transition.to = _transition.get_path_to(_node2)

	_root.tick(0.1)
	_root.send_event('Test')

	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

func test_non_existant_event_does_nothing() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()

	_root.add_child(_node1)
	_root.add_child(_node2)
	_node1.add_child(_transition)

	_transition.event_name = 'foo'
	_transition.to = _transition.get_path_to(_node2)

	_root.tick(0.1)
	_root.send_event('test')

	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

func test_transition_with_succeeding_guard() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()

	_node1.add_child(_transition)
	_root.add_child(_node1)
	_root.add_child(_node2)

	_transition.event_name = 'foo'
	_transition.to = _transition.get_path_to(_node2)
	_transition.guard = _guard
	_guard.expression = "1==1"

	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

	_root.send_event('foo')

	assert_eq(_node1.get_is_active(), false)
	assert_eq(_node2.get_is_active(), true)

func test_transition_with_failing_guard() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()

	_node1.add_child(_transition)
	_root.add_child(_node1)
	_root.add_child(_node2)

	_transition.event_name = 'foo'
	_transition.to = _transition.get_path_to(_node2)
	_transition.guard = _guard
	_guard.expression = "1==0"

	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

	_root.send_event('foo')

	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

## This test ensures active child counts don't screw up when transitioning in
## and out of grandchild states repeatedly.
func test_transition_into_and_out_of_grandchild_twice() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node11: NNSTNode = NNSTNode.new()
	var _node11_transition: NNSTTransition = NNSTTransition.new()
	_node11_transition.event_name = "to_node2"
	var _node2: NNSTNode = NNSTNode.new()
	var _node2_transition: NNSTTransition = NNSTTransition.new()
	_node2_transition.event_name = "to_node11"

	_node2.add_child(_node2_transition)
	_node11.add_child(_node11_transition)
	_node1.add_child(_node11)
	_root.add_child(_node1)
	_root.add_child(_node2)
	_node2_transition.to = _node2_transition.get_path_to(_node11)
	_node11_transition.to = _node11_transition.get_path_to(_node2)

	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node11.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

	_root.send_event('to_node2')
	assert_eq(_node1.get_is_active(), false)
	assert_eq(_node11.get_is_active(), false)
	assert_eq(_node2.get_is_active(), true)

	_root.send_event('to_node11')
	assert_eq(_node1.get_is_active(), true)
	assert_eq(_node11.get_is_active(), true)
	assert_eq(_node2.get_is_active(), false)

	_root.send_event('to_node2')
	assert_eq(_node1.get_is_active(), false)
	assert_eq(_node11.get_is_active(), false)
	assert_eq(_node2.get_is_active(), true)
