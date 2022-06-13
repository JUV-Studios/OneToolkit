using System;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Data;

namespace OneToolkit.UI.Xaml.Converters
{
	public sealed class StringLocalizationConverter : DependencyObject, IValueConverter
	{
		public ResourceLoader Context
		{
			get => GetValue(ContextProperty) as ResourceLoader;
			set => SetValue(ContextProperty, value);
		}

		public static DependencyProperty ContextProperty { get; } = DependencyProperty.Register(nameof(Context), typeof(ResourceLoader), typeof(StringLocalizationConverter), new(GetDefaultContext()));

		private static ResourceLoader GetDefaultContext()
		{
			try { return ResourceLoader.GetForViewIndependentUse(); }
			catch { return null; }
		}


		public object Convert(object value, Type targetType, object parameter, string language) => Context.GetString(value.ToString());

		public object ConvertBack(object value, Type targetType, object parameter, string language) => throw new NotImplementedException();
	}
}