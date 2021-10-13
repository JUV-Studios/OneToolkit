using System;

namespace OneToolkit.Storage
{
	public static class FileSizeHelper
	{
		private static string[] BinaryUnits = { "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB" };

		private static string[] DecimalUnits = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

		public static string Stringify(ulong sizeInBytes, bool useBinaryPrefix)
		{
			byte index = 0;
			var preciseSize = Convert.ToDouble(sizeInBytes);
			var multiple = useBinaryPrefix ? 1024 : 1000;
			while (preciseSize >= multiple)
			{
				preciseSize /= multiple;
				++index;
			}

			if (index > 8)
			{
				index = 0;
				preciseSize = Convert.ToDouble(sizeInBytes);
			}

			return $"{preciseSize} {(useBinaryPrefix ? BinaryUnits[index] : DecimalUnits[index])}";
		}

		public static ulong Parse(string formattedString)
		{
			if (formattedString.EndsWith(" B")) return Convert.ToUInt64(formattedString.Substring(0, formattedString.Length - 2));
			throw new NotImplementedException();
		}
	}
}