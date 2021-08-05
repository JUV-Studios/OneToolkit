using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Animation;
using OneToolkit.Mvvm;
using OneToolkit.Lifecycle;
using OneToolkit.Showcase.Models;
using OneToolkit.UI.Xaml.Controls;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class ApiReference : UserControl
	{
		private IContentInfo CurrentInfoItem;

		public ApiReference() => InitializeComponent();

		public static readonly Assembly[] ToolkitAssemblies = new[]
		{
			typeof(IAsyncClosable).Assembly, typeof(HubPanel).Assembly, typeof(ObservableBase).Assembly
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


		private void TitleBlock_Loaded(object sender, RoutedEventArgs e)
		{
			var target = sender as TextBlock;
			target.Text = (MainPage.NavigationMenu.SelectedItem as ComboBoxItem).Content.ToString();
			target.Loaded -= TitleBlock_Loaded;
		}

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
		}

		private void UserControl_Unloaded(object sender, RoutedEventArgs e)
		{
			if (!MainPage.ContentFrame.CanGoBack)
			{
				MemoryManager.Delete(ref Bindings, _ => Bindings.StopTracking());
#if DEBUG
				GC.Collect();
				GC.WaitForPendingFinalizers();
#endif
			}
		}

		private void Namespaces_ItemInvoked(Microsoft.UI.Xaml.Controls.TreeView sender, Microsoft.UI.Xaml.Controls.TreeViewItemInvokedEventArgs args)
		{
			args.Handled = Navigate(args.InvokedItem as IContentInfo);
		}
		
		/* private List<Microsoft.UI.Xaml.Controls.TreeViewNode> GetAllNodes()
		{
			List<Microsoft.UI.Xaml.Controls.TreeViewNode> result = new();
			foreach (var node in Namespaces.RootNodes) GetNodes(node, result);
			return result;
		}

		private void GetNodes(Microsoft.UI.Xaml.Controls.TreeViewNode currentNode, List<Microsoft.UI.Xaml.Controls.TreeViewNode> nodes)
		{
			if (currentNode == null) return;
			nodes.Add(currentNode);
			foreach (var childNode in currentNode.Children) GetNodes(childNode, nodes);
		} */

		public bool Navigate(IContentInfo contentInfo)
		{
			if (!(CurrentInfoItem?.Equals(contentInfo)).GetValueOrDefault(false))
			{
				DocsFrame.Navigate(contentInfo.PageType, contentInfo, new DrillInNavigationTransitionInfo());
				CurrentInfoItem = contentInfo;
				Namespaces.SelectedNode = TreeViewHelper.GetAllNodes(Namespaces).First(node => node.Content.Equals(contentInfo));
				if (Namespaces.SelectedNode.Parent != null) Namespaces.SelectedNode.Parent.IsExpanded = true;
				return true;
			}

			return false;
		}
	}
}