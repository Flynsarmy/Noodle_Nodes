extends AnimatedSprite2D

# This is the distance sensor.
@onready var ai: NNBTRoot = $NNBTRoot
@onready var sensor_distance: NNDistanceVector2Sensor = $NNBTRoot/NNDistanceVector2Sensor

var movement_speed: float = 0.0
var ticking_delay: float = 0.0


func _ready() -> void:
	ticking_delay = randf() * 0.3

func _physics_process(delta: float) -> void:
	sensor_distance.from_vector = global_position
	sensor_distance.to_vector = get_parent().mouse_position

	# Update the AI.
	ticking_delay -= delta
	if ticking_delay <= 0.0:
		ticking_delay = 0.3 + randf() * 0.3
		ai.tick(self, delta)

	# Move based on movement speed.
	global_position += sensor_distance.direction_vector * movement_speed * delta

	# Flip the sprite horizontally based on the direction vector horizontal (x)
	# value.
	flip_h = (sensor_distance.direction_vector.x < 0)
	# If the movement speed is negative, the entity is moving away so
	# we should flip the sprite again.
	if movement_speed < 0.0:
		flip_h = !flip_h
