#pragma once

#include <f4se/PluginAPI.h>

extern const F4SEInterface g_F4SEInterface;

class Plugin
{
	PluginInfo m_info;
	_F4SEPlugin_Query	m_query;
	_F4SEPlugin_Load	m_load;

	enum {
		k_PluginState_Failed = -1000,
		k_PluginState_Invalid,
		k_PluginState_Incompatible,
		k_PluginState_Initial = 0,
		k_PluginState_Valid,
		k_PluginState_Active = 1000
	} m_state;

protected:
    const HANDLE m_handle;
    Plugin(HANDLE);

public:
    Plugin(const char *pluginPath);
	const PluginInfo &GetInfo() { return m_info; }
	int GetState() { return (int)m_state; }
	bool Query();
	bool Activate();
    virtual INT_PTR BaseAddress() { return (INT_PTR)m_handle; }
    virtual FARPROC ProcAddress(LPCSTR);
};

class MemoryPlugin : public Plugin {
public:
    MemoryPlugin(const char *, INT_PTR);
};