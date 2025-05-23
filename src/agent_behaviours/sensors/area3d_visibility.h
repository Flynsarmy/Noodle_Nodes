#pragma once

#include "../sensor.h"
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class NNArea3DVisibilitySensor : public NNSensor {
	GDCLASS(NNArea3DVisibilitySensor, NNSensor)

private:
	Area3D *_visibility_volume;
	ObjectID _cache;

	Vector3 _from_vector;
	Vector3 _offset_vector;

	TypedArray<RID> _occlusion_test_exclusion_list;
	TypedArray<Area3D> _intersecting_areas;
	TypedArray<Node3D> _intersecting_bodies;
	TypedArray<Area3D> _unoccluded_areas;
	TypedArray<Node3D> _unoccluded_bodies;
	TypedArray<float> _squared_distances_to_intersecting_areas;
	TypedArray<float> _squared_distances_to_unoccluded_areas;
	TypedArray<float> _squared_distances_to_intersecting_bodies;
	TypedArray<float> _squared_distances_to_unoccluded_bodies;
	int _closest_intersecting_area_index;
	int _closest_unoccluded_area_index;
	int _closest_intersecting_body_index;
	int _closest_unoccluded_body_index;

	int _expected_number_of_areas_to_track;
	int _expected_number_of_bodies_to_track;
	bool _do_occlusion_test;
	uint32_t _collision_mask;

	int _num_entities_found;
	int _max_expected_entities_found;
	float _one_over_max_expected_entities_found;

	bool _use_owner_global_position;

protected:
	static void _bind_methods();
	void _update_cache();

public:
	NNArea3DVisibilitySensor();
	~NNArea3DVisibilitySensor();

	// Godot virtuals.
	//void _ready();
	//void _notification( int p_what );

	// Handling functions.
	virtual void initialize_sensor() override;
	virtual void uninitialize_sensor() override;

	virtual float evaluate_sensor_value() override;

	void on_area_entered(Area3D *area);
	void on_area_exited(Area3D *area);
	void on_body_entered(Node3D *body);
	void on_body_exited(Node3D *body);

	// Getters and setters for attributes.

	// Configuration values.

	void set_use_owner_global_position(bool use_owner_global_position);
	bool get_use_owner_global_position() const;

	void set_from_vector3(Vector3 from);
	Vector3 get_from_vector3() const;

	void set_offset_vector3(Vector3 offset);
	Vector3 get_offset_vector3() const;

	void set_visibility_volume(Area3D *visibility_volume);
	Area3D *get_visibility_volume() const;

	void set_do_occlusion_test(bool do_occlusion_test);
	bool get_do_occlusion_test() const;

	void set_collision_mask(uint32_t collision_mask);
	uint32_t get_collision_mask() const;

	void set_max_expected_entities_found(int max_expected_entities_found);
	int get_max_expected_entities_found() const;

	void set_occlusion_test_exclusion_list(TypedArray<RID> occlusion_test_exclusion_list);
	TypedArray<RID> get_occlusion_test_exclusion_list() const;

	// Debugging / current values.

	void set_num_entities_found(int num_entities_found);
	int get_num_entities_found() const;

	void set_closest_intersecting_area_index(int closest_intersecting_area_index);
	int get_closest_intersecting_area_index() const;
	void set_closest_intersecting_body_index(int closest_intersecting_body_index);
	int get_closest_intersecting_body_index() const;

	void set_closest_unoccluded_area_index(int closest_unoccluded_area_index);
	int get_closest_unoccluded_area_index() const;
	void set_closest_unoccluded_body_index(int closest_unoccluded_body_index);
	int get_closest_unoccluded_body_index() const;

	void set_intersecting_areas(TypedArray<Area3D> intersecting_areas);
	TypedArray<Area3D> get_intersecting_areas() const;

	void set_intersecting_bodies(TypedArray<Node3D> intersecting_bodies);
	TypedArray<Node3D> get_intersecting_bodies() const;

	void set_unoccluded_areas(TypedArray<Area3D> unoccluded_areas);
	TypedArray<Area3D> get_unoccluded_areas() const;

	void set_unoccluded_bodies(TypedArray<Node3D> unoccluded_bodies);
	TypedArray<Node3D> get_unoccluded_bodies() const;

	void set_squared_distances_to_intersecting_areas(TypedArray<float> squared_distances_to_intersecting_areas);
	TypedArray<float> get_squared_distances_to_intersecting_areas() const;

	void set_squared_distances_to_intersecting_bodies(TypedArray<float> squared_distances_to_intersecting_bodies);
	TypedArray<float> get_squared_distances_to_intersecting_bodies() const;

	void set_squared_distances_to_unoccluded_areas(TypedArray<float> squared_distances_to_unoccluded_areas);
	TypedArray<float> get_squared_distances_to_unoccluded_areas() const;

	void set_squared_distances_to_unoccluded_bodies(TypedArray<float> squared_distances_to_unoccluded_bodies);
	TypedArray<float> get_squared_distances_to_unoccluded_bodies() const;
};

} //namespace godot
