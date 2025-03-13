extends NNSTParallel

func on_enter_condition(blackboard: Variant, _delta: float) -> bool:
	blackboard = blackboard as Array[String]
	blackboard.push_back("%s on_enter_condition" % self.name)
	return true

func on_enter_state(blackboard: Variant, _delta: float) -> void:
	blackboard = blackboard as Array[String]
	blackboard.push_back("%s on_enter_state" % self.name)

func on_exit_state(blackboard: Variant, _delta: float) -> void:
	blackboard = blackboard as Array[String]
	blackboard.push_back("%s on_exit_state" % self.name)

func on_tick(blackboard: Variant, _delta: float) -> void:
	blackboard = blackboard as Array[String]
	blackboard.push_back("%s on_tick" % self.name)
