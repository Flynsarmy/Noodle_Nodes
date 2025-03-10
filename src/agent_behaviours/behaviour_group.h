#pragma once

#include "behaviours.h"
#include "considerations.h"
#include "noodle_nodes.h"
#include "resources/considerations/consideration_resources.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBehaviourGroup : public NNBehaviours {
	GDCLASS(NNBehaviourGroup, NNBehaviours)

private:
	//float  _score;
	float _activation_score;
	//TypedArray<NNConsiderationResources> _considerations;

	//std::vector<NNConsiderations*> _child_considerations;
	//unsigned int _num_child_considerations;
protected:
	static void _bind_methods();
#ifdef DEBUG_ENABLED
	uint64_t _last_evaluated_timestamp;
	uint64_t _last_visited_timestamp;
#endif
public:
	NNBehaviourGroup();
	~NNBehaviourGroup();

	// Getters and setters for attributes.

#ifdef DEBUG_ENABLED
	inline uint64_t get_last_visited_timestamp() const { return _last_visited_timestamp; };
	inline uint64_t get_last_evaluated_timestamp() const { return _last_evaluated_timestamp; };
#endif

	void set_activation_score(float score);
	float get_activation_score() const override;

	//void  set_score( float score );
	//float get_score() const;

	void set_considerations(TypedArray<NNConsiderationResources> considerations);
	TypedArray<NNConsiderationResources> get_considerations() const;

	// Handling functions.

	float evaluate() override;
	bool is_behaviour_group() const override { return true; };

	// Godot virtuals.
	//void _notification(int p_what);
};

} //namespace godot
