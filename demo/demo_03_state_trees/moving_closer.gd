extends NNSTNode

func on_enter_condition() -> bool:
	return get_root().get_parent().sensor_distance.distance > 200.0


func on_enter_state() -> void:
	var actor: AnimatedSprite2D = get_root().get_parent()
	actor.movement_speed = -100
	actor.play("moving")


func on_tick(_delta: float) -> void:
	if get_root().get_parent().sensor_distance.distance <= 200:
		transition_to("../Moving away")
