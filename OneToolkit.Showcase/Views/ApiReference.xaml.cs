using System;
using System.Linq;
using System.Reflection;
using System.ComponentModel;
using System.Collections.Generic;
using Windows.UI.Xaml.Controls;
using OneToolkit.Mvvm;
using OneToolkit.Lifecycle;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Showcase.Models;
using MUXC = Microsoft.UI.Xaml.Controls;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media.Animation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace OneToolkit.Showcase.Views
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class ApiReference : UserControl
	{
		public ApiReference() => InitializeComponent();

		public static readonly Assembly[] ToolkitAssemblies = new[]
		{
			typeof(IAsyncClosable).Assembly, typeof(PageView).Assembly, typeof(ObservableBase).Assembly
		};

		public static readonly string[] ExcludedNamespaces = new[]
		{
			"OneToolkit.UI.Xaml.OneToolkit_UI_Xaml_XamlTypeInfo", "System.Runtime.CompilerServices", "Microsoft.CodeAnalysis"
		};

		public static IEnumerable<TypeGroup> FoundTypes;

		public static void Initialize()
		{
			FoundTypes ??= from assembly in ToolkitAssemblies
						   from type in assembly.GetTypes()
						   where !ExcludedNamespaces.Contains(type.Namespace) && !type.Name.StartsWith("__") && !type.Name.EndsWith("Statics") && !type.Name.EndsWith("Factory")
						   group type by type.Namespace into types
						   select new TypeGroup(types.Key, types);
		}

		private void PaneView_ModeChanged(MUXC.TwoPaneView sender, object args)
		{
			if (sender.Mode == MUXC.TwoPaneViewMode.Tall)
			{
				GridLength starLength = new(1, GridUnitType.Star);
				sender.Pane1Length = starLength;
				sender.Pane2Length = starLength;
			}
		}

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			Namespaces.SelectedNode ??= Namespaces.RootNodes[0];
			if (DocsFrame.Content == null) NavigateToDocs(Namespaces.ItemFromContainer(Namespaces.ContainerFromNode(Namespaces.SelectedNode)) as TypeGroup);
		}

		private void Namespaces_ItemInvoked(MUXC.TreeView sender, MUXC.TreeViewItemInvokedEventArgs args)
		{
			if (args.InvokedItem is TypeGroup typeGroup)
			{
				args.Handled = true;
				NavigateToDocs(typeGroup);
			}
		}

		private void NavigateToDocs(TypeGroup typeGroup)
		{
			if (!typeGroup.IsType)
			{
				if (DocsFrame.Content is NamespacePage namespacePage)
				{
					if (namespacePage.GroupData == typeGroup) return;
				}

				DocsFrame.Navigate(typeof(NamespacePage), typeGroup, new DrillInNavigationTransitionInfo());
			}
		}
	}
}