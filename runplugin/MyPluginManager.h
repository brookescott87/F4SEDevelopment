#pragma once
#include <f4se/PluginManager.h>

class MyPluginManager : public PluginManager {
	INT_PTR m_qwFalloutBase = 0, m_qwProgramBase = 0, m_qwModuleFixup = 0;

public:
	bool Init(INT_PTR qwFalloutBase, INT_PTR qwProgramBase) {
		m_qwFalloutBase = qwFalloutBase;
		m_qwProgramBase = qwProgramBase;
		m_qwModuleFixup = qwFalloutBase - qwProgramBase;
		return PluginManager::Init();
	}
protected:
	virtual const char * SafeCallQueryPlugin(LoadedPlugin * plugin, const F4SEInterface * f4se) {
		INT_PTR *qwModulePtr = (INT_PTR *)((INT_PTR)(plugin->handle) + 0x75d00);
		if (*qwModulePtr == m_qwProgramBase) {
			_MESSAGE("Applying fixup to program references");
			for (int i = 0; i < 6; ++i) {
				INT_PTR qwOldRef = qwModulePtr[i];
				INT_PTR qwNewRef = qwOldRef + m_qwModuleFixup;
				qwModulePtr[i] = qwNewRef;
				_MESSAGE("  %p: %p -> %p", &qwModulePtr[i], qwOldRef, qwNewRef);
			}
		}
		return PluginManager::SafeCallQueryPlugin(plugin, f4se);
	}
};

extern MyPluginManager myPluginManager;