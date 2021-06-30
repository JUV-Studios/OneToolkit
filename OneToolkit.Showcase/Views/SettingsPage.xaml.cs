using System;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.ApplicationModel;
using Microsoft.Toolkit.Uwp;
using Microsoft.Toolkit.Uwp.Helpers;
using OneToolkit.UI.Xaml.Controls;
using Windows.UI.Xaml.Markup;
using Microsoft.Toolkit.Uwp.UI;
using Microsoft.Toolkit.Uwp.UI.Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace OneToolkit.Showcase.Views
{
	public sealed partial class SettingsPage : UserControl
	{
		public SettingsPage() => InitializeComponent();

		public string AboutAutomationText => $"{"SettingsHubAbout.Title".GetLocalized()}, {AboutDisplayText.Replace("\n", ", ")}";

		public readonly string AboutDisplayText = $"OneToolkit Showcase\n{string.Format("VersionText".GetLocalized(), Package.Current.Id.Version.ToFormattedString())}\n{"CopyrightText".GetLocalized()}";

		private void HubPanel_Loaded(object sender, RoutedEventArgs e)
		{
			var target = sender as HubPanel;
			if (string.IsNullOrEmpty(target.Title)) target.Title = (App.NavView.SettingsItem as ContentControl).Content.ToString();
		}

		private async void Contribute_Click(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(new("https://dev.azure.com/JUV-Studios/OneToolkit"));

		private void ContributeExpander_Expanding(Microsoft.UI.Xaml.Controls.Expander sender, Microsoft.UI.Xaml.Controls.ExpanderExpandingEventArgs args)
		{
			sender.MaxWidth = sender.ActualWidth;
		}
	}
}
