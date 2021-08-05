using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using Microsoft.Toolkit.Uwp;
using OneToolkit.Showcase.Views;

namespace OneToolkit.Showcase.Models
{
	public enum TypeKind : byte
	{
		None, Class, Delegate, Enumeration, Interface, Structure
	}

	public sealed record TypeInfo(Type Type) : IContentInfo
	{
		public string Name => Type.Name;

		public string DisplayName
		{
			get
			{
				var suffix = Kind switch
				{
					TypeKind.Class => "ClassText".GetLocalized(),
					TypeKind.Enumeration => "EnumerationText".GetLocalized(),
					TypeKind.Interface => "InterfaceText".GetLocalized(),
					TypeKind.Structure => "StructureText".GetLocalized(),
					_ => string.Empty
				};

				return $"{Type.Name} {suffix}";
			}
		}

		public string Caption
		{
			get
			{
				if (Kind == TypeKind.Class) return $"{Type.Name}, {"ClassText".GetLocalized()}";
				else return Type.Name;
			}
		}

		public IContentInfo Parent => (from nameSpace in ApiReference.FoundNamespaces
									   where nameSpace.Children.Contains(this)
									   select nameSpace).First();

		public IEnumerable<IContentInfo> Children => null;

		public Type PageType => typeof(TypeInfoPage);

		public TypeKind Kind
		{
			get
			{
				if (Type.IsClass) return Type.BaseType != typeof(Delegate) ? TypeKind.Class : TypeKind.Delegate;
				else if (Type.IsEnum) return TypeKind.Enumeration;
				else if (Type.IsInterface) return TypeKind.Interface;
				else if (Type.IsValueType) return TypeKind.Structure;
				else return TypeKind.None;
			}
		}

		public string Metadata => Path.GetFileName(Type.Assembly.Location);
	}
}