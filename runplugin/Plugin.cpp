#include "Plugin.h"
#include "MemoryModule.h"

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

FARPROC Plugin::ProcAddress(LPCSTR lpProcName)
{
    return GetProcAddress((HMODULE)m_handle, lpProcName);
}

Plugin::Plugin(const char *pluginPath)
    : Plugin((HANDLE)LoadLibraryA(pluginPath))
{
}

Plugin::Plugin(HANDLE handle) : m_handle(handle)
{
	memset(&m_info, 0, sizeof(m_info));
	m_query = (_F4SEPlugin_Query)ProcAddress("F4SEPlugin_Query");
	m_load = (_F4SEPlugin_Load)ProcAddress("F4SEPlugin_Load");
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
		m_state = k_PluginState_Failed;
		if (m_query(&g_F4SEInterface, &m_info)) {
			m_state = k_PluginState_Valid;
			return true;
		}
		m_state = k_PluginState_Incompatible;
	}
	return false;
}

bool Plugin::Activate()
{
	if (m_state == k_PluginState_Valid) {
		m_state = k_PluginState_Failed;
		if (m_load(&g_F4SEInterface)) {
			m_state = k_PluginState_Active;
			return true;
		}
	}
	return false;
}

MemoryPlugin::MemoryPlugin(const char *pluginPath, INT_PTR loadAddress = 0) :
    Plugin((HANDLE)MemoryLoadLibraryFile(pluginPath, (LPVOID)loadAddress)) {}
