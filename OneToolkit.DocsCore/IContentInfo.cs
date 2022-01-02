using System;
using System.Reflection;
using System.Collections.Generic;

namespace OneToolkit.DocsCore
{
	public interface IContentInfo
	{
		Assembly Metadata { get; }

		HeaderFile Header { get; }

		WeakReference<IContentInfo> Parent { get; }

		IEnumerable<IContentInfo> Children { get; }

		IEnumerable<string> SupportedLanguages { get; }

		string GetName(string codeLanguage);

		string GetSyntax(string codeLanguage);
	}

	public interface IContentInfoPresenter
	{
		IContentInfo ContentInfo { get; }
	}
}