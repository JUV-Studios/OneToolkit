using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using OneToolkit.Lifecycle;

namespace OneToolkit.DocsCore
{
	public sealed class HeaderFile : IContentInfo
	{
		private HeaderFile(string fileName, IEnumerable<string> supportedLanguages, IEnumerable<string> supportedPlatforms)
		{
			MetadataName = fileName;
			SupportedLanguages = supportedLanguages;
			SupportedPlatforms = supportedPlatforms;
		}

		private static readonly WeakValueCache<string, HeaderFile> Headers = new();

		private IEnumerable<string> SupportedLanguages;

		private IEnumerable<string> SupportedPlatforms;

		public string MetadataName { get; init; }

		public string Description => null;

		public Assembly Metadata => null;

		public HeaderFile Header => this;

		public ContentInfoKind Kind => ContentInfoKind.Header;

		public AssemblyContentType ContentType => AssemblyContentType.WindowsRuntime;

		public WeakReference<IContentInfo> Parent => new(this);

		public IEnumerable<IContentInfo> Children => null;

		public IEnumerable<IGrouping<ContentInfoKind, IContentInfo>> GroupedChildren => null;

		public string GetShortName(string codeLanguage) => MetadataName;

		public string GetFullName(string codeLanguage) => MetadataName;

		public string GetSyntax(string codeLanguage) => $"#include {MetadataName}";

		public bool IsLanguageSupported(string codeLanguage) => SupportedLanguages.Contains(codeLanguage);

		public bool IsPlatformSupported(string platformName) => SupportedPlatforms.Contains(platformName);

		public static HeaderFile GetInstance(string fileName, IEnumerable<string> supportedLanguages, IEnumerable<string> supportedPlatforms) => Headers.GetInstance(fileName, () =>
		{
			return new(fileName, supportedLanguages, supportedPlatforms);
		});

		public static HeaderFile GetInstance(string encodedFilePath)
		{
			var content = File.ReadAllLines(encodedFilePath);
			return GetInstance(content[0], content[2].Split(',').Select(text => text.Trim()), content[3].Split(',').Select(text => text.Trim()));
		}

		internal static HeaderFile GetForRtNamespace(string nameSpace) => GetInstance($"winrt/{nameSpace}.h", new[] { "C++/WinRT" }, new[] { "Windows" });
	}
}