#include "Plugin.h"

static UInt32 GetReleaseIndex() { return 17; }
static PluginHandle GetPluginHandle() { return (PluginHandle)1; }

static void *QueryInterface(UInt32 id)
{
	_MESSAGE("QueryInterface called with id of %u", id);

	return NULL;
}

const F4SEInterface g_F4SEInterface =
{
	0x010A06A0,		// 1.10.106.0
	0x010A06A0,
	0, 0,
	QueryInterface,
	GetPluginHandle,
	GetReleaseIndex
};

Plugin::Plugin(const char *pluginPath)
	: m_handle(LoadLibraryA(pluginPath))
{
	memset(&m_info, 0, sizeof(m_info));
	m_query = (_F4SEPlugin_Query)GetProcAddress(m_handle, "F4SEPlugin_Query");
	m_load = (_F4SEPlugin_Load)GetProcAddress(m_handle, "F4SEPlugin_Load");
	if (m_query && m_load) {
		m_state = k_PluginState_Initial;
	}
	else {
		m_state = k_PluginState_Invalid;
	}
}

bool Plugin::Query()
{
	if (m_state == k_PluginState_Initial) {
		if (m_query(&g_F4SEInterface, &m_info)) {
			m_state = k_PluginState_Valid;
			return true;
		}
		m_state = k_PluginState_Incompatible;
	}
	return false;
}

bool Plugin::SafeQuery()
{
	__try {
		return Query();
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		m_state = k_PluginState_Failed;
	}
	return false;
}

bool Plugin::Load()
{
	if (m_state == k_PluginState_Valid) {
		if (m_load(&g_F4SEInterface)) {
			m_state = k_PluginState_Active;
			return true;
		}
		m_state = k_PluginState_Failed;
	}
	return false;
}

bool Plugin::SafeLoad()
{
	__try {
		return Load();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		m_state = k_PluginState_Failed;
	}
	return false;
}
