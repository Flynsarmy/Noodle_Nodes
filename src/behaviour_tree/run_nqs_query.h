#pragma once

#include "../node_query_system/node_query_system.h"
#include "../node_query_system/search_spaces/nqs.h"
#include "task_nodes.h"
#include <godot_cpp/classes/node.hpp>

#define QS_IDLE 0
#define QS_RUNNING 1
#define QS_COMPLETED 2

namespace godot {

class NNBTRunNQSQuery : public NNBTTaskNodes {
	GDCLASS(NNBTRunNQSQuery, NNBTTaskNodes)

private:
	//ObjectID                  _nqs_search_space_cache;
	NNNQSSearchSpaces *_nqs_search_space;
	NNNodeQuerySystem *_nqs;

	int _time_budget_usec;
	int _top_n_to_find;
	bool _is_high_priority;

	int _query_state;

protected:
	static void _bind_methods();

public:
	NNBTRunNQSQuery();
	~NNBTRunNQSQuery();

	// Getters and setters for attributes.

	void set_nqs_search_space(NNNQSSearchSpaces *nqs_search_space);
	NNNQSSearchSpaces *get_nqs_search_space() const;

	void set_time_budget_usec(int time_budget_usec);
	int get_time_budget_usec() const;

	void set_top_n_to_find(int top_n_to_find);
	int get_top_n_to_find() const;

	void set_is_high_priority(bool is_high_priority);
	bool get_is_high_priority() const;

	// Handling functions.
	virtual void reset_bt_node() override;
	virtual Status tick(Variant blackboard, float delta) override;

	// Godot virtuals.

	void _ready() override;
	void _physics_process(double delta) override;
};

} //namespace godot
