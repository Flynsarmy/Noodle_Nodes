#ifndef NNBehaviours_H_INCLUDED
#define NNBehaviours_H_INCLUDED

#include "noodle_nodes.h"
#include <godot_cpp/classes/node.hpp>

#include "../resources/considerations/consideration_resources.h"
#include "considerations.h"

namespace godot {

class NNBehaviours : public NoodleNodes {
	GDCLASS(NNBehaviours, NoodleNodes)

private:
protected:
	static void _bind_methods();
#ifdef DEBUG_ENABLED
	uint64_t _last_evaluated_timestamp;
	uint64_t _last_visited_timestamp;
#endif

	int _evaluation_method;

	float _score;
	TypedArray<NNConsiderationResources> _considerations;

	std::vector<NNConsiderations *> _child_considerations;
	unsigned int _num_child_considerations;

	std::vector<NNBehaviours *> _child_behaviours;
	unsigned int _num_child_behaviours;

public:
	NNBehaviours() {
		_score = 0.0f;
		_evaluation_method = 0;
#ifdef DEBUG_ENABLED
		_last_evaluated_timestamp = 0;
		_last_visited_timestamp = 0;
#endif
	};

	// Getters and setters for attributes.

	void set_evaluation_method(int evaluation_method);
	int get_evaluation_method() const;

#ifdef DEBUG_ENABLED
	inline uint64_t get_last_visited_timestamp() const { return _last_visited_timestamp; };
	inline uint64_t get_last_evaluated_timestamp() const { return _last_evaluated_timestamp; };
	inline void set_last_visited_timestamp(uint64_t new_timestamp) { _last_visited_timestamp = new_timestamp; };
	inline void set_last_evaluated_timestamp(uint64_t new_timestamp) { _last_evaluated_timestamp = new_timestamp; };
#endif

	void set_score(float score) { _score = score; };
	float get_score() const { return _score; };

	std::vector<NNBehaviours *> get_child_behaviours() const { return _child_behaviours; };
	unsigned int get_num_child_behaviours() const { return _num_child_behaviours; };

	void set_considerations(TypedArray<NNConsiderationResources> considerations) { _considerations = considerations; };
	TypedArray<NNConsiderationResources> get_considerations() const { return _considerations; };

	void _notification(int p_what);

	// Handling functions.

	virtual float evaluate() {
		_score = 0.0f;
		return _score;
	};
	virtual bool is_behaviour_group() const { return false; };
	virtual float get_activation_score() const { return 0.0f; };
};

} //namespace godot

#endif
