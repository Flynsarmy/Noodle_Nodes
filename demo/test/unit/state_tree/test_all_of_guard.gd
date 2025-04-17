# GdUnit generated TestSuite
extends GutTest

func test_empty_guard_should_return_true() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _all_of_guard: NNSTAllOfGuard = NNSTAllOfGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _all_of_guard

	var result: bool = _all_of_guard.is_satisfied(_transition, _child)
	assert_eq(result, true)

func test_single_failing_expression_should_return_false() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _all_of_guard: NNSTAllOfGuard = NNSTAllOfGuard.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _all_of_guard

	_guard.expression = "1 == 0"
	_all_of_guard.guards.push_back(_guard)
	var result: bool = _all_of_guard.is_satisfied(_transition, _child)
	assert_eq(result, false)

func test_single_succeeding_expression_should_return_true() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _all_of_guard: NNSTAllOfGuard = NNSTAllOfGuard.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _all_of_guard

	_guard.expression = "1 == 1"
	_all_of_guard.guards.push_back(_guard)
	var result: bool = _all_of_guard.is_satisfied(_transition, _child)
	assert_eq(result, true)

func test_all_succeeding_expression_should_return_true() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _all_of_guard: NNSTAllOfGuard = NNSTAllOfGuard.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _guard2: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _all_of_guard

	_guard.expression = "1 == 1"
	_guard2.expression = "2 == 2"
	_all_of_guard.guards.push_back(_guard)
	_all_of_guard.guards.push_back(_guard2)
	var result: bool = _all_of_guard.is_satisfied(_transition, _child)
	assert_eq(result, true)

func test_some_failing_expressions_should_return_false() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _all_of_guard: NNSTAllOfGuard = NNSTAllOfGuard.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _guard2: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _all_of_guard

	_guard.expression = "1 == 1"
	_guard2.expression = "2 == 1"
	_all_of_guard.guards.push_back(_guard)
	_all_of_guard.guards.push_back(_guard2)
	var result: bool = _all_of_guard.is_satisfied(_transition, _child)
	assert_eq(result, false)
