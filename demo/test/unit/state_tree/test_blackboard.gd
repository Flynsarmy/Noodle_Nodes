# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_get_blackboard_works() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node: NNSTNode = NNSTNode.new()

	_root.add_child(_node)
	_root.blackboard['existing_key'] = 'existing_value'

	assert_eq(_node.get_blackboard('missing_key'), null)
	assert_eq(_node.get_blackboard('missing_key', 'default_value'), 'default_value')
	assert_eq(_node.get_blackboard('existing_key'), 'existing_value')

	_root.blackboard['missing_key'] = 'newly_added'
	assert_eq(_node.get_blackboard('missing_key'), 'newly_added')

func test_set_blackboard_works() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _node: NNSTNode = NNSTNode.new()

	_root.add_child(_node)

	assert_true(not _root.blackboard.has('missing_key'))
	_node.set_blackboard('missing_key', 'some_value')
	assert_eq(_root.blackboard.get('missing_key'), 'some_value')

func test_blackboard_methods_throw_error_on_unready_node() -> void:
	var _node: NNSTNode = add_child_autofree(NNSTNode.new())

	assert_eq(_node.get_blackboard('missing_key'), null)
	assert_eq(_node.get_blackboard('missing_key', 'some_value'), 'some_value')
