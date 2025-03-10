#pragma once

#include "decorator_nodes.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNBTCooldownMsec : public NNBTDecoratorNodes {
	GDCLASS(NNBTCooldownMsec, NNBTDecoratorNodes)

private:
	uint64_t _cooldown_msec;
	uint64_t _cooldown_start_timestamp;

	Status _cooldown_return_value;
	bool _is_in_cooldown;

protected:
	static void _bind_methods();

public:
	NNBTCooldownMsec();
	~NNBTCooldownMsec();

	// Getters and setters for attributes.

	void set_cooldown_msec(int cooldown_msec);
	int get_cooldown_msec() const;

	void set_cooldown_start_timestamp(int cooldown_msec);
	int get_cooldown_start_timestamp() const;

	void set_cooldown_return_value(Status cooldown_return_value);
	Status get_cooldown_return_value() const;

	// Handling functions.

	virtual Status tick(Variant blackboard, float delta) override;
};

} //namespace godot
