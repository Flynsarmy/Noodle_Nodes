extends NNSTNode

func on_tick(_delta: float) -> void:
	var sensor_distance: float = get_root().get_parent().sensor_distance.distance
	if sensor_distance > 90 and sensor_distance < 200:
		transition_to("../Waiting")
