extends NNBTLeaf

@export var key: String = "custom_value"

func on_tick(blackboard: Variant, delta: float) -> Status:
	blackboard = blackboard as Dictionary
	if not blackboard.has(key):
		blackboard[key] = 0

	blackboard[key] += 1

	return Status.SUCCESS
