using System;
using System.Collections.Generic;
using System.Reflection;

namespace OneToolkit.DocsCore
{
	public sealed record CppType(string Name, TypeKind Kind, HeaderFile Header, Assembly Metadata, IEnumerable<IContentInfo> Children, WeakReference<IContentInfo> Parent) : IContentInfo
	{
		public IEnumerable<string> SupportedLanguages => new[] { "C++" };

		public string GetName(string codeLanguage) => Name;

		public string GetSyntax(string codeLanguage) => null;
	}
}