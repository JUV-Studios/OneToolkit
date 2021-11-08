using System;
using System.Linq;
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

		public string Name => _Name;

		public WeakReference<IContentInfo> Parent => null;

		public IEnumerable<IContentInfo> Children => _Children;

		public IEnumerable<IGrouping<TypeKind, TypeInfo>> GroupedChildren => _GroupedChildren ??= from child in _Children
																								  group child by child.Kind;

		public void Dispose()
		{
			_GroupedChildren = null;
		}
	}
}