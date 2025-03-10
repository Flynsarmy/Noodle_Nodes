extends NNBTLeaf

func on_tick(blackboard: Variant, _delta: float) -> Status:
	# Check if the distance is less than expected.
	if blackboard.sensor_distance.distance < 90:
		return SUCCESS # Success = Too close
	return FAILURE # Failure = Not too close
