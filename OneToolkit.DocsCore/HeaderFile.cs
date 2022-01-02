using System;
using System.Reflection;
using System.Collections.Generic;

namespace OneToolkit.DocsCore
{
	public sealed class HeaderFile : IContentInfo
	{
		private HeaderFile() { }

		private string Name;

		private static readonly Dictionary<string, HeaderFile> Headers = new();

		public Assembly Metadata => null;

		public HeaderFile Header => this;

		public WeakReference<IContentInfo> Parent => new(this);

		public IEnumerable<IContentInfo> Children => null;

		public IEnumerable<string> SupportedLanguages { get; init; }

		public string GetName(string codeLanguage) => Name;

		public string GetSyntax(string codeLanguage) => null;

		public static HeaderFile GetInstance(string fileName, IEnumerable<string> supportedLanguages) => Headers.TryGetValue(fileName, out var result) ? result : new HeaderFile()
		{
			Name = fileName,
			SupportedLanguages = supportedLanguages
		};
	}
}