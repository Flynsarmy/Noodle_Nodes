extends NNSTNode

@export var speed: float = 5.0
@export var sprint_speed: float = 7.0

func on_tick(delta: float) -> void:
	var input_dir: Vector3 = get_blackboard('input_dir', Vector3.ZERO)
	var target_speed: float

	if not input_dir.length():
		get_root().movement_state.send_event("movement_idle")
		return
	else:
		target_speed = speed if not get_blackboard('holding_sprint', false) else sprint_speed

	var velocity: Vector3 = input_dir * target_speed

	# Don't slow down, only speed up
	get_root().actor.velocity.x = lerp(get_root().actor.velocity.x, velocity.x, get_blackboard('acceleration', 0.75))
	get_root().actor.velocity.z = lerp(get_root().actor.velocity.z, velocity.z, get_blackboard('acceleration', 0.75))

	get_root().movement_state.rotate_towards_input(delta)
