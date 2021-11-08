using System;
using System.IO;
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
		public string Name => Type.Name;

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

		public string MetadataFileName => Path.GetFileName(Type.Assembly.Location);

		public string HeaderFileName => Type.Assembly.GetName().ContentType == AssemblyContentType.WindowsRuntime ? $"winrt/{Type.Namespace}" : string.Empty;
	}
}

namespace System.Runtime.CompilerServices
{
	internal static class IsExternalInit { }
}