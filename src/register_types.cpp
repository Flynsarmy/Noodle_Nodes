#include "register_types.h"
#include <godot_cpp/classes/engine.hpp>

// Classes to include in to the gdextension module.

// Performance monitor singleton.
#include "performance_monitor_singleton.h"

// AI AGENT

// Core nodes
#include "agent_behaviours/action.h"
#include "agent_behaviours/action_group.h"
#include "agent_behaviours/actions.h"
#include "agent_behaviours/agent.h"
#include "agent_behaviours/behaviour.h"
#include "agent_behaviours/behaviour_group.h"
#include "agent_behaviours/behaviours.h"
#include "agent_behaviours/consideration.h"
#include "agent_behaviours/consideration_group.h"
#include "agent_behaviours/considerations.h"
#include "agent_behaviours/sensor.h"
#include "agent_behaviours/sensor_group.h"
#include "agent_behaviours/sensors.h"

// Special sensors
#include "agent_behaviours/sensors/angle_vector2.h"
#include "agent_behaviours/sensors/angle_vector3.h"
#include "agent_behaviours/sensors/area2d_visibility.h"
#include "agent_behaviours/sensors/area3d_visibility.h"
#include "agent_behaviours/sensors/boolean.h"
#include "agent_behaviours/sensors/distance_vector2.h"
#include "agent_behaviours/sensors/distance_vector3.h"
#include "agent_behaviours/sensors/float_range.h"
#include "agent_behaviours/sensors/int_range.h"

// Special considerations
#include "agent_behaviours/considerations/boolean.h"
#include "agent_behaviours/considerations/custom_property.h"

// BEHAVIOUR TREE
#include "behaviour_tree/nodes.h"
#include "behaviour_tree/root.h"

// Composites
#include "behaviour_tree/composite_nodes.h"
#include "behaviour_tree/parallel.h"
#include "behaviour_tree/random_selector.h"
#include "behaviour_tree/random_sequence.h"
#include "behaviour_tree/score_based_picker.h"
#include "behaviour_tree/selector.h"
#include "behaviour_tree/sequence.h"

// Decorators
#include "behaviour_tree/cooldown_msec.h"
#include "behaviour_tree/cooldown_ticks.h"
#include "behaviour_tree/decorator_nodes.h"
#include "behaviour_tree/fixed_result.h"
#include "behaviour_tree/inverter.h"
#include "behaviour_tree/limiter.h"
#include "behaviour_tree/pass_through.h"
#include "behaviour_tree/repeat_until.h"
#include "behaviour_tree/repeater.h"

// Tasks
#include "behaviour_tree/leaf.h"
#include "behaviour_tree/node_reference.h"
#include "behaviour_tree/pass_by.h"
#include "behaviour_tree/run_nqs_query.h"

// STATE TREE

//#include "state_tree/leaf.h"
#include "state_tree/all_of_guard.h"
#include "state_tree/any_of_guard.h"
#include "state_tree/branch_nodes.h"
#include "state_tree/expression_guard.h"
#include "state_tree/guard.h"
#include "state_tree/node.h"
#include "state_tree/nodes.h"
#include "state_tree/not_guard.h"
#include "state_tree/parallel.h"
#include "state_tree/root.h"
#include "state_tree/state_is_active_guard.h"
#include "state_tree/ticked_nodes.h"
#include "state_tree/transition.h"

// NODE QUERY SYSTEM
#include "node_query_system/node_query_system.h"

// Search spaces
#include "node_query_system/search_spaces/node_children.h"
#include "node_query_system/search_spaces/node_group.h"
#include "node_query_system/search_spaces/nqs.h"

#include "node_query_system/search_spaces/area2d.h"
#include "node_query_system/search_spaces/area3d.h"

#include "node_query_system/search_spaces/point_grid/point_grid_2d.h"
#include "node_query_system/search_spaces/point_grid/point_grid_3d.h"

// Search criteria
#include "node_query_system/search_criteria/custom.h"
#include "node_query_system/search_criteria/metadata.h"
#include "node_query_system/search_criteria/nqs.h"

#include "node_query_system/search_criteria/distance_to_node2d.h"
#include "node_query_system/search_criteria/distance_to_node3d.h"

#include "node_query_system/search_criteria/distance_to_vector2.h"
#include "node_query_system/search_criteria/distance_to_vector3.h"

#include "node_query_system/search_criteria/angle_to_vector2.h"
#include "node_query_system/search_criteria/angle_to_vector3.h"
#include "node_query_system/search_criteria/angle_to_vector3_xz.h"

