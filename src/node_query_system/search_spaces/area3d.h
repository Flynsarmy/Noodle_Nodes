#pragma once

#include "nqs.h"
#include <godot_cpp/classes/area3d.hpp>

namespace godot {

class NNArea3DSearchSpace : public NNNQSSearchSpaces {
	GDCLASS(NNArea3DSearchSpace, NNNQSSearchSpaces)

private:
	Area3D *_area3d;

	TypedArray<Area3D> _intersecting_areas;

protected:
	static void _bind_methods();

	virtual void _initialize_search_space() override;
	virtual void _uninitialize_search_space() override;

public:
	NNArea3DSearchSpace();
	~NNArea3DSearchSpace();

	void on_area_entered(Area3D *area);
	void on_area_exited(Area3D *area);

	// Getters and setters for attributes.

	void set_area3d(Area3D *area3d);
	Area3D *get_area3d() const;

	void set_intersecting_areas(TypedArray<Area3D> intersecting_areas);
	TypedArray<Area3D> get_intersecting_areas() const;

	virtual TypedArray<Node> get_searchspace_nodes() const override;
};

} //namespace godot
