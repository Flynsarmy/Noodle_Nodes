extends Node2D

@onready var ai_entity_template: PackedScene = preload("res://demo_01_ai_behaviours/ai_entity.tscn")
var mouse_position: Vector2


func _ready() -> void:
	randomize()
	# For convinience in changing the number of AI-entities to create
	var num_entities: int = 100

	# Create all the entities.
	for i in range(0, num_entities):
		# Instantiates an AI-entity.
		var new_ai_entity: Node2D = ai_entity_template.instantiate()
		# Sets a random position somewhere on the screen for the AI-entity.
		new_ai_entity.position = Vector2(
			randf() * get_viewport_rect().end.x,
			randf() * get_viewport_rect().end.y
		)
		# Adds the AI-entity to the scene.
		add_child(new_ai_entity)


func _physics_process(delta: float) -> void:
	# Set the mouse cursor position as the to-vector.
	mouse_position = get_viewport().get_mouse_position()
