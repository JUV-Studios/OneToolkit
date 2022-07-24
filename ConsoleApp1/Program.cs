using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text;

namespace ConsoleApp1
{
    internal class Program
    {
        static void Main(string[] args)
        {
			var version = FileVersionInfo.GetVersionInfo(Assembly.GetExecutingAssembly().Location).FileVersion;
			var stringBuilder = new StringBuilder();
			stringBuilder.AppendLine("#pragma once");
			stringBuilder.AppendLine();
			stringBuilder.AppendLine("namespace OneToolkit::UI");
			stringBuilder.AppendLine("{");
			stringBuilder.AppendLine("\tstruct Colors");
			stringBuilder.AppendLine("\t{");
			stringBuilder.Append("\t\tColors() = delete;");
			foreach (var property in typeof(Windows.UI.Colors).GetRuntimeProperties())
			{
				stringBuilder.AppendLine("\n");
				var color = (Windows.UI.Color)property.GetValue(null);
				stringBuilder.AppendLine($"\t\tstatic constexpr Color {property.Name}() noexcept");
				stringBuilder.AppendLine("\t\t{");
				stringBuilder.AppendLine($"\t\t\treturn {{ {color.A}, {color.R}, {color.G}, {color.B} }};");
				stringBuilder.AppendLine("\t\t}");
			}

			stringBuilder.AppendLine("\t};");
			stringBuilder.Append("}");
			File.WriteAllText("Colors.h", stringBuilder.ToString());
			Console.ReadKey();
        }
    }
}
