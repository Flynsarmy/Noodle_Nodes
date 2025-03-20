extends NNSTNode

func on_enter_condition() -> bool:
	get_root().blackboard['on_enter_condition'] += 1
	return false

func on_enter_state() -> void:
	get_root().blackboard['on_enter_state'] += 1

func on_exit_state() -> void:
	get_root().blackboard['on_exit_state'] += 1

func on_tick(_delta: float) -> void:
	get_root().blackboard['on_tick'] += 1
