using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using OneToolkit.Lifecycle;

namespace OneToolkit.DocsCore
{
	public sealed class TypeGroup : IContentInfo
	{
		private readonly WeakReference<IEnumerable<IGrouping<ContentInfoKind, IContentInfo>>> _GroupedChildren = new(null);

		private static readonly WeakValueCache<string, TypeGroup> Namespaces = new();

		private TypeGroup(string nameSpace, IEnumerable<IContentInfo> children)
		{
			foreach (var child in children)
			{
				if (child is IParentHolder<IContentInfo> holder) holder.Initialize(this);
			}

			Children = children;
			MetadataName = nameSpace;
		}

		public string MetadataName { get; init; }

		public string Description => string.Empty;

		public ContentInfoKind Kind => ContentInfoKind.Namespace;

		public AssemblyContentType ContentType => Children.Any(child => child.ContentType == AssemblyContentType.WindowsRuntime) ? AssemblyContentType.WindowsRuntime : AssemblyContentType.Default;

		public Assembly Metadata => null;

		public HeaderFile Header => ContentType == AssemblyContentType.WindowsRuntime ? HeaderFile.GetForRtNamespace(MetadataName) : null;

		public WeakReference<IContentInfo> Parent => null;

		public IEnumerable<IContentInfo> Children { get; init; }

		public IEnumerable<IGrouping<ContentInfoKind, IContentInfo>> GroupedChildren
		{
			get
			{
				IEnumerable<IGrouping<ContentInfoKind, IContentInfo>> result;
				if (_GroupedChildren.TryGetTarget(out var grouping)) 
				{
					result = grouping;
				}
				else
				{
					result = from child in Children
							 group child by child.Kind;

					_GroupedChildren.SetTarget(result);
				}

				return result; 
			}
		}

		public string GetShortName(string codeLanguage) => GetFullName(codeLanguage);

		public string GetFullName(string codeLanguage)
		{
			if (codeLanguage.StartsWith("C++")) return MetadataName.Replace(".", "::");
			else if (codeLanguage == "Rust") return MetadataName.Replace(".", "::").ToLower();
			else return MetadataName;
		}

		public string GetSyntax(string codeLanguage) => null;

		public bool IsLanguageSupported(string codeLanguage) => true;

		public bool IsPlatformSupported(string platformName) => true;

		public static TypeGroup GetInstance(string nameSpace, IEnumerable<IContentInfo> children) => Namespaces.GetInstance(nameSpace, () => new(nameSpace, children));
	}
}