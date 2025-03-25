extends NNSTNode

func on_tick(_delta: float) -> void:
	if get_blackboard('input_dir', Vector3.ZERO).length() > 0:
		get_root().send_event("movement_run")
		return

	get_root().actor.velocity.x = move_toward(get_root().actor.velocity.x, 0, get_blackboard('acceleration', 0.75))
	get_root().actor.velocity.z = move_toward(get_root().actor.velocity.z, 0, get_blackboard('acceleration', 0.75))
