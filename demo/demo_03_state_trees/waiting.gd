extends NNSTNode


func on_enter_state(blackboard: Variant, _delta: float) -> void:
	blackboard.movement_speed = 0
	blackboard.play("default")


func on_tick(blackboard: Variant, delta: float) -> void:
	if blackboard.sensor_distance.distance > 200 or blackboard.sensor_distance.distance < 90:
		transition_to("Moving", blackboard, delta)
