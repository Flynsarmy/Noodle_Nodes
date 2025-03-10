#pragma once

#include "../agent_behaviours/considerations.h"
#include "../noodle_nodes.h"
#include "../resources/considerations/consideration_resources.h"
#include "definitions.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTNodes : public NoodleNodes {
	GDCLASS(NNBTNodes, NoodleNodes)

public:
	NNBTNodes();
	~NNBTNodes();

	// Getters and setters for attributes.

#ifdef DEBUG_ENABLED
	inline uint64_t get_last_visited_timestamp() const { return _last_visited_timestamp; };
	inline uint64_t get_last_evaluated_timestamp() const { return _last_evaluated_timestamp; };
#endif

	void set_considerations(TypedArray<NNConsiderationResources> considerations);
	TypedArray<NNConsiderationResources> get_considerations() const;

	void set_evaluation_method(int evaluation_method);
	int get_evaluation_method() const;

	enum Status {
		SKIP = -2,
		FAILURE = -1,
		RUNNING = 0,
		SUCCESS = 1,
	};

	enum NNBTNodesEvaluationMethod {
		Sum = 0,
		Min = 1,
		Max = 2,
		Mean = 3,
		Multiply = 4,
		FirstNonZero = 5,
		//OneMinusScore=6
	};

	void set_score(float score);
	float get_score() const;

	void set_tick_result(Status tick_result);
	Status get_tick_result() const;

	void set_internal_status(int internal_status);
	int get_internal_status() const;

	void set_reset_rule(int reset_rule);
	int get_reset_rule() const;

	enum NNBTNodesResetRule {
		WHEN_TICKED = 0,
		WHEN_COMPLETED,
		WHEN_TICKED_AFTER_BEING_COMPLETED,
		NEVER,
	};

	// Handling functions.

	virtual float evaluate();

	virtual Status tick(Variant blackboard, float delta);

	virtual void reset();
	//virtual void reset_for_looping();
	virtual void reset_bt_node() {};
	virtual void update_child_vectors();

	inline virtual bool has_completed() { return (_internal_status == BT_INTERNAL_STATUS_COMPLETED); };

	mutable NNBTNodes *_tree_root_node;
	NNBTNodes *get_tree_root() const;

	// Godot virtuals.
	virtual void _notification(int p_what);

private:
	float _score;
	int _evaluation_method;
	bool _invert_score;
	Status _tick_result;
	int _internal_status;
	int _reset_rule;
	bool _has_reset_rule_changed;

	TypedArray<NNConsiderationResources> _considerations;

protected:
	static void _bind_methods();

	//bool   _is_first_tick;
	std::vector<NNBTNodes *> _child_btnodes;
	unsigned int _num_child_btnodes;
	std::vector<NNConsiderations *> _child_considerations;
	unsigned int _num_child_considerations;
#ifdef DEBUG_ENABLED
	uint64_t _last_evaluated_timestamp;
	uint64_t _last_visited_timestamp;
#endif
};

} //namespace godot

VARIANT_ENUM_CAST(NNBTNodes::Status)
