extends AnimatedSprite2D

# This is the distance sensor.
@onready var agent: NNAgent = $NNAgent
@onready var mouse_sensor: NNDistanceVector2Sensor = $NNAgent/MouseSensor
@onready var box_sensor: NNDistanceVector2Sensor = $NNAgent/BoxSensor
var movement_speed: float = 0.0


func _physics_process(delta: float) -> void:
	mouse_sensor.from_vector = global_position
	mouse_sensor.to_vector = get_parent().mouse_position

	box_sensor.from_vector = global_position
	box_sensor.to_vector = _get_closest_box_pos()

	# Update the AI.
	agent.evaluate_options(delta)

	# Move based on movement speed.
	global_position += mouse_sensor.direction_vector * movement_speed * delta

	# Flip the sprite horizontally based on the direction vector horizontal (x)
	# value.
	flip_h = (mouse_sensor.direction_vector.x < 0)
	# If the movement speed is negative, the entity is moving away so
	# we should flip the sprite again.
	if movement_speed < 0.0:
		flip_h = !flip_h


func _on_nn_agent_behaviour_changed(behaviour_node: NNBehaviour) -> void:
	if behaviour_node == null:
		return

	if behaviour_node.name == "Approach":
		movement_speed = -100
		play("moving")
	elif behaviour_node.name == "Flee":
		movement_speed = 100
		play("moving")
	else:
		movement_speed = 0
		play("default")

# Returns global position of closest box
func _get_closest_box_pos() -> Vector2:
	var boxes: Array[Node] = get_tree().get_nodes_in_group('avoider_box')

	var min_dist: float = 9999
	var closest: Vector2
	for box in boxes:
		box = box as MeshInstance2D
		var dist: float = box.global_position.distance_to(global_position)
		if dist < min_dist:
			min_dist = dist
			closest = box.global_position

	return closest
