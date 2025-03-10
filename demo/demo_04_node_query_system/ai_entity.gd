extends AnimatedSprite2D

@onready var root: NNBTRoot = $NNBTRoot
@onready var distance_to_vector_2_search_criterion: NNDistanceToVector2SearchCriterion = $NNNodeGroupSearchSpace/NNDistanceToVector2SearchCriterion
@onready var debug_circle: MeshInstance2D = $debug_circle


var movement_speed: float = 100.0
var movement_target_point: Vector2
var dir_to_target: Vector2


func _physics_process(delta: float) -> void:
	# Set the AI-entity position as the from-vector.
	distance_to_vector_2_search_criterion.distance_to_vector = get_parent().mouse_position

	# Update the AI.
	root.tick(self, delta)

	# Move based on movement speed.
	global_position += dir_to_target * movement_speed * delta
	if (global_position - movement_target_point).length() < 10.0:
		movement_speed = 0.0
	else:
		movement_speed = 100.0

	# Flip the sprite horizontally based on the direction vector horizontal (x)
	# value.
	flip_h = (dir_to_target.x < 0)
	# If the movement speed is negative, the entity is moving away so
	# we should flip the sprite again.
	if movement_speed < 0.0:
		flip_h = !flip_h

	debug_circle.global_position = movement_target_point


func _on_nn_node_group_search_space_query_completed(search_space: Object) -> void:
	if search_space.query_results.size() == 0:
		return
	movement_target_point = search_space.query_results[0].global_position
	dir_to_target = (movement_target_point - global_position).normalized()
