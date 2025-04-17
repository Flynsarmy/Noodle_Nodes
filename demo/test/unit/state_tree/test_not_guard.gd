# GdUnit generated TestSuite
extends GutTest

func test_empty_guard_should_return_true() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _not_guard: NNSTNotGuard = NNSTNotGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _not_guard

	var result: bool = _not_guard.is_satisfied(_transition, _child)
	assert_eq(result, true)

func test_succeeding_guard_should_return_false() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _not_guard: NNSTNotGuard = NNSTNotGuard.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _not_guard

	_guard.expression = "1 == 1"
	_not_guard.guard = _guard
	var result: bool = _not_guard.is_satisfied(_transition, _child)
	assert_eq(result, false)

func test_failing_guard_should_return_true() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _not_guard: NNSTNotGuard = NNSTNotGuard.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _not_guard

	_guard.expression = "1 == 0"
	_not_guard.guard = _guard
	var result: bool = _not_guard.is_satisfied(_transition, _child)
	assert_eq(result, true)
