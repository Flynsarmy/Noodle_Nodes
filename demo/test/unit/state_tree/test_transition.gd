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
