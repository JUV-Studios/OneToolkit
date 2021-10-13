using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using OneToolkit.DocsCore;
using OneToolkit.Lifecycle;
using OneToolkit.UI;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class ApiReference : Page, IContentInfoPage
	{
		public ApiReference() => InitializeComponent();

		public static readonly Assembly[] ToolkitAssemblies = new[]
		{
			typeof(IAsyncClosable).Assembly, typeof(HubPanel).Assembly, typeof(ColorUtility).Assembly
		};

		public static readonly string[] ExcludedNamespaces = new[]
		{
			"OneToolkit.UI.Xaml.OneToolkit_UI_Xaml_XamlTypeInfo"
		};

		public static readonly IEnumerable<IContentInfo> FoundNamespaces = from assembly in ToolkitAssemblies
																		   from type in assembly.GetTypes()
																		   where !ExcludedNamespaces.Contains(type.Namespace) && type.IsPublic
																		   group type by type.Namespace into types
																		   orderby types.Key
																		   select new TypeGroup(types.Key, types);

		public IContentInfo ContentInfo { get; private set; }

		public static string GetDisplayName(IContentInfo contentInfo)
		{
			if (contentInfo is TypeGroup typeGroup) return $"{typeGroup.Name} {SettingsViewModel.Resources.GetString("NamespaceText")}";
			else return contentInfo.Name;
		}

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			ContentInfo = e.Parameter as IContentInfo;
		}
	}
}