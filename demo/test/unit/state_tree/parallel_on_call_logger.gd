extends NNSTParallel

func on_enter_condition() -> bool:
	get_root().blackboard['log'].push_back("%s on_enter_condition" % self.name)
	return true

func on_enter_state() -> void:
	get_root().blackboard['log'].push_back("%s on_enter_state" % self.name)

func on_exit_state() -> void:
	get_root().blackboard['log'].push_back("%s on_exit_state" % self.name)

func on_tick(_delta: float) -> void:
	get_root().blackboard['log'].push_back("%s on_tick" % self.name)
