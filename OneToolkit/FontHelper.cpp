#include "pch.h"
#include "FontHelper.h"
#include "UI.FontHelper.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Foundation::Collections;

namespace winrt::OneToolkit::UI::implementation
{
	using RtlGetVersion = int(_stdcall*)(PRTL_OSVERSIONINFOW);

	hstring FontHelper::DefaultTextFontFamily()
	{
		RTL_OSVERSIONINFOW versionInfo{ .dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW) };
		Runtime::DynamicModule ntdll{ L"ntdll.dll" };
		ntdll.GetProcAddress<RtlGetVersion>("RtlGetVersion")(&versionInfo);
		return versionInfo.dwBuildNumber >= 21376 ? L"Segoe UI Variable" : L"Segoe UI";
	}

	hstring FontHelper::DefaultIconFontFamily()
	{
		RTL_OSVERSIONINFOW versionInfo { .dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW) };
		Runtime::DynamicModule ntdll { L"ntdll.dll" };
		ntdll.GetProcAddress<RtlGetVersion>("RtlGetVersion")(&versionInfo);
		return versionInfo.dwBuildNumber >= 21327 ? L"Segoe Fluent Icons" : L"Segoe MDL2 Assets";
	}

	IVectorView<hstring> FontHelper::InstalledFontFamilies()
	{
		std::vector<hstring> fontFamilies;
		com_ptr<IDWriteFactory> factory;
		com_ptr<IDWriteFontCollection> fontCollection;
		check_hresult(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(factory.put())));
		check_hresult(factory->GetSystemFontCollection(fontCollection.put(), true));
		auto familyCount = fontCollection->GetFontFamilyCount();
		fontFamilies.reserve(familyCount);
		for (uint32 index = 0; index < familyCount; ++index)
		{
			com_ptr<IDWriteFontFamily> fontFamily;
			com_ptr<IDWriteLocalizedStrings> familyNames;
			check_hresult(fontCollection->GetFontFamily(index, fontFamily.put()));
			check_hresult(fontFamily->GetFamilyNames(familyNames.put()));
			int exists = false;
			uint32 localeIndex = 0;
			std::array<wchar, LOCALE_NAME_MAX_LENGTH> localeName;
			if (GetUserDefaultLocaleName(localeName.data(), static_cast<int>(localeName.size()))) check_hresult(familyNames->FindLocaleName(localeName.data(), &localeIndex, &exists));
			if (!exists) check_hresult(familyNames->FindLocaleName(L"en-us", &localeIndex, &exists));
			if (!exists) localeIndex = 0;
			uint32 length = 0;
			check_hresult(familyNames->GetStringLength(localeIndex, &length));
			length++;
			impl::hstring_builder stringBuilder{ length };
			check_hresult(familyNames->GetString(localeIndex, stringBuilder.data(), length));
			fontFamilies.emplace_back(stringBuilder.to_hstring());
		}

		std::sort(fontFamilies.begin(), fontFamilies.end());
		return single_threaded_vector<hstring>(std::move(fontFamilies)).GetView();
	}
}