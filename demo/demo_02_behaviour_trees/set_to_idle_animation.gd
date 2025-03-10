extends NNBTLeaf

func on_tick(blackboard: Variant, _delta: float) -> Status:
	blackboard.movement_speed = 0.0
	blackboard.play("default")
	return SUCCESS
