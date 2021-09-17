using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.ApplicationModel;
using Microsoft.Toolkit.Uwp;
using Microsoft.Toolkit.Uwp.UI.Controls;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.ApplicationModel;
using OneToolkit.Lifecycle;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class SettingsPage : UserControl
	{
		public SettingsPage() => InitializeComponent();

		public static string AboutAutomationText => $"{"SettingsHubAbout.Header".GetLocalized()}, {AboutDisplayText.Replace("\n", ", ")}";

		public static readonly string AboutDisplayText = $"OneToolkit Showcase\n{string.Format("VersionText".GetLocalized(), PackageVersionHelper.ToFormattedString(Package.Current.Id.Version))}\n"
			+ "CopyrightText".GetLocalized();

		private void HubPanel_Loaded(object sender, RoutedEventArgs e)
		{
			var target = sender as HubPanel;
			target.Header = (MainPage.NavigationMenu.SelectedItem as ComboBoxItem).Content;
			target.Loaded -= HubPanel_Loaded;
		}

		private void AboutPanel_Loaded(object sender, RoutedEventArgs e)
		{
		 	var target = sender as WrapPanel;
			/*var actualWidth = target.ActualWidth;
			LinksExpander.Width = actualWidth;
			LinksExpander.MaxWidth = actualWidth;*/
			target.Loaded -= AboutPanel_Loaded;
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
	}
}
