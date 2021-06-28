using System;
using System.ComponentModel;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.ViewManagement;
using Windows.ApplicationModel;
using Microsoft.Toolkit.Uwp.Helpers;
using OneToolkit.Showcase.ViewModels;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace OneToolkit.Showcase.Views
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainPage : UserControl
	{
		public MainPage()
		{
			InitializeComponent();
			SettingsViewModel.ViewServiceProvider.SetMinimumSize(new(500, 500));
			SettingsViewModel.ViewServiceProvider.ExtendsContentIntoTitleBar = true;
			var titleBar = ApplicationView.GetForCurrentView().TitleBar;
			titleBar.BackgroundColor = Colors.Transparent;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
		}

		public Uri FeedbackUri => new($"https://www.nuget.org/packages/OneToolkit/{Package.Current.Id.Version.ToFormattedString()}/ContactOwners");

		private void Page_Loaded(object sender, RoutedEventArgs e)
		{
			NavView.SelectedItem ??= NavView.MenuItems[0];
		}
	}
}
