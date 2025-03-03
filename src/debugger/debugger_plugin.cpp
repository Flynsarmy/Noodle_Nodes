#ifdef DEBUG_ENABLED

#include "debugger_plugin.h"
#include "../agent_behaviours/agent.h"
#include "../behaviour_tree/root.h"
#include "../noodle_nodes.h"
#include "../state_tree/root.h"
#include <godot_cpp/classes/label.hpp>

using namespace godot;

NNDebuggerPlugin::NNDebuggerPlugin() {
}

NNDebuggerPlugin::~NNDebuggerPlugin() {
}

void NNDebuggerPlugin::_bind_methods() {
}

void NNDebuggerPlugin::_setup_session(int32_t session_id) {
	Label *debugger_label = memnew(Label);
	ERR_FAIL_COND_MSG(debugger_label == nullptr, "NNDebuggerPlugin::_setup_session(): Failed to create the debugger label - Out of memory?");
	debugger_label->set_name("NN Debugger");
	debugger_label->set_name("NN Debugger");
	//Ref<EditorDebuggerSession> uai_session = get_session(session_id);
	_debugger_session = get_session(session_id);
	if (!_debugger_session.is_valid()) { //uai_session.is_null() || !uai_session.is_valid() ) {
		memdelete(debugger_label);
		ERR_FAIL_MSG("NNDebuggerPlugin::_setup_session(): Failed to create the debugger label - Out of memory?");
	}

	//uai_session->add_session_tab(debugger_label);
	_debugger_session->add_session_tab(debugger_label);
}

bool NNDebuggerPlugin::_has_capture(const String &capture) const {
	return capture.begins_with("uai_");
}

bool NNDebuggerPlugin::_capture(const String &message, const Array &data, int32_t session_id) {
	//if( data.size() == 0 ) {
	//    return false;
	//}//
	//Ref<EditorDebuggerSession> uai_session = get_session(session_id);
	if (message == "uai_select_root") {
		if (NNBTRoot *btroot = godot::Object::cast_to<NNBTRoot>(data[0])) {
			WARN_PRINT("Got BT Root!");

		} else if (NNSTRoot *stroot = godot::Object::cast_to<NNSTRoot>(data[0])) {
			WARN_PRINT("Got ST Root!");
		} else if (NNAgent *agent = godot::Object::cast_to<NNAgent>(data[0])) {
			WARN_PRINT("Got Agent!");
		}
		return true;
	} else if (message == "") {
		return true;
	} //endif message
	return false;
}

void NNDebuggerPlugin::_notification(int p_what) {
}

#endif //debug enabled
