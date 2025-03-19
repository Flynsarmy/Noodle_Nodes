#pragma once

#include "../agent_behaviours/considerations.h"
#include "definitions.h"
#include "noodle_nodes.h"
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {

class NNSTTransition; // Forward declaration

class NNSTNodes : public NoodleNodes {
	GDCLASS(NNSTNodes, NoodleNodes)

private:
	int _internal_status;
	bool _is_on_entered_condition_true;
	float _score;
	int _evaluation_method;
	bool _invert_score;
	int _child_state_selection_rule;
	NNSTNodes *_tree_root_node;

	std::vector<NNConsiderations *> _child_considerations;
	unsigned int _num_child_considerations;

protected:
	static void _bind_methods();

	std::vector<NNSTNodes *> _child_states;
	unsigned int _num_child_states;

	std::vector<NNSTTransition *> _child_transitions;
	unsigned int _num_child_transitions;

#ifdef DEBUG_ENABLED
	uint64_t _last_evaluated_timestamp;
	uint64_t _last_visited_timestamp;
#endif
public:
	NNSTNodes();
	~NNSTNodes();

	// Getters and setters for attributes.

#ifdef DEBUG_ENABLED
	inline uint64_t get_last_visited_timestamp() const { return _last_visited_timestamp; };
	inline uint64_t get_last_evaluated_timestamp() const { return _last_evaluated_timestamp; };
#endif

	void set_is_on_entered_condition_true(bool is_on_entered_condition_true);
	bool get_is_on_entered_condition_true() const;

	void set_evaluation_method(int evaluation_method);
	int get_evaluation_method() const;

	enum NNSTNodesEvaluationMethod {
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

	NNSTNodes *get_tree_root() const;

	void set_child_state_selection_rule(int child_state_selection_rule);
	int get_child_state_selection_rule() const;

	virtual void set_root_node(NNSTNodes *tree_root_node);
	//Dictionary get_child_nodes_as_dictionary(NNSTNodes* tree_root_node );

	void set_internal_status(int internal_status);
	int get_internal_status() const;

	//inline virtual bool get_is_leaf() const { return false; };

	enum NNStateTreeNodeChildStateSelectionRule {
		ON_ENTER_CONDITION_METHOD = 0,
		UTILITY_SCORING = 1,
	};

	// Handling functions.

	virtual float evaluate();
	virtual void on_input(const Ref<InputEvent> &event);
	virtual void on_unhandled_input(const Ref<InputEvent> &event);
	virtual void on_unhandled_key_input(const Ref<InputEvent> &event);
	virtual bool on_enter_condition(Variant blackboard, float delta);
	virtual void on_enter_state(Variant blackboard, float delta);
	virtual void on_exit_state(Variant blackboard, float delta);
	virtual void on_tick(Variant blackboard, float delta);
	virtual void transition_to(NodePath path_to_node, Variant blackboard, float delta);
	virtual void send_event(String name, Variant blackboard, float delta);

	TypedArray<NNSTNodes> _active_states;
	virtual void _transition_out(Variant blackboard, float delta);
	virtual void _transition_in(Variant blackboard, float delta);
	virtual TypedArray<NNSTNodes> _evaluate_child_activations(Variant blackboard, float delta);
	virtual void _handle_transition(NNSTNodes *from_state, NNSTNodes *to_state, Variant blackboard, float delta);
	virtual bool _can_transition_to(NNSTNodes *from_state, NNSTNodes *to_state);

	GDVIRTUAL1(on_input, Ref<InputEvent>);
	GDVIRTUAL1(on_unhandled_input, Ref<InputEvent>);
	GDVIRTUAL1(on_unhandled_key_input, Ref<InputEvent>);
	GDVIRTUAL2R(bool, on_enter_condition, Variant, double);
	GDVIRTUAL2(on_enter_state, Variant, double);
	GDVIRTUAL2(on_exit_state, Variant, double);
	GDVIRTUAL2(on_tick, Variant, double);
	GDVIRTUAL3(transition_to, NodePath, Variant, double);

	// virtual NNSTNodes *evaluate_state_activation(Variant blackboard, float delta);
	// virtual void evaluate_state_activations(TypedArray<NNSTNodes> *nodes, Variant blackboard, float delta);

	// Godot virtuals.

	//void _enter_tree();
	virtual void _input(const Ref<InputEvent> &p_event) override;
	virtual void _unhandled_input(const Ref<InputEvent> &p_event) override;
	virtual void _unhandled_key_input(const Ref<InputEvent> &p_event) override;
	void _notification(int p_what);
};

} //namespace godot