#include "node_query_system/search_criteria/dot_product_vector2.h"
#include "node_query_system/search_criteria/dot_product_vector3.h"

#include "node_query_system/search_criteria/dot_product_to_position_vector2.h"
#include "node_query_system/search_criteria/dot_product_to_position_vector3.h"

// Main header for NN.
#include "noodle_nodes.h"

// EDITOR PLUGIN CLASSES.
#ifdef DEBUG_ENABLED

//#include "editor/editor_plugin.h"
//#include "debugger/debugger_plugin.h"

#include "debugger/debugger_overlay.h"

#endif

// Standard headers.
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

// Singletons
static NNNodeQuerySystem *gpNodeQuerySystem;
static NNPerformanceMonitorSingleton *gpAIPerformanceMonitor;
#ifdef DEBUG_ENABLED
static NNDebuggerOverlay *gpAIDebuggerOverlay;

#endif

void register_scene_classes() {
	GDREGISTER_VIRTUAL_CLASS(NoodleNodes);
	GDREGISTER_VIRTUAL_CLASS(NNConsiderations);
	GDREGISTER_CLASS(NNConsiderationGroup);
	GDREGISTER_CLASS(NNConsideration);
	GDREGISTER_VIRTUAL_CLASS(NNSensors);
	GDREGISTER_CLASS(NNSensorGroup);
	GDREGISTER_CLASS(NNSensor);
	GDREGISTER_VIRTUAL_CLASS(NNActions);
	GDREGISTER_CLASS(NNActionGroup);
	GDREGISTER_CLASS(NNAction);
	GDREGISTER_VIRTUAL_CLASS(NNBehaviours);
	GDREGISTER_CLASS(NNBehaviour);
	GDREGISTER_CLASS(NNBehaviourGroup);
	GDREGISTER_CLASS(NNAgent);

	// // Special sensors.
	GDREGISTER_CLASS(NNBooleanSensor);
	GDREGISTER_CLASS(NNFloatRangeSensor);
	GDREGISTER_CLASS(NNIntRangeSensor);
	GDREGISTER_CLASS(NNAngleVector2Sensor);
	GDREGISTER_CLASS(NNAngleVector3Sensor);
	GDREGISTER_CLASS(NNDistanceVector2Sensor);
	GDREGISTER_CLASS(NNDistanceVector3Sensor);
	GDREGISTER_CLASS(NNArea2DVisibilitySensor);
	GDREGISTER_CLASS(NNArea3DVisibilitySensor);

	// Special considerations.
	GDREGISTER_CLASS(NNBooleanConsideration);
	GDREGISTER_CLASS(NNCustomPropertyConsideration);

	// Behaviour tree.
	GDREGISTER_VIRTUAL_CLASS(NNBTNodes);
	GDREGISTER_CLASS(NNBTRoot);

	GDREGISTER_VIRTUAL_CLASS(NNBTCompositeNodes);
	GDREGISTER_CLASS(NNBTSequence);
	GDREGISTER_CLASS(NNBTSelector);
	GDREGISTER_CLASS(NNBTParallel);
	GDREGISTER_CLASS(NNBTRandomSequence);
	GDREGISTER_CLASS(NNBTRandomSelector);
	GDREGISTER_CLASS(NNBTScoreBasedPicker);

	GDREGISTER_VIRTUAL_CLASS(NNBTTaskNodes);
	GDREGISTER_CLASS(NNBTLeaf);
	GDREGISTER_CLASS(NNBTRunNQSQuery);
	GDREGISTER_CLASS(NNBTPassBy);
	GDREGISTER_CLASS(NNBTNodeReference);

	GDREGISTER_VIRTUAL_CLASS(NNBTDecoratorNodes);
	GDREGISTER_CLASS(NNBTRepeater);
	GDREGISTER_CLASS(NNBTRepeatUntil);
	GDREGISTER_CLASS(NNBTLimiter);
	GDREGISTER_CLASS(NNBTFixedResult);
	GDREGISTER_CLASS(NNBTInverter);
	GDREGISTER_CLASS(NNBTPassThrough);
	GDREGISTER_CLASS(NNBTCooldownTicks);
	GDREGISTER_CLASS(NNBTCooldownMsec);

	// State tree.
	GDREGISTER_CLASS(NNSTGuard);
	GDREGISTER_CLASS(NNSTExpressionGuard);
	GDREGISTER_CLASS(NNSTAllOfGuard);
	GDREGISTER_CLASS(NNSTAnyOfGuard);
	GDREGISTER_CLASS(NNSTNotGuard);
	GDREGISTER_CLASS(NNSTStateIsActiveGuard);
	GDREGISTER_VIRTUAL_CLASS(NNSTNodes);
	GDREGISTER_VIRTUAL_CLASS(NNSTBranchNodes);
	GDREGISTER_VIRTUAL_CLASS(NNSTTickedNodes);
	GDREGISTER_CLASS(NNSTRoot);
	GDREGISTER_CLASS(NNSTNode);
	GDREGISTER_CLASS(NNSTParallel);
	GDREGISTER_CLASS(NNSTTransition);

	// Node Query System.
	GDREGISTER_CLASS(NNNodeQuerySystem);

	GDREGISTER_VIRTUAL_CLASS(NNNQSSearchSpaces);
	GDREGISTER_CLASS(NNNodeGroupSearchSpace);
	GDREGISTER_CLASS(NNNodeChildrenSearchSpace);

	GDREGISTER_CLASS(NNArea3DSearchSpace);
	GDREGISTER_CLASS(NNArea2DSearchSpace);

	GDREGISTER_CLASS(NNPointGrid3DSearchSpace);
	GDREGISTER_CLASS(NNPointGrid2DSearchSpace);

	GDREGISTER_VIRTUAL_CLASS(NNNQSSearchCriteria);
	GDREGISTER_CLASS(NNCustomSearchCriterion);
	GDREGISTER_CLASS(NNMetadataSearchCriterion);
	GDREGISTER_CLASS(NNDistanceToVector3SearchCriterion);
	GDREGISTER_CLASS(NNDistanceToVector2SearchCriterion);

	GDREGISTER_CLASS(NNDistanceToNode3DSearchCriterion);
	GDREGISTER_CLASS(NNDistanceToNode2DSearchCriterion);

	GDREGISTER_CLASS(NNAngleToVector3XZSearchCriterion);
	GDREGISTER_CLASS(NNAngleToVector3SearchCriterion);
	GDREGISTER_CLASS(NNAngleToVector2SearchCriterion);

	GDREGISTER_CLASS(NNDotProductVector3SearchCriterion);
	GDREGISTER_CLASS(NNDotProductVector2SearchCriterion);

	GDREGISTER_CLASS(NNDotProductToPositionVector3SearchCriterion);
	GDREGISTER_CLASS(NNDotProductToPositionVector2SearchCriterion);

	// Add singletons.
	gpNodeQuerySystem = memnew(NNNodeQuerySystem);
	Engine::get_singleton()->register_singleton("NodeQuerySystem", gpNodeQuerySystem);

	GDREGISTER_CLASS(NNPerformanceMonitorSingleton);
	gpAIPerformanceMonitor = memnew(NNPerformanceMonitorSingleton);
	Engine::get_singleton()->register_singleton("AIPerformanceMonitor", gpAIPerformanceMonitor);

#ifdef DEBUG_ENABLED

	GDREGISTER_CLASS(NNDebuggerOverlay);
	gpAIDebuggerOverlay = memnew(NNDebuggerOverlay);
	Engine::get_singleton()->register_singleton("AIDebuggerOverlay", gpAIDebuggerOverlay);
#endif
}

