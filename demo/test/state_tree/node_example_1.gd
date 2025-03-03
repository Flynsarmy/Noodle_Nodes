extends NNSTNode

func on_enter_condition(blackboard: Variant, _delta: float) -> bool:
	blackboard = blackboard as Dictionary
	blackboard['on_enter_condition'] += 1
	return true

func on_enter_state(blackboard: Variant, _delta: float) -> void:
	blackboard = blackboard as Dictionary
	blackboard['on_enter_state'] += 1

func on_exit_state(blackboard: Variant, _delta: float) -> void:
	blackboard = blackboard as Dictionary
	blackboard['on_exit_state'] += 1

func on_tick(blackboard: Variant, _delta: float) -> void:
	blackboard = blackboard as Dictionary
	blackboard['on_tick'] += 1
