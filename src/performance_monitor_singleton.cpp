#include "performance_monitor_singleton.h"
#include <godot_cpp/classes/performance.hpp>

using namespace godot;

NNPerformanceMonitorSingleton *NNPerformanceMonitorSingleton::_singleton = nullptr;

NNPerformanceMonitorSingleton::NNPerformanceMonitorSingleton() {
	set_is_active(false);
	_total_time_elapsed_ai_agents_usec = 0;
	_total_time_elapsed_behaviour_trees_usec = 0;
	_total_time_elapsed_state_trees_usec = 0;
	_current_total_time_elapsed_ai_agents_usec = 0;
	_current_total_time_elapsed_behaviour_trees_usec = 0;
	_current_total_time_elapsed_state_trees_usec = 0;
	_singleton = this;
}

NNPerformanceMonitorSingleton::~NNPerformanceMonitorSingleton() {
}

void NNPerformanceMonitorSingleton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("increment_total_time_elapsed_ai_agents_usec", "time_usec"), &NNPerformanceMonitorSingleton::increment_total_time_elapsed_ai_agents_usec);
	ClassDB::bind_method(D_METHOD("increment_total_time_elapsed_ai_agents_update_behaviour_usec", "time_usec"), &NNPerformanceMonitorSingleton::increment_total_time_elapsed_ai_agents_update_behaviour_usec);
	ClassDB::bind_method(D_METHOD("increment_total_time_elapsed_behaviour_trees_usec", "time_usec"), &NNPerformanceMonitorSingleton::increment_total_time_elapsed_behaviour_trees_usec);
	ClassDB::bind_method(D_METHOD("increment_total_time_elapsed_state_trees_usec", "time_usec"), &NNPerformanceMonitorSingleton::increment_total_time_elapsed_state_trees_usec);

	ClassDB::bind_method(D_METHOD("initialize_performance_counters"), &NNPerformanceMonitorSingleton::initialize_performance_counters);
	ClassDB::bind_method(D_METHOD("update_performance_counters"), &NNPerformanceMonitorSingleton::update_performance_counters);

	ClassDB::bind_method(D_METHOD("get_total_time_elapsed_ai_agents_usec"), &NNPerformanceMonitorSingleton::get_total_time_elapsed_ai_agents_usec);
	ClassDB::bind_method(D_METHOD("get_total_time_elapsed_ai_agents_update_behaviour_usec"), &NNPerformanceMonitorSingleton::get_total_time_elapsed_ai_agents_update_behaviour_usec);
	ClassDB::bind_method(D_METHOD("get_total_time_elapsed_behaviour_trees_usec"), &NNPerformanceMonitorSingleton::get_total_time_elapsed_behaviour_trees_usec);
	ClassDB::bind_method(D_METHOD("get_total_time_elapsed_state_trees_usec"), &NNPerformanceMonitorSingleton::get_total_time_elapsed_state_trees_usec);
}

// Getters and setters.

int NNPerformanceMonitorSingleton::get_current_total_time_elapsed_ai_agents_usec() const {
	return _current_total_time_elapsed_ai_agents_usec;
}

int NNPerformanceMonitorSingleton::get_current_total_time_elapsed_ai_agents_update_behaviour_usec() const {
	return _current_total_time_elapsed_ai_agents_update_behaviour_usec;
}

int NNPerformanceMonitorSingleton::get_current_total_time_elapsed_behaviour_trees_usec() const {
	return _current_total_time_elapsed_behaviour_trees_usec;
}

int NNPerformanceMonitorSingleton::get_current_total_time_elapsed_state_trees_usec() const {
	return _current_total_time_elapsed_state_trees_usec;
}

int NNPerformanceMonitorSingleton::get_total_time_elapsed_ai_agents_usec() const {
	return _total_time_elapsed_ai_agents_usec;
}

int NNPerformanceMonitorSingleton::get_total_time_elapsed_ai_agents_update_behaviour_usec() const {
	return _total_time_elapsed_ai_agents_update_behaviour_usec;
}

int NNPerformanceMonitorSingleton::get_total_time_elapsed_behaviour_trees_usec() const {
	return _total_time_elapsed_behaviour_trees_usec;
}

int NNPerformanceMonitorSingleton::get_total_time_elapsed_state_trees_usec() const {
	return _total_time_elapsed_state_trees_usec;
}

// Handling methods.

void NNPerformanceMonitorSingleton::increment_total_time_elapsed_ai_agents_usec(uint64_t time_usec) {
	_current_total_time_elapsed_ai_agents_usec += time_usec;
}

void NNPerformanceMonitorSingleton::increment_total_time_elapsed_ai_agents_update_behaviour_usec(uint64_t time_usec) {
	_current_total_time_elapsed_ai_agents_update_behaviour_usec += time_usec;
}

void NNPerformanceMonitorSingleton::increment_total_time_elapsed_behaviour_trees_usec(uint64_t time_usec) {
	_current_total_time_elapsed_behaviour_trees_usec += time_usec;
}

void NNPerformanceMonitorSingleton::increment_total_time_elapsed_state_trees_usec(uint64_t time_usec) {
	_current_total_time_elapsed_state_trees_usec += time_usec;
}

void NNPerformanceMonitorSingleton::initialize_performance_counters() {
#ifdef DEBUG_ENABLED
	if (_is_performance_counter_initialized)
		return;
	Performance *perf = godot::Performance::get_singleton();
	if (perf == nullptr)
		return;
	perf->add_custom_monitor("NN/AI Agent evaluate options time usec", Callable(this, "get_total_time_elapsed_ai_agents_usec"));
	perf->add_custom_monitor("NN/AI Agent update behaviour time usec", Callable(this, "get_total_time_elapsed_ai_agents_update_behaviour_usec"));
	perf->add_custom_monitor("NN/Behaviour Tree tick time usec", Callable(this, "get_total_time_elapsed_behaviour_trees_usec"));
	perf->add_custom_monitor("NN/State Tree tick time usec", Callable(this, "get_total_time_elapsed_state_trees_usec"));
	_is_performance_counter_initialized = true;
	set_is_active(true);
#endif
}

void NNPerformanceMonitorSingleton::update_performance_counters() {
	_total_time_elapsed_ai_agents_usec = _current_total_time_elapsed_ai_agents_usec;
	_total_time_elapsed_ai_agents_update_behaviour_usec = _current_total_time_elapsed_ai_agents_update_behaviour_usec;
	_total_time_elapsed_behaviour_trees_usec = _current_total_time_elapsed_behaviour_trees_usec;
	_total_time_elapsed_state_trees_usec = _current_total_time_elapsed_state_trees_usec;
	_current_total_time_elapsed_ai_agents_usec = 0;
	_current_total_time_elapsed_ai_agents_update_behaviour_usec = 0;
	_current_total_time_elapsed_behaviour_trees_usec = 0;
	_current_total_time_elapsed_state_trees_usec = 0;
}