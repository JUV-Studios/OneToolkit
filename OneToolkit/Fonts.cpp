#include "UI.Fonts.g.h"
#include <dwrite.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.UserProfile.h>

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace winrt::impl;
using namespace Windows::System::UserProfile;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Foundation::Collections;
using namespace OneToolkit::System;

namespace winrt::OneToolkit::UI
{
	namespace implementation
	{
		using RtlGetVersion = int(_stdcall*)(PRTL_OSVERSIONINFOW);

		struct Fonts : static_t, FontsT<Fonts>
		{
			static hstring DefaultTextFontFamily()
			{
				RTL_OSVERSIONINFOW versionInfo{ .dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW) };
				DynamicLibrary ntdll{ L"ntdll.dll" };
				ntdll.GetProcAddress<RtlGetVersion>("RtlGetVersion")(&versionInfo);
				return versionInfo.dwBuildNumber >= 21376 ? L"Segoe UI Variable" : L"Segoe UI";
			}

			static hstring DefaultIconFontFamily()
			{
				RTL_OSVERSIONINFOW versionInfo{ .dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW) };
				DynamicLibrary ntdll{ L"ntdll.dll" };
				ntdll.GetProcAddress<RtlGetVersion>("RtlGetVersion")(&versionInfo);
				return versionInfo.dwBuildNumber >= 21327 ? L"Segoe Fluent Icons" : L"Segoe MDL2 Assets";
			}

			static IVectorView<hstring> InstalledFontFamilies()
			{
				std::vector<hstring> fontFamilies;
				com_ptr<IDWriteFactory> factory;
				com_ptr<IDWriteFontCollection> fontCollection;
				check_hresult(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(factory.put())));
				check_hresult(factory->GetSystemFontCollection(fontCollection.put(), true));
				auto const familyCount = fontCollection->GetFontFamilyCount();
				fontFamilies.reserve(familyCount);
				for (uint32 index = 0; index < familyCount; ++index)
				{
					com_ptr<IDWriteFontFamily> fontFamily;
					com_ptr<IDWriteLocalizedStrings> familyNames;
					check_hresult(fontCollection->GetFontFamily(index, fontFamily.put()));
					check_hresult(fontFamily->GetFamilyNames(familyNames.put()));
					int exists = false;
					uint32 localeIndex = 0;
					auto localeName = GlobalizationPreferences::Languages().GetAt(0);
					check_hresult(familyNames->FindLocaleName(localeName.data(), &localeIndex, &exists));
					if (!exists)
					{
						check_hresult(familyNames->FindLocaleName(L"en-us", &localeIndex, &exists));
						if (!exists) localeIndex = 0;
					}

					uint32 length = 0;
					check_hresult(familyNames->GetStringLength(localeIndex, &length));
					length++;
					hstring_builder stringBuilder{ length };
					check_hresult(familyNames->GetString(localeIndex, stringBuilder.data(), length));
					fontFamilies.emplace_back(stringBuilder.to_hstring());
				}

				std::sort(fontFamilies.begin(), fontFamilies.end());
				return single_threaded_vector<hstring>(std::move(fontFamilies)).GetView();
			}
		};
	}

	namespace factory_implementation
	{
		struct Fonts : FontsT<Fonts, implementation::Fonts>
		{
		};
	}
}

#include "UI.Fonts.g.cpp"