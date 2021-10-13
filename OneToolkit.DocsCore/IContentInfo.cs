using System;
using System.Collections.Generic;

namespace OneToolkit.DocsCore
{
	public interface IContentInfo
	{
		string Name { get; }

		WeakReference<IContentInfo> Parent { get; }

		IEnumerable<IContentInfo> Children { get; }
	}

	public interface IContentInfoPage
	{
		IContentInfo ContentInfo { get; }
	}
}