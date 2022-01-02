using System;
using System.Reflection;
using System.Collections.Generic;

namespace OneToolkit.DocsCore
{
	public enum TypeKind : byte
	{
		None, Class, Delegate, Enumeration, Interface, Structure
	}

	public sealed record TypeInfo(Type Type, WeakReference<IContentInfo> Parent) : IContentInfo
	{
		public Assembly Metadata => Type.Assembly;

		public HeaderFile Header
		{
			get
			{
				
				var assemblyName = Type.Assembly.GetName();
				if (assemblyName.Name == "OneToolkit.Dotnet") return HeaderFile.GetInstance("juv.h", new[] { "C++/CX", "C++/WinRT" });
				else if (assemblyName.ContentType == AssemblyContentType.WindowsRuntime) return HeaderFile.GetInstance($"winrt/{Type.Namespace}", new[] { "C++/WinRT" });
				else return null;
			}
		}

		public IEnumerable<IContentInfo> Children => null;

		public TypeKind Kind
		{
			get
			{
				if (Type.IsClass) return typeof(Delegate).IsAssignableFrom(Type) ? TypeKind.Class : TypeKind.Delegate;
				else if (Type.IsEnum) return TypeKind.Enumeration;
				else if (Type.IsInterface) return TypeKind.Interface;
				else if (Type.IsValueType) return TypeKind.Structure;
				else return TypeKind.None;
			}
		}

		public IEnumerable<string> SupportedLanguages => null;

		public string GetName(string codeLanguage) => Type.Name;

		public string GetSyntax(string codeLanguage)
		{
			string prefix = null;
			if (codeLanguage == "C#")
			{
				return Kind switch
				{
					TypeKind.Class => Type.IsSealed ? "sealed class" : "class",
					TypeKind.Delegate => "delegate",
					TypeKind.Enumeration => "enum",
					TypeKind.Interface => "interface",
					TypeKind.Structure => "struct"
				} + Type.Name;
			}

			return prefix;
		}
	}
}

namespace System.Runtime.CompilerServices
{
	internal static class IsExternalInit { }
}