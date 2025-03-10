#pragma once

//#include <godot_cpp/classes/node.hpp>
#include "noodle_nodes.h"

namespace godot {

class NNPerformanceMonitorSingleton : public NoodleNodes {
	GDCLASS(NNPerformanceMonitorSingleton, NoodleNodes)

private:
	bool _is_performance_counter_initialized;

	uint64_t _total_time_elapsed_ai_agents_usec;
	uint64_t _total_time_elapsed_ai_agents_update_behaviour_usec;
	uint64_t _total_time_elapsed_behaviour_trees_usec;
	uint64_t _total_time_elapsed_state_trees_usec;

	uint64_t _current_total_time_elapsed_ai_agents_usec;
	uint64_t _current_total_time_elapsed_ai_agents_update_behaviour_usec;
	uint64_t _current_total_time_elapsed_behaviour_trees_usec;
	uint64_t _current_total_time_elapsed_state_trees_usec;

protected:
	static void _bind_methods();
	static NNPerformanceMonitorSingleton *_singleton;

public:
	NNPerformanceMonitorSingleton();
	~NNPerformanceMonitorSingleton();

	static NNPerformanceMonitorSingleton *get_singleton() { return _singleton; };

	// Getters and setters for attributes.

	int get_total_time_elapsed_ai_agents_usec() const;
	int get_total_time_elapsed_ai_agents_update_behaviour_usec() const;
	int get_total_time_elapsed_behaviour_trees_usec() const;
	int get_total_time_elapsed_state_trees_usec() const;

	int get_current_total_time_elapsed_ai_agents_usec() const;
	int get_current_total_time_elapsed_ai_agents_update_behaviour_usec() const;

	int get_current_total_time_elapsed_behaviour_trees_usec() const;
	int get_current_total_time_elapsed_state_trees_usec() const;

	// Handling methods.

	void increment_total_time_elapsed_ai_agents_usec(uint64_t time_usec);
	void increment_total_time_elapsed_ai_agents_update_behaviour_usec(uint64_t time_usec);
	void increment_total_time_elapsed_behaviour_trees_usec(uint64_t time_usec);
	void increment_total_time_elapsed_state_trees_usec(uint64_t time_usec);

	void initialize_performance_counters();
	void update_performance_counters();
};

} //namespace godot
