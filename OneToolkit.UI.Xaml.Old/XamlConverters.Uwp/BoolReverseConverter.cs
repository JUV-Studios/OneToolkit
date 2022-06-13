using System;
using System.Runtime.InteropServices.WindowsRuntime;

namespace OneToolkit.UI.Xaml.Converters
{
	public sealed class BoolReverseConverter : IMultiConverter
	{
		[return: ReturnValueName("result")]
		public object ConvertValue(object value, Type targetType, object parameter, string language)
		{
			if (value is bool flag) return !flag;
			else return value == null;
		}

		public object Convert(object value, Type targetType, object parameter, string language) => ConvertValue(value, targetType, parameter, language);

		public object ConvertBack(object value, Type targetType, object parameter, string language) => ConvertValue(value, targetType, parameter, language);
	}
}
