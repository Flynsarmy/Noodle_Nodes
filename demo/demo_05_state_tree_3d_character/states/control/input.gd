extends NNSTNode

func on_tick(_delta: float) -> void:
	var input: Vector2 =  Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	# Rotate based on current camera
	var input_dir: Vector3 = (
		get_root().actor.global_transform.basis *
		Vector3(input.x, 0, input.y)
	)
	set_blackboard("input_dir", input_dir.rotated(
		Vector3.UP,
		get_root().actor.camera_arm.global_rotation.y
	))

func on_unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("jump"):
		get_root().movement_state.send_event("movement_jump")
	elif event.is_action_pressed("sprint"):
		set_blackboard('holding_sprint', true)
		get_root().actor.anim_tree['parameters/basic_movement/conditions/holding_sprint'] = true
	elif event.is_action_released("sprint"):
		set_blackboard('holding_sprint', false)
		get_root().actor.anim_tree['parameters/basic_movement/conditions/holding_sprint'] = false
