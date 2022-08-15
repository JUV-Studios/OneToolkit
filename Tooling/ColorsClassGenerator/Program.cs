using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;

namespace ColorsClassGenerator
{
    internal class Program
    {
        static void Main(string[] args)
        {
			var stringBuilder = new StringBuilder();
			stringBuilder.AppendLine("#pragma once");
			stringBuilder.AppendLine("#include <map>");
			stringBuilder.AppendLine("#include <winrt/Windows.UI.h>");
			stringBuilder.AppendLine();
			stringBuilder.AppendLine("inline auto GetPredefinedColors()");
			stringBuilder.AppendLine("{");
			stringBuilder.AppendLine("\treturn std::map<winrt::hstring, winrt::Windows::UI::Color>");
			stringBuilder.AppendLine("\t{");
			var colorsProperties = typeof(Windows.UI.Colors).GetRuntimeProperties().ToArray();
			var largestNameLength = colorsProperties.Select(colorsProperty => colorsProperty.Name.Length).Max();
			for (int index = 0; index < colorsProperties.Length; ++index)
			{
				var colorsProperty = colorsProperties[index];
				var color = (Windows.UI.Color)colorsProperty.GetValue(null);
				// Align all color values uniformly. 
				var separatedValuesSpace = new string(' ', largestNameLength - colorsProperty.Name.Length + 1);
				stringBuilder.Append($"\t\t{{ L\"{colorsProperty.Name}\",{separatedValuesSpace}{{ {color.A}, {color.R}, {color.G}, {color.B} }} }}");
				stringBuilder.AppendLine(index != colorsProperties.Length - 1 ? "," : "");
			}

			stringBuilder.AppendLine("\t};");
			stringBuilder.Append("}");
			File.WriteAllText("PredefinedColors.h", stringBuilder.ToString());
			Console.ReadKey();
        }
    }
}
