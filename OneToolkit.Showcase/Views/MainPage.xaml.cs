using System;
using Windows.System;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Windows.ApplicationModel;
using OneToolkit.System;
using OneToolkit.Showcase.Dialogs;
using OneToolkit.Showcase.ViewModels;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class MainPage : Page
	{
		public MainPage()
		{
			InitializeComponent();
			SettingsViewModel.ViewServiceProvider.AppView.SetPreferredMinSize(new(500, 500));
			SettingsViewModel.ViewServiceProvider.CoreAppView.TitleBar.ExtendViewIntoTitleBar = true;
			SettingsViewModel.ViewServiceProvider.NavigationManager.BackRequested += NavigationManager_BackRequested;
			var titleBar = SettingsViewModel.ViewServiceProvider.AppView.TitleBar;
			titleBar.BackgroundColor = Colors.Transparent;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
			Microsoft.UI.Xaml.Controls.BackdropMaterial.SetApplyToRootOrPageBackground(this, true);
		}

		private void Page_Loaded(object sender, RoutedEventArgs e)
		{
			Window.Current.SetTitleBar(DragRegion);
			Presenter.Navigate(typeof(HomePage));
		}

		private void NavigationManager_BackRequested(object sender, BackRequestedEventArgs e)
		{
			if (Presenter.CanGoBack)
			{
				e.Handled = true;
				Presenter.GoBack();
			}
		}

		private void Presenter_Navigated(object sender, NavigationEventArgs e)
		{
			SettingsViewModel.ViewServiceProvider.NavigationManager.AppViewBackButtonVisibility = Presenter.CanGoBack ? AppViewBackButtonVisibility.Visible : AppViewBackButtonVisibility.Collapsed;
		}

		private async void HelpItem_Click(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(new("http://discord.com/invite/CZpBpPQjq8"));

		private async void FeedbackItem_Click(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(new($"https://www.nuget.org/packages/OneToolkit/{PackageVersionHelper.Stringify(Package.Current.Id.Version)}/ContactOwners"));

		private async void AboutItem_Click(object sender, RoutedEventArgs e) => await new AboutDialog().ShowAsync();
	}
}