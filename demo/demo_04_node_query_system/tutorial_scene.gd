extends Node2D

@onready var ai_entity_template: PackedScene = preload("ai_entity.tscn")
@onready var mouse_point_grid: Node2D = $mouse_point_grid

var mouse_position: Vector2


# Called when the node enters the scene tree for the first time.
func _ready() -> void:

	# To see how the NQS is running, we initialize the performance counters.
	NodeQuerySystem.initialize_performance_counters()
	# We won't be using regular queries, so allocate 100% of the time budget to
	# the high priority queries.
	NodeQuerySystem.set_time_allocation_pct_to_high_priority_queries(1.0)

	# For convinience in changing the number of AI-entities to create
	var num_entities: int = 250

	# Create all the entities.
	for i in range(0, num_entities):
		# Instantiates an AI-entity.
		var new_ai_entity: Node2D = ai_entity_template.instantiate()
		# Sets a random position somewhere on the screen for the AI-entity.
		new_ai_entity.position = Vector2( randf() * get_viewport_rect().end.x, randf() * get_viewport_rect().end.y)

		# Adds the AI-entity to the scene.
		add_child(new_ai_entity)


func _physics_process(_delta: float) -> void:
	# This will run the node queries that have been posted by the ai entities.
	NodeQuerySystem.run_queries()

	# Set the mouse cursor position as the to-vector.
	mouse_position = get_viewport().get_mouse_position()

	# Move the point grid to the mouse position.
	mouse_point_grid.position = mouse_position
