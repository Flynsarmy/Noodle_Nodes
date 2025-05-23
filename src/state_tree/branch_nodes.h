#pragma once

#include "../agent_behaviours/considerations.h"
#include "definitions.h"
#include "nodes.h"
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {

class NNSTTransition; // Forward declaration
class NNSTTickedNodes; // Forward declaration

class NNSTBranchNodes : public NNSTNodes {
	GDCLASS(NNSTBranchNodes, NNSTNodes)

private:
protected:
	static void _bind_methods();

	int _internal_status;
	int _child_state_selection_rule;

	std::vector<NNConsiderations *> _child_considerations;
	unsigned int _num_child_considerations;

	std::vector<NNSTTickedNodes *> _child_states;
	unsigned int _num_child_states;

	std::vector<NNSTTransition *> _child_transitions;
	unsigned int _num_child_transitions;

	std::vector<NNSTTickedNodes *> _active_states;
	unsigned int _num_active_states;

#ifdef DEBUG_ENABLED
	uint64_t _last_evaluated_timestamp;
	uint64_t _last_visited_timestamp;
#endif

public:
	NNSTBranchNodes();
	~NNSTBranchNodes();

	// Getters and setters for attributes.

	bool get_is_active() const;

#ifdef DEBUG_ENABLED
	inline uint64_t get_last_visited_timestamp() const { return _last_visited_timestamp; };
	inline uint64_t get_last_evaluated_timestamp() const { return _last_evaluated_timestamp; };
#endif

	void set_child_state_selection_rule(int child_state_selection_rule);
	int get_child_state_selection_rule() const;

	//Dictionary get_child_nodes_as_dictionary(NNSTBranchNodes* tree_root_node );

	void set_internal_status(int internal_status);
	int get_internal_status() const;

	//inline virtual bool get_is_leaf() const { return false; };

	enum NNSTNodeChildStateSelectionRule {
		ON_ENTER_CONDITION_METHOD = 0,
		UTILITY_SCORING = 1,
	};

	// Handling functions.

	virtual void on_input(const Ref<InputEvent> &event);
	virtual void on_unhandled_input(const Ref<InputEvent> &event);
	virtual void on_unhandled_key_input(const Ref<InputEvent> &event);
	virtual void send_event(String name) {};

	virtual void _transition_in() {};
	virtual std::vector<NNSTTickedNodes *> _get_active_child_states() const { return _active_states; };
	virtual void _add_active_child_state(NNSTTickedNodes *child_state);
	virtual void _remove_active_child_state(NNSTTickedNodes *child_state);
	virtual void _handle_transition(NNSTTickedNodes *from_state, NNSTTickedNodes *to_state);
	virtual bool _can_transition_to(NNSTTickedNodes *from_state, NNSTTickedNodes *to_state);
	virtual NNSTBranchNodes *_get_common_ancestor(NNSTTickedNodes *from_state, NNSTTickedNodes *to_state);
	virtual void _evaluate_child_activations(std::vector<NNSTTickedNodes *> &nodes);

	GDVIRTUAL1(on_input, Ref<InputEvent>);
	GDVIRTUAL1(on_unhandled_input, Ref<InputEvent>);
	GDVIRTUAL1(on_unhandled_key_input, Ref<InputEvent>);

	// virtual NNSTBranchNodes *evaluate_state_activation(Variant blackboard, float delta);
	// virtual void evaluate_state_activations(TypedArray<NNSTBranchNodes> *nodes, Variant blackboard, float delta);

	// Godot virtuals.

	//void _enter_tree();
	virtual void _input(const Ref<InputEvent> &p_event) override;
	virtual void _unhandled_input(const Ref<InputEvent> &p_event) override;
	virtual void _unhandled_key_input(const Ref<InputEvent> &p_event) override;
	void _notification(int p_what);
};

} //namespace godot
