# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_highest_scoring_child_of_root_is_entered() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child1: NNSTNode = NNSTNode.new()
	var _child2: NNSTNode = NNSTNode.new()
	var _consideration: NNConsideration = NNConsideration.new()

	_root.child_state_selection_rule = 1
	_consideration.activation_input_value = 0.5

	assert_eq(_child2.score, 0)

	# Hierarchy needs to be set up before adding to the tree or this
	# test won't work correctly.
	_root.add_child(_child1)
	_root.add_child(_child2)
	_child2.add_child(_consideration)
	add_child_autofree(_root)

	assert_eq(_child1.get_is_active(), false)
	assert_eq(_child2.get_is_active(), true)
	assert_eq(_child2.score, 0.5)

func test_highest_scoring_child_of_node_is_entered() -> void:
	var _root: NNSTRoot = NNSTRoot.new()
	var _child: NNSTNode = NNSTNode.new()
	var _grandchild1: NNSTNode = NNSTNode.new()
	var _grandchild2: NNSTNode = NNSTNode.new()
	var _consideration: NNConsideration = NNConsideration.new()

	_child.child_state_selection_rule = 1
	_consideration.activation_input_value = 0.5

	assert_eq(_grandchild2.score, 0)

	# Hierarchy needs to be set up before adding to the tree or this
	# test won't work correctly.
	_root.add_child(_child)
	_child.add_child(_grandchild1)
	_child.add_child(_grandchild2)
	_grandchild2.add_child(_consideration)
	add_child_autofree(_root)

	assert_eq(_grandchild1.get_is_active(), false)
	assert_eq(_grandchild2.get_is_active(), true)
	assert_eq(_grandchild2.score, 0.5)
