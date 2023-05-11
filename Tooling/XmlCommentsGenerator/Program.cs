using OneToolkit.Metadata.Documentation;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace XmlCommentsGenerator
{
    internal class Program
    {
		static void Main(string[] args)
		{
			var assembly = typeof(SummaryAttribute).Assembly;
			StringBuilder xmlTextBuilder = new StringBuilder();
			xmlTextBuilder.AppendLine("<?xml version=\"1.0\" encoding =\"UTF-8\" /> ");
			xmlTextBuilder.AppendLine("<doc>");
			xmlTextBuilder.AppendLine("\t<assembly>");
			xmlTextBuilder.AppendLine($"\t\t<name>{assembly.FullName}</name>");
			xmlTextBuilder.AppendLine("\t</assembly>");
			xmlTextBuilder.AppendLine("\t<members>");
			foreach (var type in (typeof(SummaryAttribute).Assembly).ExportedTypes)
			{
				xmlTextBuilder.AppendLine($"\t\t<member name=\"T:{type}\">");
				var summaryAttribute = type.GetCustomAttribute<SummaryAttribute>();
				if (summaryAttribute != null)
				{
					xmlTextBuilder.AppendLine($"\t\t\t<summary>{summaryAttribute.Text}</summary>");
				}

				xmlTextBuilder.AppendLine($"\t\t</member>");
			}

			xmlTextBuilder.AppendLine("\t</members>");
			xmlTextBuilder.AppendLine("</doc>");
			File.WriteAllText("onetoolkit.xml", xmlTextBuilder.ToString());
		}
    }
}
