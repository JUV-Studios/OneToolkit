using System;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel;
using OneToolkit.System;

namespace OneToolkit.UI.Xaml.Converters
{
	public sealed class StringVersionConverter : IMultiConverter
	{
		[return: ReturnValueName("result")]
		public object ConvertValue(object value, Type targetType, object parameter, string language)
		{
			if (value is string text)
			{
				if (targetType == typeof(Version)) return new Version(text);
				else return PackageVersionHelper.Parse(text);
			}
			else if (value is Version version)
			{
				return version.ToString();
			}
			else if (value is PackageVersion packageVersion)
			{
				return PackageVersionHelper.Stringify(packageVersion);
			}
			else
			{
				return value.ToString();
			}
		}

		public object Convert(object value, Type targetType, object parameter, string language) => ConvertValue(value, targetType, parameter, language);

		public object ConvertBack(object value, Type targetType, object parameter, string language) => ConvertValue(value, targetType, parameter, language);
	}
}
