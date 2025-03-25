extends NNSTNode

@export var terminal_velocity: float = 100

func on_tick(delta: float) -> void:
	get_root().actor.velocity += get_root().actor.get_gravity() * delta

	# Apply terminal velocity
	get_root().actor.velocity.y = max(get_root().actor.velocity.y, -terminal_velocity)
