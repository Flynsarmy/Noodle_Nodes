extends CharacterBody3D

@onready var visuals: Node3D = $gobot
@onready var anim_tree: AnimationTree = $AnimationTree
@onready var camera_arm: SpringArm3D = $CameraArm

func get_horizontal_velocity() -> Vector2:
	return Vector2(velocity.x, velocity.z)
