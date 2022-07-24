using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.ComponentModel;
using System.Collections.Generic;
using Windows.ApplicationModel;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using CommunityToolkit.Mvvm.ComponentModel;
using ColorCode;
using OneToolkit.DocsCore;
using OneToolkit.Lifecycle;
using OneToolkit.UI;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;
using OneToolkit.System;
using OneToolkit.UI.Xaml;
using OneToolkit.Data;

namespace OneToolkit.Showcase.Views
{
	[INotifyPropertyChanged]
	public sealed partial class ApiReference : Page, IContentInfoPresenter
	{
		public ApiReference() => InitializeComponent();

		public static readonly Assembly[] ToolkitAssemblies = new[]
		{
			typeof(SystemInformation).Assembly, typeof(TreeViewHelper).Assembly, typeof(Endianness).Assembly
		};

		public static readonly IEnumerable<TypeGroup> FoundNamespaces = from assembly in ToolkitAssemblies
																		from type in assembly.ExportedTypes
																		select DocsCore.TypeInfo.GetInstance(type) into type
																		group type by type.TypeDetails.Namespace into nameSpace
																		where nameSpace.Key != "OneToolkit.UI.Xaml.OneToolkit_UI_Xaml_Uwp_XamlTypeInfo"
																		orderby nameSpace.Key
																		select TypeGroup.GetInstance(nameSpace.Key, nameSpace);

		public static IEnumerable<HeaderFile> FoundHeaders = (from file in Directory.EnumerateFiles(Path.Combine(Package.Current.InstalledLocation.Path, @"Assets\Reference\Headers"))
															  select HeaderFile.GetInstance(file)).Concat(from nameSpace in FoundNamespaces
															  where nameSpace.ContentType == AssemblyContentType.WindowsRuntime
															  select nameSpace.Header);

		public static IEnumerable<DocsCore.TypeInfo> XamlControls => from nameSpace in FoundNamespaces
																	 where nameSpace.GetShortName(string.Empty) == "OneToolkit.UI.Xaml.Controls"
																	 select nameSpace.Children as IEnumerable<DocsCore.TypeInfo> into types
																	 from type in types
																	 where type.TypeDetails.IsSubclassOf(typeof(UIElement))
																	 orderby type.GetShortName(string.Empty)
																	 select type;


		public static IEnumerable<DocsCore.TypeInfo> XamlConverters => from nameSpace in FoundNamespaces
																	   where nameSpace.GetShortName(string.Empty) == "OneToolkit.UI.Xaml.Converters"
																	   select nameSpace.Children as IEnumerable<DocsCore.TypeInfo> into types
																	   from type in types
																	   where type.TypeDetails.IsClass
																	   orderby type.GetShortName(string.Empty)
																	   select type;

		public IContentInfo ContentInfo { get; private set; }

		public string PageTitle
		{
			get => GetValue(PageTitleProperty).ToString();
			set => SetValue(PageTitleProperty, value);
		}

		public static readonly DependencyProperty PageTitleProperty = DependencyProperty.Register(nameof(PageTitle), typeof(string), typeof(ApiReference), new(string.Empty));

		public static string GetGroupName(ContentInfoKind kind) => SettingsViewModel.Resources.GetString(kind switch
		{
			ContentInfoKind.Class => "ClassesText",
			ContentInfoKind.Delegate => "DelegatesText",
			ContentInfoKind.Enumeration => "EnumerationsText",
			ContentInfoKind.Interface => "InterfacesText",
			ContentInfoKind.Structure => "StructuresText",
			_ => string.Empty
		});

		public static string GetDisplaySuffix(ContentInfoKind kind) => SettingsViewModel.Resources.GetString(kind switch
		{
			ContentInfoKind.Header => "HeaderText",
			ContentInfoKind.Namespace => "NamespaceText",
			ContentInfoKind.Class => "ClassText",
			ContentInfoKind.Delegate => "DelegateText",
			ContentInfoKind.Enumeration => "EnumerationText",
			ContentInfoKind.Interface => "InterfaceText",
			ContentInfoKind.Structure => "StructureText",
			_ => string.Empty
		});

		public static ILanguage GetColorCodeLanguage(string codeLanguage)
		{
			return codeLanguage switch
			{
				"C#" => Languages.CSharp,
				"Visual Basuc" => Languages.VbDotNet,
				_ => codeLanguage.StartsWith("C++") ? Languages.Cpp : null
			};
		}

		public static Uri GetWebLink(IContentInfo contentInfo)
		{
			throw new NotImplementedException();
		}

		public static Uri GetLocalLink(IContentInfo contentInfo) => new($"onetoolkit:///{contentInfo}");

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			ContentInfo = e.Parameter as IContentInfo;
			SettingsViewModel.Instance.PropertyChanged += Settings_PropertyChanged;
		}

		protected override void OnNavigatedFrom(NavigationEventArgs e)
		{
			base.OnNavigatedFrom(e);
			(ContentInfo as IDisposable)?.Dispose();
			SettingsViewModel.Instance.PropertyChanged -= Settings_PropertyChanged;
		}

		private void SetTitle()
		{
			PageTitle = $"{ContentInfo.GetShortName(SettingsViewModel.Instance.SelectedCodeLanguage)} {GetDisplaySuffix(ContentInfo.Kind)}";
		}

		private void Settings_PropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			if (e.PropertyName == nameof(SettingsViewModel.Instance.SelectedCodeLanguage)) SetTitle();
		}
	}
}