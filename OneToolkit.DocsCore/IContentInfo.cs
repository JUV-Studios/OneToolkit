using System;
using System.Reflection;
using System.Collections.Generic;
using System.Linq;

namespace OneToolkit.DocsCore
{
	public enum ContentInfoKind : byte
	{
		Unknown, Assembly, Header, Namespace, Class, Delegate, Enumeration, Interface, Structure
	}

	/// <summary>
	/// Represents a top-level or child API reference item.
	/// </summary>
	public interface IContentInfo
	{
		string MetadataName { get; }

		string Description { get; }

		ContentInfoKind Kind { get; }

		AssemblyContentType ContentType { get; }

		Assembly Metadata { get; }

		HeaderFile Header { get; }

		WeakReference<IContentInfo> Parent { get; }

		IEnumerable<IContentInfo> Children { get; }

		IEnumerable<IGrouping<ContentInfoKind, IContentInfo>> GroupedChildren { get; }

		string GetShortName(string codeLanguage);
		
		string GetFullName(string codeLanguage);

		string GetSyntax(string codeLanguage);
		
		bool IsLanguageSupported(string codeLanguage);

		bool IsPlatformSupported(string platformName);
	}

	public interface IContentInfoPresenter
	{
		IContentInfo ContentInfo { get; }
	}
}