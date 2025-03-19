#pragma once

#include "task_nodes.h"
#include "transition.h"

namespace godot {

class NNSTRoot; // Forward declaration

class NNSTNode : public NNSTTaskNodes {
	GDCLASS(NNSTNode, NNSTTaskNodes)

private:
	NNSTRoot *_root;
	bool _is_on_entered_condition_true;
	int _evaluation_method;
	float _score;
	bool _invert_score;

protected:
	static void _bind_methods();

public:
	NNSTNode();
	~NNSTNode();

	enum NNSTNodeEvaluationMethod {
		Sum = 0,
		Min = 1,
		Max = 2,
		Mean = 3,
		Multiply = 4,
		FirstNonZero = 5,
		//OneMinusScore=6
	};

	// Getters and setters for attributes.

	void set_root(NNSTRoot *p_root); // Setter
	NNSTRoot *get_root() const;

	void set_score(float score);
	float get_score() const;

	void set_evaluation_method(int evaluation_method);
	int get_evaluation_method() const;

	void set_is_on_entered_condition_true(bool is_on_entered_condition_true);
	bool get_is_on_entered_condition_true() const;

	// Handling functions.

	virtual float evaluate();
	virtual void send_event(String name, Variant blackboard, float delta) override;
	virtual void transition_to(NodePath path_to_node, Variant blackboard, float delta);

	virtual void _transition_in(Variant blackboard, float delta) override;
	virtual void _transition_out(Variant blackboard, float delta);

	virtual bool on_enter_condition(Variant blackboard, float delta);
	virtual void on_enter_state(Variant blackboard, float delta);
	virtual void on_exit_state(Variant blackboard, float delta);
	virtual void on_tick(Variant blackboard, float delta);

	GDVIRTUAL2R(bool, on_enter_condition, Variant, double);
	GDVIRTUAL2(on_enter_state, Variant, double);
	GDVIRTUAL2(on_exit_state, Variant, double);
	GDVIRTUAL2(on_tick, Variant, double);
	GDVIRTUAL3(transition_to, NodePath, Variant, double);

	// Godot virtuals.

	// void _notification(int p_what);
};

} //namespace godot
