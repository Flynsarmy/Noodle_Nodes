# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_get_tree_root() -> void:
	var _root: NNBTRoot = NNBTRoot.new()
	var _child: NNBTCooldownMsec = NNBTCooldownMsec.new()
	var _grandchild: NNBTFixedResult = NNBTFixedResult.new()

	_child.add_child(_grandchild)
	_root.add_child(_child)

	assert_eq(_child.get_tree_root(), NodePath(".."))
	assert_eq(_grandchild.get_tree_root(), NodePath("../.."))

	# Repeat callings should also work
	assert_eq(_child.get_tree_root(), NodePath(".."))
	assert_eq(_grandchild.get_tree_root(), NodePath("../.."))

	# Roots should return itself
	assert_eq(_root.get_tree_root(), NodePath("."))

	# Orphaned nodes should return themself
	var _orphan: NNBTCooldownMsec = NNBTCooldownMsec.new()
	assert_eq(_orphan.get_tree_root(), NodePath("."))

	_root.free()
	_orphan.free()
