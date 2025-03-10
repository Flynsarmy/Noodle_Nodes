extends NNSTNode


func on_enter_condition(blackboard: Variant, _delta: float) -> bool:
	return blackboard.sensor_distance.distance < 90.0


func on_enter_state(blackboard: Variant, _delta: float) -> void:
	blackboard.movement_speed = 100
	blackboard.play("moving")


func on_tick(blackboard: Variant, delta: float) -> void:
	if blackboard.sensor_distance.distance >= 90 :
		transition_to(".", blackboard, delta)
