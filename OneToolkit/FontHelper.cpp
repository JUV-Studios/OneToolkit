#include "pch.h"
#include "FontHelper.h"
#include "UI.FontHelper.g.cpp"
#include <winrt/OneToolkit.Lifecycle.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

using namespace winrt;
using namespace OneToolkit::Storage;
using namespace OneToolkit::Lifecycle;
using namespace Windows::Globalization;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Interop;
using namespace Microsoft::Graphics::Canvas::Text;

namespace winrt::OneToolkit::UI::implementation
{
	struct FontFamilyIterator : implements<FontFamilyIterator, IIterator<hstring>>
	{
	public:
		FontFamilyIterator(IVectorView<hstring> const& vectorView) : m_VectorView(vectorView)
		{
		}

		hstring Current() const
		{
			return m_VectorView.GetAt(m_CurrentIndex);
		}

		bool HasCurrent() const noexcept
		{
			return m_CurrentIndex < m_VectorView.Size();
		}

		uint32_t GetMany(array_view<hstring> items)
		{
			if (items.size() > m_VectorView.Size()) throw hresult_out_of_bounds();
			for (uint32_t index = 0; index < items.size(); ++index) items[index] = m_VectorView.GetAt(index);
			return items.size();
		}

		bool MoveNext() noexcept
		{
			m_CurrentIndex++;
			return m_CurrentIndex < m_VectorView.Size();
		}
	private:
		uint32_t m_CurrentIndex = 0;
		IVectorView<hstring> m_VectorView;
	};

	struct FontFamilyVectorView : implements<FontFamilyVectorView, IVectorView<hstring>, IIterable<hstring>>
	{
	public:
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
			return make<FontFamilyIterator>(*this);
		}

		// CanvasTextFormat::GetSystemFontFamilies requires Win2D version 1.4.0 or later.
		com_array<hstring> Array = CanvasTextFormat::GetSystemFontFamilies(ApplicationLanguages::Languages());
	};

	IVectorView<hstring> _InstalledFontFamilies;

	IVectorView<hstring> FontHelper::InstalledFontFamilies()
	{
		static slim_mutex singletonLock;
		const slim_lock_guard lockGuard{ singletonLock };
		if (_InstalledFontFamilies == nullptr) _InstalledFontFamilies = make<FontFamilyVectorView>();
		return _InstalledFontFamilies;
	}

	bool FontHelper::IsFontFamilyPresent(hstring const& fontFamilyName)
	{
		auto const& array = get_self<FontFamilyVectorView>(InstalledFontFamilies())->Array;
		return std::find(array.begin(), array.end(), fontFamilyName) != array.end();
	}
}