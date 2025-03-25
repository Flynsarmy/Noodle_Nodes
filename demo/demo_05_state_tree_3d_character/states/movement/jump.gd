extends NNSTNode

@export var jump_velocity: float = 10

func on_tick(_delta: float) -> void:
	get_root().actor.anim_tree['parameters/jump_oneshot/request'] = AnimationNodeOneShot.ONE_SHOT_REQUEST_FIRE

	get_root().actor.velocity.y = jump_velocity

	get_root().movement_state.send_event("movement_airborn")
