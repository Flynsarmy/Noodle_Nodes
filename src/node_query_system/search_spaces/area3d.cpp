#include "area3d.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

NNArea3DSearchSpace::NNArea3DSearchSpace() {
	_area3d = nullptr;
}

NNArea3DSearchSpace::~NNArea3DSearchSpace() {
}

void NNArea3DSearchSpace::_bind_methods() {
	ADD_SUBGROUP("Configuration", "");

	ClassDB::bind_method(D_METHOD("set_area3d", "area3d"), &NNArea3DSearchSpace::set_area3d);
	ClassDB::bind_method(D_METHOD("get_area3d"), &NNArea3DSearchSpace::get_area3d);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "area3d", PROPERTY_HINT_NODE_TYPE, "Area3D"), "set_area3d", "get_area3d");

	ClassDB::bind_method(D_METHOD("on_area_entered", "area"), &NNArea3DSearchSpace::on_area_entered);
	ClassDB::bind_method(D_METHOD("on_area_exited", "area"), &NNArea3DSearchSpace::on_area_exited);

	ADD_SUBGROUP("Debugging", "");

	ClassDB::bind_method(D_METHOD("set_intersecting_areas", "intersecting_areas"), &NNArea3DSearchSpace::set_intersecting_areas);
	ClassDB::bind_method(D_METHOD("get_intersecting_areas"), &NNArea3DSearchSpace::get_intersecting_areas);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "intersecting_areas", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "Area3D")), "set_intersecting_areas", "get_intersecting_areas");
}

void NNArea3DSearchSpace::on_area_entered(Area3D *area) {
	if (area == nullptr) {
		return;
	}
	if (_intersecting_areas.has(area)) {
		return;
	}
	_intersecting_areas.push_back(area);
}

void NNArea3DSearchSpace::on_area_exited(Area3D *area) {
	if (area == nullptr) {
		return;
	}
	if (!_intersecting_areas.has(area)) {
		return;
	}
	int index = _intersecting_areas.find(area);
	if (index < 0) {
		return;
	}
	_intersecting_areas.remove_at(index);
}

// Getters and setters for attributes.

void NNArea3DSearchSpace::set_area3d(Area3D *area3d) {
	_area3d = area3d;
}

Area3D *NNArea3DSearchSpace::get_area3d() const {
	return _area3d;
}

void NNArea3DSearchSpace::set_intersecting_areas(TypedArray<Area3D> intersecting_areas) {
	_intersecting_areas = intersecting_areas;
}

TypedArray<Area3D> NNArea3DSearchSpace::get_intersecting_areas() const {
	return _intersecting_areas;
}

// Handling methods.

TypedArray<Node> NNArea3DSearchSpace::get_searchspace_nodes() const {
	return _intersecting_areas;
}

void NNArea3DSearchSpace::_initialize_search_space() {
	ERR_FAIL_COND_MSG(_area3d == nullptr || !UtilityFunctions::is_instance_id_valid(_area3d->get_instance_id()), "NNArea3DSearchSpace::_initialize_search_space() - Error, the node for the Area3D has not been set.");

	// Connect to the area entered and exited signals.
	Error error_visibility_volume_on_entered = _area3d->connect("area_entered", Callable(this, "on_area_entered"));
	Error error_visibility_volume_on_exited = _area3d->connect("area_exited", Callable(this, "on_area_exited"));
}

void NNArea3DSearchSpace::_uninitialize_search_space() {
	if (_area3d != nullptr) {
		_area3d->disconnect("area_entered", Callable(this, "on_area_entered"));
		_area3d->disconnect("area_exited", Callable(this, "on_area_exited"));
	}
	_area3d = nullptr;
}
