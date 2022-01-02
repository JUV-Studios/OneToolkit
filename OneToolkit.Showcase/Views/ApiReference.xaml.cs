using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using OneToolkit.DocsCore;
using OneToolkit.Lifecycle;
using OneToolkit.UI;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class ApiReference : Page, IContentInfoPresenter
	{
		public ApiReference() => InitializeComponent();

		public static readonly Assembly[] ToolkitAssemblies = new[]
		{
			typeof(IAsyncClosable).Assembly, typeof(HubPanel).Assembly, typeof(ColorUtility).Assembly
		};

		public static readonly IEnumerable<TypeGroup> FoundNamespaces = from assembly in ToolkitAssemblies
																		from type in assembly.GetTypes()
																		where type.IsPublic
																		group type by type.Namespace into types
																		orderby types.Key
																		select new TypeGroup(types.Key, types);

		public static IEnumerable<DocsCore.TypeInfo> XamlControls => from nameSpace in FoundNamespaces
																	 where nameSpace.GetName(null) == "OneToolkit.UI.Xaml.Controls"
																	 select nameSpace.Children as IEnumerable<DocsCore.TypeInfo> into types
																	 from type in types
																	 where type.Type.IsSubclassOf(typeof(UIElement))
																	 select type;


		public static IEnumerable<DocsCore.TypeInfo> XamlConverters => from nameSpace in FoundNamespaces
																	   where nameSpace.GetName(null) == "OneToolkit.UI.Xaml.Converters"
																	   select nameSpace.Children as IEnumerable<DocsCore.TypeInfo> into types
																	   from type in types
																	   select type;

		public IContentInfo ContentInfo { get; private set; }

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			ContentInfo = e.Parameter as IContentInfo;
		}

		protected override void OnNavigatedFrom(NavigationEventArgs e)
		{
			base.OnNavigatedFrom(e);
			(ContentInfo as IDisposable)?.Dispose();
		}

		public static string GetGroupName(TypeKind kind) => SettingsViewModel.Resources.GetString(kind switch
		{
			TypeKind.Class => "ClassesText",
			TypeKind.Delegate => "DelegatesText",
			TypeKind.Enumeration => "EnumerationsText",
			TypeKind.Interface => "InterfacesText",
			TypeKind.Structure => "StructuresText",
			_ => string.Empty
		});

		public static string GetDisplayName(IContentInfo contentInfo)
		{
			if (contentInfo is TypeGroup typeGroup) return $"{typeGroup.GetName(SettingsViewModel.Instance.SelectedCodeLanguage)} {SettingsViewModel.Resources.GetString("NamespaceText")}";
			else return contentInfo.GetName(null);
		}

		private void Page_Loaded(object sender, RoutedEventArgs e)
		{
			if (ContentInfo is TypeGroup typeGroup)
			{
				foreach (var group in typeGroup.GroupedChildren)
				{
					HubPanelSection section = new()
					{
						Header = GetGroupName(group.Key)
					};

					(Content as HubPanel).Sections.Add(section);
				}
			}
		}
	}
}