extends NNSTNode

func _on_state_entered() -> void:
	set_blackboard("input_dir", Vector3.ZERO)
