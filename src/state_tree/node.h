#ifndef NNSTSelector_H_INCLUDED
#define NNSTSelector_H_INCLUDED

#include "nodes.h"
//#include <godot_cpp/classes/node.hpp>

namespace godot {

class NNSTNode : public NNSTNodes {
	GDCLASS(NNSTNode, NNSTNodes)

private:
protected:
	static void _bind_methods();

public:
	NNSTNode();
	~NNSTNode();
};

} //namespace godot

#endif
