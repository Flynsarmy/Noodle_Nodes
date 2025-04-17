#include "guard.h"
#include <godot_cpp/classes/expression.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

// Method binds.

void NNSTGuard::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_satisfied", "p_transition", "p_state"), &NNSTGuard::is_satisfied);
	// ClassDB::bind_method(D_METHOD("evaluate_expression", "context", "state_chart", "expression", "default_value"), &NNSTGuard::evaluate_expression);
	// ClassDB::bind_method(D_METHOD("path_of", "node"), &NNSTGuard::path_of);
}

bool NNSTGuard::is_satisfied(NNSTTransition *p_transition, NNSTTickedNodes *p_state) {
	print_error("Guard.is_satisfied() is not implemented. Did you forget to override it?");
	return false;
}

Variant NNSTGuard::evaluate_expression(String context, NNSTRoot *state_chart, String expression, Variant default_value) const {
	Ref<Expression> the_expression;
	the_expression.instantiate();
	Dictionary blackboard = state_chart->get_blackboard();
	Array input_names = blackboard.keys();

	int parse_result = the_expression->parse(expression, input_names);

	if (parse_result != OK) {
		ERR_PRINT("(" + context + ") Expression parse error : " + the_expression->get_error_text() + " for expression " + expression);
		return default_value;
	}

	Array input_values = blackboard.values();
	Variant result = the_expression->execute(input_values, Variant(), false);

	if (the_expression->has_execute_failed()) {
		ERR_PRINT("(" + context + ") Expression execute error: " + the_expression->get_error_text() + " for expression: " + expression);
		return default_value;
	}

	return result;
}

String NNSTGuard::path_of(Node *node) const {
	if (node == nullptr) {
		return "";
	}
	if (!node->is_inside_tree()) {
		return node->get_name() + String(" (not in tree)");
	}
	return String(node->get_path());
}
