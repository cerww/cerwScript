#include "function.h"

variable function::run(const std::vector<variable>& args) {
	stackFrame functionFrame(m_parent, orderOfInstructions);
	functionFrame.createVar(m_retType, "RET_VAL_EEEE");
	for (int i = 0; i < args.size(); ++i) {
		functionFrame.createVar(m_sig[i].type,m_sig[i].name).setVar(m_sig[i].name, args[i]);
	}functionFrame.run();
	return std::move(functionFrame.getVal("RET_VAL_EEEE"));
}
