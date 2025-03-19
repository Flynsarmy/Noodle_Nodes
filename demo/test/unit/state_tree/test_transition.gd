# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_state_transitions_successfully() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()

	_root.add_child(_node1)
	_root.add_child(_node2)
	_node1.add_child(_transition)

	_transition.event_name = 'test'
	_transition.to = _transition.get_path_to(_node2)

	_root.tick(_root, 0.1)
	_root.send_event('test', _root, 0.1)

	assert_eq(_node1.internal_status, 0)
	assert_eq(_node2.internal_status, 1)


func test_events_are_case_sensitive() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()

	_root.add_child(_node1)
	_root.add_child(_node2)
	_node1.add_child(_transition)

	_transition.event_name = 'test'
	_transition.to = _transition.get_path_to(_node2)

	_root.tick(_root, 0.1)
	_root.send_event('Test', _root, 0.1)

	assert_eq(_node1.internal_status, 1)
	assert_eq(_node2.internal_status, 0)

func test_non_existant_event_does_nothing() -> void:
	var _root: NNSTRoot = autofree(NNSTRoot.new())
	var _node1: NNSTNode = NNSTNode.new()
	var _node2: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()

	_root.add_child(_node1)
	_root.add_child(_node2)
	_node1.add_child(_transition)

	_transition.event_name = 'foo'
	_transition.to = _transition.get_path_to(_node2)

	_root.tick(_root, 0.1)
	_root.send_event('test', _root, 0.1)

	assert_eq(_node1.internal_status, 1)
	assert_eq(_node2.internal_status, 0)
