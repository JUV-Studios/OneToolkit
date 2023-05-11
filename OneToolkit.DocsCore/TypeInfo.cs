using System;
using System.IO;
using System.Reflection;
using System.Collections.Generic;
using System.Linq;
using OneToolkit.Lifecycle;

namespace OneToolkit.DocsCore
{
	public interface ITypeInfoWrapper : IContentInfo, IParentHolder<IContentInfo>
	{
	}

	public class TypeInfo : IContentInfo, IParentHolder<IContentInfo>
	{
		protected TypeInfo(Type typeDetails)
		{
			TypeDetails = typeDetails;
		}

		private static readonly StrongValueCache<string, (Uri, Dictionary<Type, string>)> BaseTypeNames = new();

		public Type TypeDetails { get; init; }

		public string MetadataName => TypeDetails.FullName;

		public string Description => string.Empty;

		public Assembly Metadata => TypeDetails.Assembly;

		public HeaderFile Header
		{
			get
			{
				var assemblyName = TypeDetails.Assembly.GetName();
				if (assemblyName.Name == "OneToolkit.Dotnet") return HeaderFile.GetInstance("juv.h", new[] { "C++/CX", "C++/WinRT" }, new[] { "Windows", "macOS", "Linux", "Android" });
				else if (assemblyName.ContentType == AssemblyContentType.WindowsRuntime) return HeaderFile.GetForRtNamespace(TypeDetails.Namespace);
				else return null;
			}
		}

		public WeakReference<IContentInfo> Parent { get; private set; }

		public IEnumerable<IContentInfo> Children => null;

		public ContentInfoKind Kind
		{
			get
			{
				if (TypeDetails.IsClass) return typeof(Delegate).IsAssignableFrom(TypeDetails) ? ContentInfoKind.Delegate : ContentInfoKind.Class;
				else if (TypeDetails.IsEnum) return ContentInfoKind.Enumeration;
				else if (TypeDetails.IsInterface) return ContentInfoKind.Interface;
				else if (TypeDetails.IsValueType) return ContentInfoKind.Structure;
				else return ContentInfoKind.Unknown;
			}
		}

		public AssemblyContentType ContentType => TypeDetails.Assembly.GetName().ContentType;

		public IEnumerable<IGrouping<ContentInfoKind, IContentInfo>> GroupedChildren => throw new NotImplementedException();

		public string GetShortName(string codeLanguage)
		{
			return GetFullName(codeLanguage);
		}

		public string GetFullName(string codeLanguage)
		{
			if (codeLanguage == "C#")
			{
				var typeNames = BaseTypeNames.GetInstance(codeLanguage, () =>
				{
					Uri docsLink = null;
					Dictionary<Type, string> typeNames = new();
					foreach (var content in YamlFileParser.GetContents(Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), @"TypeMappings\csharp.yaml")))
					{
						if (content.Key == "DocsLink") docsLink = new(content.Value);
						else
						{
							typeNames.Add(Type.GetType($"System.{content.Key}"), content.Value);
						}
					}

					return (docsLink, typeNames);
				});

