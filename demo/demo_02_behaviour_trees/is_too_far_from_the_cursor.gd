extends NNBTLeaf

func on_tick(blackboard: Variant, _delta: float) -> Status:
		# Check if the distance is greater than expected.
	if blackboard.sensor_distance.distance > 150.0:
		return SUCCESS # Success = Far
	return FAILURE # Failure = Not far
