// Code adapted from https://github.com/CommunityToolkit/WindowsCommunityToolkit/tree/rel/7.1.0/Microsoft.Toolkit.Uwp.UI/Converters.

#include "pch.h"
#include "XamlFramework/Base.h"
#include "Converters.BoolReverseConverter.g.h"
#include "Converters.StringLocalizationConverter.g.h"
#include <winrt/Windows.Foundation.h>
#if __has_include(<winrt/Microsoft.UI.h>)
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>
#else
#include <winrt/Windows.ApplicationModel.Resources.h>
#endif

using winrt::Windows::Foundation::IInspectable;
using winrt::Windows::UI::Xaml::Interop::TypeName;
#if __has_include(<winrt/Microsoft.UI.h>)
using winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader;
#else
using winrt::Windows::ApplicationModel::Resources::ResourceLoader;
#endif

namespace winrt::OneToolkit::UI::Xaml::Converters
{
	namespace implementation
	{
		struct BoolReverseConverter : BoolReverseConverterT<BoolReverseConverter>
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

		struct StringLocalizationConverter : StringLocalizationConverterT<StringLocalizationConverter>
		{
		public:
			IInspectable Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language)
			{
				return box_value(GetResourceLoader().GetString(unbox_value<hstring>(value)));
			}

			IInspectable ConvertBack(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language)
			{
				throw hresult_not_implemented();
			}
		private:
			static ResourceLoader GetResourceLoader()
			{
#if __has_include(<winrt/Microsoft.UI.h>)
				return ResourceLoader();
#else
				return ResourceLoader::GetForViewIndependentUse();
#endif
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

#include "Converters.BoolReverseConverter.g.cpp"
#include "Converters.StringLocalizationConverter.g.cpp"