# GdUnit generated TestSuite
extends GutTest

func test_empty_state_succeeds() -> void:
	var _state: NNSTNode = add_child_autofree(NNSTNode.new())
	var _transition: NNSTTransition = NNSTTransition.new()
	var _guard: NNSTStateIsActiveGuard = NNSTStateIsActiveGuard.new()

	_state.add_child(_transition)
	_transition.guard = _guard

	var result: bool = _guard.is_satisfied(_transition, _state)

	assert_eq(result, true)


func test_active_state_returns_true() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	var _guard: NNSTStateIsActiveGuard = NNSTStateIsActiveGuard.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _guard
	_guard.state = _transition.get_path_to(_child)

	var result: bool = _guard.is_satisfied(_transition, _child)
	assert_eq(result, true)


func test_inactive_state_returns_false() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _inactive_child: NNSTNode = NNSTNode.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	var _guard: NNSTStateIsActiveGuard = NNSTStateIsActiveGuard.new()
	_root.add_child(_child)
	_root.add_child(_inactive_child)
	_child.add_child(_transition)
	_transition.guard = _guard
	_guard.state = _transition.get_path_to(_inactive_child)

	var result: bool = _guard.is_satisfied(_transition, _child)
	assert_eq(result, false)
