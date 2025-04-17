# GdUnit generated TestSuite
extends GutTest

func test_expression_setter_getter() -> void:
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	_guard.expression = "test"

	assert_eq(_guard.expression, "test")

func test_generic_expression_satisfied() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	var result: bool;
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _guard

	_guard.expression = "1==1"
	result = _guard.is_satisfied(_transition, _child)
	assert_eq(result, true)

	_guard.expression = "1==0"
	result = _guard.is_satisfied(_transition, _child)
	assert_eq(result, false)

func test_expression_satisfied_with_blackboard_vars() -> void:
	var _root: NNSTRoot = add_child_autofree(NNSTRoot.new())
	var _child: NNSTNode = NNSTNode.new()
	var _guard: NNSTExpressionGuard = NNSTExpressionGuard.new()
	var _transition: NNSTTransition = NNSTTransition.new()
	var result: bool;
	_root.add_child(_child)
	_child.add_child(_transition)
	_transition.guard = _guard

	var blackboard: Dictionary = {"foo": "bar"}
	_root.blackboard = blackboard

	_guard.expression = "foo=='bar'"
	result = _guard.is_satisfied(_transition, _child)
	assert_eq(result, true)
	_guard.expression = "bar=='foo'"
	result = _guard.is_satisfied(_transition, _child)
	assert_eq(result, false)
