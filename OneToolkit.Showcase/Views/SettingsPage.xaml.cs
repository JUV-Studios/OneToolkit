using System;
using System.Linq;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Controls;
using Windows.ApplicationModel;
using Microsoft.Toolkit.Uwp;
using OneToolkit.ApplicationModel;
using OneToolkit.UI.Xaml.Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace OneToolkit.Showcase.Views
{
	public sealed partial class SettingsPage : UserControl
	{
		public SettingsPage() => InitializeComponent();

		public static string AboutAutomationText => $"{"SettingsHubAbout.Header".GetLocalized()}, {AboutDisplayText.Replace("\n", ", ")}";

		public static readonly string AboutDisplayText = $"OneToolkit Showcase\n{string.Format("VersionText".GetLocalized(), PackageVersionHelper.GetFormattedString(Package.Current.Id.Version))}\n"
			+ "CopyrightText".GetLocalized();

		private void HubPanel_Loaded(object sender, RoutedEventArgs e)
		{
			var target = sender as HubPanel;
			if (target.Header == null) target.Header = (MainPage.NavigationMenu.SelectedItem as ComboBoxItem).Content;
		}

		private async void Contribute_Click(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(new("https://dev.azure.com/JUV-Studios/OneToolkit"));

		private void ContributeExpander_Expanding(Microsoft.UI.Xaml.Controls.Expander sender, Microsoft.UI.Xaml.Controls.ExpanderExpandingEventArgs args)
		{
			sender.MaxWidth = sender.ActualWidth;
		}

		private void UserControl_Unloaded(object sender, RoutedEventArgs e)
		{
			if (!MainPage.Frame.CanGoBack)
			{
				ContributeExpander.Content = null;
				VisualTreeHelper.DisconnectChildrenRecursive(this);
				Bindings.StopTracking();
				Bindings = null;
				App.PageTypeCache.Remove(typeof(SettingsPage));
#if DEBUG
				GC.Collect();
				GC.WaitForPendingFinalizers();
#endif
			}
		}
	}
}
