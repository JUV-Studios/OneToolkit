using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;

namespace OneToolkit.DocsCore
{
	public sealed class TypeGroup : IContentInfo, IDisposable
	{
		private readonly string _Name;
		private readonly IEnumerable<TypeInfo> _Children;
		private IEnumerable<IGrouping<TypeKind, TypeInfo>> _GroupedChildren;

		public TypeGroup(string name, IEnumerable<Type> types)
		{
			WeakReference<IContentInfo> weakThis = new(this);
			_Name = name;
			_Children = from type in types
						select new TypeInfo(type, weakThis);
		}

		public Assembly Metadata => null;

		public HeaderFile Header => null;

		public WeakReference<IContentInfo> Parent => null;

		public IEnumerable<IContentInfo> Children => _Children;

		public IEnumerable<IGrouping<TypeKind, TypeInfo>> GroupedChildren => _GroupedChildren ??= from child in _Children
																								  group child by child.Kind;

		public IEnumerable<string> SupportedLanguages => null;

		public string GetName(string codeLanguage) => codeLanguage.StartsWith("C++") ? _Name.Replace(".", "::") : _Name;

		public string GetSyntax(string codeLanguage) => null;

		public void Dispose()
		{
			_GroupedChildren = null;
		}
	}
}