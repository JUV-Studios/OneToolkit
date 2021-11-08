#pragma once
#define declare_multi_converter virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language) { return ConvertValue(value, targetType, parameter, language); } virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language) { return ConvertValue(value, targetType, parameter, language); }

namespace OneToolkit::UI::Xaml::Converters
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref struct BoolReverseConverter sealed : Framework::Data::IValueConverter
	{
		declare_multi_converter;
		Platform::Object^ ConvertValue(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref struct StringVersionConverter sealed : Framework::Data::IValueConverter
	{
		declare_multi_converter;
		Platform::Object^ ConvertValue(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class StringLocalizationConverter sealed : Framework::DependencyObject, Framework::Data::IValueConverter
	{
	public:
		declare_dependency_property(Windows::ApplicationModel::Resources::ResourceLoader^, Context);
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	private:
		static void DependencyPropertyChanged(Framework::DependencyObject^ sender, Framework::DependencyPropertyChangedEventArgs^ e) { }
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref struct StringToVisibilityConverter sealed : Framework::Data::IValueConverter
	{
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};
}