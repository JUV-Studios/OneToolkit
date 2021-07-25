using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using Windows.Storage;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Controls;
using Windows.ApplicationModel;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Mvvm;
using OneToolkit.Lifecycle;
using OneToolkit.Showcase.Models;
using OneToolkit.Showcase.ViewModels;
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
			typeof(IAsyncClosable).Assembly, typeof(HubPanel).Assembly, typeof(ObservableBase).Assembly
		};

		public static readonly string[] ExcludedNamespaces = new[]
		{
			"OneToolkit.UI.Xaml.OneToolkit_UI_Xaml_XamlTypeInfo", "System.Runtime.CompilerServices", "Microsoft.CodeAnalysis"
		};

		public static IEnumerable<TypeGroup> FoundTypes
		{
			get
			{
				return from assembly in ToolkitAssemblies
					   from type in assembly.GetTypes()
					   where !ExcludedNamespaces.Contains(type.Namespace) && !type.Name.StartsWith("__") && !type.Name.EndsWith("Statics") && !type.Name.EndsWith("Factory") && type.IsPublic
					   group type by type.Namespace into types
					   select new TypeGroup(types.Key, types);
			}
		}

		private void HubPanel_Loaded(object sender, RoutedEventArgs e)
		{
			var target = sender as HubPanel;
			if (target.Header == null) target.Header = (MainPage.NavigationMenu.SelectedItem as ComboBoxItem).Content;
		}

		private /* async */ void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			/* if (SettingsViewModel.Instance.SelectedProgrammingLanguage.StartsWith("C++"))
			{
				if (MacrosList.ItemsSource == null)
				{
					MacrosList.ItemsSource = await PathIO.ReadLinesAsync(Path.Combine(Package.Current.InstalledLocation.Path, "Assets", "Reference", "Macros.txt"));
					MacrosList.SelectedIndex = 0;
				}

				ReferenceMacros.Visibility = Visibility.Visible;
			}
			else
			{
				ReferenceMacros.Visibility = Visibility.Collapsed;
			} */
		}

		private void UserControl_Unloaded(object sender, RoutedEventArgs e)
		{
			if (!MainPage.Frame.CanGoBack)
			{
				VisualTreeHelper.DisconnectChildrenRecursive(this);
				Bindings.StopTracking();
				Bindings = null;
				App.PageTypeCache.Remove(typeof(ApiReference));
#if DEBUG
				GC.Collect();
				GC.WaitForPendingFinalizers();
#endif
			}
		}

		private void GridView_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			var item = e.AddedItems[0] as TypeGroup;
			DocsHub.Header = item.Name;
			DocsFrame.Navigate(typeof(NamespacePage), item, new DrillInNavigationTransitionInfo());
		}
	}
}