#pragma once
#define declare_multi_converter(Name, Code) [Windows::Foundation::Metadata::WebHostHidden] public ref struct Name sealed : IMultiConverter\
{ virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language) { return ConvertValue(value, targetType, parameter, language); }\
virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language) { return ConvertValue(value, targetType, parameter, language);  }\
virtual Platform::Object^ ConvertValue(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language) { Code } }


namespace OneToolkit::UI::Xaml::Converters
{
#ifdef framework_winui3
	using ResourceLoader = Microsoft::Windows::ApplicationModel::Resources::ResourceLoader;
#else
	using ResourceLoader = Windows::ApplicationModel::Resources::ResourceLoader;
#endif

	[Windows::Foundation::Metadata::WebHostHidden]
	public interface struct IMultiConverter : AppFramework::Data::IValueConverter
	{
		Platform::Object^ ConvertValue(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};

	declare_multi_converter(BoolReverseConverter,
		{
			if (auto flag = dynamic_cast<Platform::IBox<bool>^>(value)) return flag->Value;
			else return !value;
		});

	declare_multi_converter(StringVersionConverter,
		{
			if (auto packageVersion = dynamic_cast<Platform::IBox<Windows::ApplicationModel::PackageVersion>^>(value)) return OneToolkit::System::PackageVersionHelper::Stringify(packageVersion->Value);
			else return OneToolkit::System::PackageVersionHelper::Parse(value->ToString());
		});

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class StringLocalizationConverter sealed : AppFramework::DependencyObject, AppFramework::Data::IValueConverter
	{
	public:
		declare_dependency_property(ResourceLoader^, Context);
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	private:
		static ResourceLoader^ GetDefaultContext();
		static void DependencyPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyPropertyChangedEventArgs^ e) { }
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class NullToVisibilityConverter sealed : AppFramework::DependencyObject, AppFramework::Data::IValueConverter
	{
	public:
		declare_dependency_property(AppFramework::Visibility, ValueForNull);
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	private:
		static void DependencyPropertyChanged(AppFramework::DependencyObject^ sender, AppFramework::DependencyPropertyChangedEventArgs^ e) { }
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref struct StringToVisibilityConverter sealed : AppFramework::Data::IValueConverter
	{
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};
}