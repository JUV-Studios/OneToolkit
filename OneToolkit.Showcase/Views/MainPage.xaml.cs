using System;
using Windows.System;
using Windows.ApplicationModel;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using OneToolkit.UI;
using OneToolkit.System;
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
			ThemeService_AppThemeChanged(ThemeService.SystemAppsTheme);
			ThemeService.AppThemeChanged += ThemeService_AppThemeChanged;
			Microsoft.UI.Xaml.Controls.BackdropMaterial.SetApplyToRootOrPageBackground(this, true);
		}

		private void ShowAppMenu() => AppMenu.ShowAt(AppIconButton, new(4, AppIconButton.ActualHeight + 4));

		private void NavigationManager_BackRequested(object sender, BackRequestedEventArgs e)
		{
			if (Presenter.CanGoBack)
			{
				e.Handled = true;
				Presenter.GoBack();
			}
		}

		private void ThemeService_AppThemeChanged(ApplicationTheme newTheme)
		{
			var titleBar = SettingsViewModel.ViewServiceProvider.AppView.TitleBar;
			titleBar.ButtonHoverBackgroundColor = App.Current.Resources["SystemBaseLowColor"] as Color?;
			titleBar.ButtonHoverForegroundColor = App.Current.Resources["SystemBaseHighColor"] as Color?;
		}

		private void Page_Loaded(object sender, RoutedEventArgs e)
		{
			Window.Current.SetTitleBar(DragRegion);
			Presenter.Navigate(typeof(HomePage));
		}

		private void Presenter_Navigated(object sender, NavigationEventArgs e)
		{
			AppMenuHomeItem.IsEnabled = Presenter.CurrentSourcePageType != typeof(HomePage);
			SettingsViewModel.ViewServiceProvider.NavigationManager.AppViewBackButtonVisibility = Presenter.CanGoBack ? AppViewBackButtonVisibility.Visible : AppViewBackButtonVisibility.Collapsed;
		}

		private void HomeItem_Click(object sender, RoutedEventArgs e)
		{
			Presenter.Navigate(typeof(HomePage), null, App.PageTransition);
			SettingsViewModel.ViewServiceProvider.NavigationManager.AppViewBackButtonVisibility = Presenter.CanGoBack ? AppViewBackButtonVisibility.Visible : AppViewBackButtonVisibility.Collapsed;
			Presenter.BackStack.Clear();
			Presenter.ForwardStack.Clear();
		}

		private async void HelpItem_Click(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(new("http://discord.com/invite/CZpBpPQjq8"));

		private async void FeedbackItem_Click(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(new($"https://www.nuget.org/packages/OneToolkit/{PackageVersionHelper.Stringify(Package.Current.Id.Version)}/ContactOwners"));
	}
}