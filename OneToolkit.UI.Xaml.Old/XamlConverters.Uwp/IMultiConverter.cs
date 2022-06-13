using System;
using Windows.UI.Xaml.Data;
using System.Runtime.InteropServices.WindowsRuntime;

namespace OneToolkit.UI.Xaml.Converters
{
	public interface IMultiConverter : IValueConverter
	{
		[return: ReturnValueName("result")]
		object ConvertValue(object value, Type targetType, object parameter, string language);
	}
}
