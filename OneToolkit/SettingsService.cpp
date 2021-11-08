#include "pch.h"
#include "SettingsService.h"
#include "Storage.SettingsService.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Data::Json;

namespace winrt::OneToolkit::Storage::implementation
{
	SettingsService::SettingsService(ApplicationDataContainer const& dataContainer) : DataContainer(dataContainer)
	{
		if (!dataContainer) throw hresult_invalid_argument(L"DataContainer for a SettingsService instance must not be null.");
	}

	IInspectable SettingsService::GetValue(hstring const& key, IInspectable const& defaultValue) const
	{
		if (DataContainer().Values().HasKey(key))
		{
			return DataContainer().Values().Lookup(key);
		}
		else
		{
			SetValue(key, defaultValue);
			return defaultValue;
		}
	}

	void SettingsService::SetValue(hstring const& key, IInspectable const& newValue) const
	{
		static slim_mutex settingsValueLock;
		const slim_lock_guard lockGuard { settingsValueLock };
		DataContainer().Values().Insert(key, newValue);
	}
}