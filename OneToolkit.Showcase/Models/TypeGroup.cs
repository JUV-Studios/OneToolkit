using System;
using System.Linq;
using System.Collections.Generic;
using Microsoft.Toolkit.Uwp;
using OneToolkit.Showcase.Views;

namespace OneToolkit.Showcase.Models
{
	public sealed class TypeGroup : IContentInfo, IDisposable
	{
		private readonly string _Name;
		private readonly IEnumerable<TypeInfo> _Children;
		private IEnumerable<TypeInfo> _Classes;
		private IEnumerable<TypeInfo> _Enumerations;
		private IEnumerable<TypeInfo> _Interfaces;
		private IEnumerable<TypeInfo> _Structures;

		public TypeGroup(string name, IEnumerable<Type> types)
		{
			_Name = name;
			_Children = from type in types
						select new TypeInfo(type);
		}

		public string Name => _Name;

		public string DisplayName => $"{Name} {"NamespaceText".GetLocalized()}";

		public string Caption => $"{Name} {"NamespaceText".GetLocalized()}";

		public IContentInfo Parent => null;

		public Type PageType => typeof(NamespacePage);

		public IEnumerable<IContentInfo> Children => _Children;

		public IEnumerable<IContentInfo> Classes => _Classes ??= from child in _Children
																 where child.Kind == TypeKind.Class
																 select child;

		public IEnumerable<IContentInfo> Enumerations => _Enumerations ??= from child in _Children
																		   where child.Kind == TypeKind.Enumeration
																		   select child;

		public IEnumerable<IContentInfo> Interfaces => _Interfaces ??= from child in _Children
																	   where child.Kind == TypeKind.Interface
																	   select child;

		public IEnumerable<IContentInfo> Structures => _Structures ??= from child in _Children
																	   where child.Kind == TypeKind.Structure
																	   select child;

		public void Dispose()
		{
			_Classes = null;
			_Enumerations = null;
			_Interfaces = null;
			_Structures = null;
		}
	}
}