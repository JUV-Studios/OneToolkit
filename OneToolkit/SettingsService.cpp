#include "pch.h"
#include "SettingsService.h"
#include "Storage.SettingsService.g.cpp"

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::Storage::implementation
{
	SettingsService::SettingsService(ApplicationDataContainer const& dataContainer) : m_DataContainer(dataContainer)
	{
	}

	ApplicationDataContainer SettingsService::DataContainer() const noexcept
	{
		return m_DataContainer;
	}

	IInspectable SettingsService::GetValue(hstring const& key, IInspectable const& defaultValue)
	{
		if (m_DataContainer.Values().HasKey(key)) return m_DataContainer.Values().Lookup(key);
		else
		{
			SetValue(key, defaultValue);
			return defaultValue;
		}
	}

	void SettingsService::SetValue(hstring const& key, IInspectable const& newValue)
	{
		static slim_mutex settingsValueLock;
		const slim_lock_guard lockGuard { settingsValueLock };
		if (!m_DataContainer.Values().Insert(key, newValue)) throw hresult_error(E_FAIL);
	}
}