extends NNBTLeaf

func on_tick(blackboard: Variant, _delta: float) -> Status:
	blackboard.movement_speed = 100.0
	blackboard.play("moving")
	return SUCCESS