void register_editor_classes() {
#ifdef DEBUG_ENABLED
	//GDREGISTER_CLASS(NNEditorPlugin);
	//ClassDB::register_class<NNEditorPlugin>();
	//EditorPlugins::add_by_type<NNEditorPlugin>();

	//ClassDB::register_class<NNDebuggerPlugin>();
	//EditorPlugins::add_by_type<NNDebuggerPlugin>();
#endif
}

void initialize_noodle_nodes_module(ModuleInitializationLevel p_level) {
	//if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
	//    return;
	//}

	// Add the classes here.
	switch (p_level) {
		case MODULE_INITIALIZATION_LEVEL_SCENE: {
			register_scene_classes();
		} break;
		case MODULE_INITIALIZATION_LEVEL_EDITOR: {
			//register_editor_classes();
		} break;

		default: {
		} break;
	} //endswitch p_level
}

void uninitialize_noodle_nodes_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	Engine::get_singleton()->unregister_singleton("NodeQuerySystem");
	memdelete(gpNodeQuerySystem);

	Engine::get_singleton()->unregister_singleton("AIPerformanceMonitor");
	memdelete(gpAIPerformanceMonitor);

#ifdef DEBUG_ENABLED
	Engine::get_singleton()->unregister_singleton("AIDebuggerOverlay");
	memdelete(gpAIDebuggerOverlay);
#endif
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT noodle_nodes_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_noodle_nodes_module);
	init_obj.register_terminator(uninitialize_noodle_nodes_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