				return typeNames.Item2[TypeDetails];
			}
			else if (codeLanguage.StartsWith("C++"))
			{
				if (TypeDetails == typeof(object))
				{
					switch (codeLanguage)
					{
						case "C++/CLI": return "System::Object^";
						case "C++/CX": return "Platform::Object^";
						case "C++/WinRT": return "winrt::Windows::Foundation::IInspectable";
						default: return "std::any";
					}
				}
				else if (TypeDetails == typeof(string))
				{
					switch (codeLanguage)
					{
						case "C++/CLI": return "System::String^";
						case "C++/CX": return "Platform::Object^";
						case "C++/WinRT": return "winrt::hstring";
						default: return "std::wstring";
					}
				}
				else
				{

				}

				if (TypeDetails == typeof(bool)) return "bool";
				else if (TypeDetails == typeof(byte)) return "juv::uint8";
				else if (TypeDetails == typeof(sbyte)) return "juv::int8";
				else if (TypeDetails == typeof(char)) return "juv::char16";
				else if (TypeDetails == typeof(short)) return "juv::int16";
				else if (TypeDetails == typeof(ushort)) return "juv::uint16";
				else if (TypeDetails == typeof(int)) return "juv::int32";
				else if (TypeDetails == typeof(uint)) return "juv::uint32";
				else if (TypeDetails == typeof(long)) return "juv::int64";
				else if (TypeDetails == typeof(ulong)) return "juv::uint64";
				else if (TypeDetails == typeof(float)) return "juv::float32";
				else if (TypeDetails == typeof(double)) return "juv::float64";
				else if (TypeDetails == typeof(nint) || TypeDetails == typeof(nuint)) return "juv::size";
				else if (TypeDetails == typeof(object))
				{
					if (codeLanguage == "C++/CX") return "Platform::Object";
					else if (codeLanguage == "C++/WinRT") return "winrt::Windows::Foundation::IInspectable";
					else return "std::any";
				}
				else if (TypeDetails == typeof(string))
				{
					if (codeLanguage == "C++/CX") return "Platform::String";
					else if (codeLanguage == "C++/WinRT") return "winrt::hstring";
					else return "std::wstring";
				}
			}
			else if (codeLanguage.StartsWith("Python"))
			{
				if (TypeDetails == typeof(bool)) return "bool";
				else if (TypeDetails == typeof(byte) || TypeDetails == typeof(sbyte) || TypeDetails == typeof(short) || TypeDetails == typeof(ushort) || TypeDetails == typeof(int) || TypeDetails == typeof(uint) || TypeDetails == typeof(long) || TypeDetails == typeof(ulong) || TypeDetails == typeof(nint) || TypeDetails == typeof(nuint)) return "int";
				else if (TypeDetails == typeof(float) || TypeDetails == typeof(double)) return "float";
				else if (TypeDetails == typeof(decimal)) return "decimal.Decimal";
				else if (TypeDetails == typeof(object)) return "object";
				else if (TypeDetails == typeof(string)) return "str";
			}
			else if (codeLanguage.StartsWith("Rust"))
			{

			}

			return TypeDetails.Name;
		}

		public string GetSyntax(string codeLanguage)
		{
			if (codeLanguage == "C#")
			{
				var prefix = Kind switch
				{
					ContentInfoKind.Class => TypeDetails.IsSealed ? "sealed class" : "class",
					ContentInfoKind.Delegate => "delegate",
					ContentInfoKind.Enumeration => "enum",
					ContentInfoKind.Interface => "interface",
					ContentInfoKind.Structure => "struct",
					_ => ""
				};

				return $"{prefix} {TypeDetails.Name}";
			}
			else if (codeLanguage == "C++/CX")
			{
				return Kind switch
				{
					ContentInfoKind.Class => $"ref class {TypeDetails.Name} {(TypeDetails.IsSealed ? "sealed" : string.Empty)}",
					ContentInfoKind.Delegate => $"delegate {TypeDetails.Name}",
					ContentInfoKind.Enumeration => $"enum class {TypeDetails.Name} : {TypeDetails.GetEnumUnderlyingType().Name}",
					ContentInfoKind.Interface => $"interface class {TypeDetails.Name}",
					ContentInfoKind.Structure => $"value class {TypeDetails.Name}",
					_ => ""
				};
			}

			return "";
		}

		public bool IsLanguageSupported(string codeLanguage) => true;
		
		public bool IsPlatformSupported(string platformName)
		{
			// Assume that non-Windows Runtime types are cross-platform.
			if (TypeDetails.Assembly.GetName().ContentType == AssemblyContentType.WindowsRuntime) return platformName == "Windows";
			else return true;
		}

		public void Initialize(IContentInfo parent) => Parent = new(parent);

		public static TypeInfo GetInstance(Type typeDetails) => new TypeInfo(typeDetails);
	}
}

namespace System.Runtime.CompilerServices
{
	internal static class IsExternalInit { }
}