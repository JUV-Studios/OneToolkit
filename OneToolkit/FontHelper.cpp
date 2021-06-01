#include "pch.h"
#include "FontHelper.h"
#include "UI.FontHelper.g.cpp"
#include <dwrite.h>

using namespace winrt;
using namespace Windows::Globalization;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Foundation::Collections;

namespace winrt::OneToolkit::UI::implementation
{
	IVectorView<hstring> FontHelper::InstalledFontFamilies()
	{
		static slim_mutex singletonLock;
		const slim_lock_guard lockGuard { singletonLock };
		static IVectorView<hstring> _InstalledFontFamilies;
		if (_InstalledFontFamilies == nullptr)
		{
			std::vector<hstring> fontFamilies;
			com_ptr<IDWriteFactory> factory;
			com_ptr<IDWriteFontCollection> fontCollection;
			check_hresult(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(factory.put())));
			check_hresult(factory->GetSystemFontCollection(fontCollection.put(), true));
			auto familyCount = fontCollection->GetFontFamilyCount();
			fontFamilies.reserve(familyCount);
			for (uint32_t index = 0; index < familyCount; ++index)
			{
				uint32_t localeIndex = 0;
				int exists = false;
				wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
				com_ptr<IDWriteFontFamily> fontFamily;
				com_ptr<IDWriteLocalizedStrings> familyNames;
				check_hresult(fontCollection->GetFontFamily(index, fontFamily.put()));
				check_hresult(fontFamily->GetFamilyNames(familyNames.put())); // Get a list of localized strings for the family name.
				if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH)) // Get the default locale for this user.
				{
					// If the default locale is returned, find that locale name, otherwise use "en-us".
					check_hresult(familyNames->FindLocaleName(localeName, &localeIndex, &exists));
				}

				if (!exists) check_hresult(familyNames->FindLocaleName(L"en-us", &localeIndex, &exists)); // If the above find did not find a match, retry with US English
				if (!exists) localeIndex = 0; // If the specified locale doesn't exist, select the first on the list.
				uint32_t length = 0;
				check_hresult(familyNames->GetStringLength(localeIndex, &length));
				length++;
				impl::hstring_builder stringBuilder { length }; // Allocate a string big enough to hold the name.
				check_hresult(familyNames->GetString(localeIndex, stringBuilder.data(), length));
				fontFamilies.emplace_back(stringBuilder.to_hstring());
			}

			std::sort(fontFamilies.begin(), fontFamilies.end());
			_InstalledFontFamilies = single_threaded_vector<hstring>(std::move(fontFamilies)).GetView();
		}

		return _InstalledFontFamilies;
	}
}