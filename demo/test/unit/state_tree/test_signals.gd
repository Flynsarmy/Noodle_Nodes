# GdUnit generated TestSuite
extends GutTest
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

func test_state_check_enter_condition_affects_entering() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()

	watch_signals(_child)
	_child.state_check_enter_condition.connect(func (): _child.set_on_entered_condition(false))
	_root.add_child(_child)

	assert_signal_emitted(_child, "state_check_enter_condition")
	assert_signal_not_emitted(_child, "state_entered")

	var _root2: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child2: NNSTNode = NNSTNode.new()

	watch_signals(_child2)
	_root2.add_child(_child2)

	assert_signal_emitted(_child2, "state_check_enter_condition")
	assert_signal_emitted(_child2, "state_entered")

func test_state_check_enter_condition_not_called_when_method_exists() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = load("res://test/unit/state_tree/node_enter_condition_success.gd").new()

	_root.blackboard = {
		"on_enter_condition": 0,
		"on_enter_state": 0,
		"on_tick": 0,
		"on_exit_state": 0
	}

	watch_signals(_child)
	_root.add_child(_child)

	assert_signal_not_emitted(_child, "state_check_enter_condition")
