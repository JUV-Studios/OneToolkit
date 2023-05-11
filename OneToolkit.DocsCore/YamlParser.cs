using System.Collections.Generic;
using System.IO;

namespace OneToolkit.DocsCore
{
	public static class YamlFileParser
	{
		public static Dictionary<string, string> GetContents(string filePath)
		{
			Dictionary<string, string> result = new();
			var lines = File.ReadAllLines(filePath);
			foreach (var line in lines)
			{
				var components = line.Split(':');
				var key = components[0].Trim();
				var value = components.Length > 1 ? string.Join(":", components, 1, components.Length - 1).Trim() : string.Empty;
				result.Add(key, value);
			}

			return result;
		}
	}
}
