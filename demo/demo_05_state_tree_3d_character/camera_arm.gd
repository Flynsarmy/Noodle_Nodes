extends SpringArm3D

@export var mouse_sensitivity: float = 0.05
@export var yaw_constraint: Vector2 = Vector2(-89.9, 50)
@export var pitch_constraint: Vector2 = Vector2(0, 360)

func _ready() -> void:
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED

func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseMotion:
		global_rotation_degrees.x -= event.relative.y * mouse_sensitivity
		global_rotation_degrees.x = clampf(global_rotation_degrees.x, yaw_constraint.x, yaw_constraint.y)
		global_rotation_degrees.y -= event.relative.x * mouse_sensitivity
		global_rotation_degrees.y = wrapf(global_rotation_degrees.y, pitch_constraint.x, pitch_constraint.y)
