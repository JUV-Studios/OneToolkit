// Code adapted from https://github.com/CommunityToolkit/WindowsCommunityToolkit/tree/rel/7.1.0/Microsoft.Toolkit.Uwp.UI/Converters.

#include "UI.Converters.BoolReverseConverter.g.h"
#include "UI.Converters.StringLocalizationConverter.g.h"
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.Resources.h>

using winrt::Windows::Foundation::IInspectable;
using winrt::Windows::UI::Xaml::Interop::TypeName;

namespace winrt::OneToolkit::UI::Converters
{
	namespace implementation
	{
		template <typename Derived, template <typename, typename...> typename DerivedT>
		struct ValueConverterBase : DerivedT<Derived, Windows::UI::Xaml::Data::IValueConverter, Microsoft::UI::Xaml::Data::IValueConverter> 
		{
		};

		struct BoolReverseConverter : ValueConverterBase<BoolReverseConverter, BoolReverseConverterT>
		{
		public:
			IInspectable Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language)
			{
				return ConvertValue(value, targetType, parameter, language);
			}

			IInspectable ConvertBack(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language)
			{
				return ConvertValue(value, targetType, parameter, language);
			}
		private:
			IInspectable ConvertValue(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language)
			{
				return box_value(!unbox_value<bool>(value));
			}
		};

		struct StringLocalizationConverter : ValueConverterBase<StringLocalizationConverter, StringLocalizationConverterT>
		{
			IInspectable Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language)
			{
				return box_value(Windows::ApplicationModel::Resources::ResourceLoader::GetForViewIndependentUse().GetString(unbox_value<hstring>(value)));
			}

			IInspectable ConvertBack(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language)
			{
				throw hresult_not_implemented();
			}
		};
	}

	namespace factory_implementation
	{
		struct BoolReverseConverter : BoolReverseConverterT<BoolReverseConverter, implementation::BoolReverseConverter>
		{
		};

		struct StringLocalizationConverter : StringLocalizationConverterT< StringLocalizationConverter, implementation::StringLocalizationConverter>
		{
		};
	}
}

#include "UI.Converters.BoolReverseConverter.g.cpp"
#include "UI.Converters.StringLocalizationConverter.g.cpp"