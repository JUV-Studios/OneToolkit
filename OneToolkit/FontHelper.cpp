#include "pch.h"
#include "FontHelper.h"
#include "UI.FontHelper.g.cpp"
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

using namespace winrt;
using namespace Windows::Globalization;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Foundation::Collections;
using namespace Microsoft::Graphics::Canvas::Text;

namespace winrt::OneToolkit::UI::implementation
{
	struct FontFamilyVectorView : implements<FontFamilyVectorView, IVectorView<hstring>, IIterable<hstring>>
	{
	public:
		FontFamilyVectorView()
		{
			for (uint32_t i = 1; i < Array.size(); i++)
			{
				int index = i - 1;
				auto current = Array[i];
				while (index >= 0 && Array[index] > current)
				{
					Array[index + 1] = Array[index];
					index--;
				}

				Array[index + 1] = current;
			}
		}

		uint32_t Size() const noexcept
		{
			return Array.size();
		}

		hstring GetAt(uint32_t index) const
		{
			return Array.at(index);
		}

		bool IndexOf(hstring const& value, uint32_t& index)
		{
			auto found = std::find(Array.begin(), Array.end(), value);
			index = static_cast<uint32_t>(found - Array.begin());
			return found != Array.end();
		}

		uint32_t GetMany(uint32_t startIndex, array_view<hstring> items)
		{
			if (items.size() > Array.size()) return static_cast<uint32_t>(E_BOUNDS);
			else
			{
				uint32_t counter = 0;
				for (uint32_t index = startIndex; index < Array.size(); ++index)
				{
					items[counter] = Array[index];
					counter++;
				}

				return static_cast<uint32_t>(S_OK);
			}
		}

		IIterator<hstring> First() const
		{
			return juv::collections::raw_array_iterator(Array);
		}

		// CanvasTextFormat::GetSystemFontFamilies requires Win2D version 1.4.0 or later.
		com_array<hstring> Array = CanvasTextFormat::GetSystemFontFamilies(ApplicationLanguages::Languages());
	};

	IVectorView<hstring> FontHelper::InstalledFontFamilies()
	{
		static slim_mutex singletonLock;
		const slim_lock_guard lockGuard{ singletonLock };
		static IVectorView<hstring> _InstalledFontFamilies;
		if (_InstalledFontFamilies == nullptr) _InstalledFontFamilies = make<FontFamilyVectorView>();
		return _InstalledFontFamilies;
	}

	bool FontHelper::IsFontFamilyPresent(hstring const& fontFamilyName)
	{
		auto const& array = get_self<FontFamilyVectorView>(InstalledFontFamilies())->Array;
		return std::find(array.begin(), array.end(), fontFamilyName) != array.end();
	}
}