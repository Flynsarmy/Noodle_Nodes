extends NNSTRoot

@onready var actor: CharacterBody3D = get_parent()
@onready var movement_state : NNSTNode = $Root/Movement

func _physics_process(delta: float) -> void:
	tick(delta)
