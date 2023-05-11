using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Windows.ApplicationModel.Search;
using Microsoft.UI.Xaml.Controls;
using OneToolkit.UI;
using OneToolkit.Showcase.ViewModels;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class MainPage : Page
	{
		public MainPage()
		{
			InitializeComponent();
			Presenter.Navigate(typeof(HomePage));
			Window.Current.SetTitleBar(DragRegion);
			ThemeService.AppThemeChanged += ThemeService_AppThemeChanged;
			BackdropMaterial.SetApplyToRootOrPageBackground(this, true);
			SettingsViewModel.ViewServiceProvider.TrySetMinimumSize(new(500, 500));
			SettingsViewModel.ViewServiceProvider.CoreAppView.TitleBar.ExtendViewIntoTitleBar = true;
			SettingsViewModel.ViewServiceProvider.NavigationManager.BackRequested += NavigationManager_BackRequested;
#pragma warning disable CS0618
			if (SettingsViewModel.IsSearchCharmSupported)
			{
				SettingsViewModel.SearchCharm.SearchHistoryEnabled = false;
				SettingsViewModel.SearchCharm.PlaceholderText = SettingsViewModel.Resources.GetString("SearchCharmPlaceholderText");
				SettingsViewModel.SearchCharm.VisibilityChanged += SearchCharm_VisibilityChanged;
			}
#pragma warning restore CS0618
		}

		private void ShowAppMenu() => AppMenu.ShowAt(AppIconButton, new(4, SettingsViewModel.ViewServiceProvider.IsInFullScreen ? 4 : AppIconButton.ActualHeight + 4));

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
			Color baseLowColor, baseHighColor;
			if (newTheme == ApplicationTheme.Light)
			{
				baseLowColor = Color.FromArgb(51, 0, 0, 0);
				baseHighColor = Colors.Black;
			}
			else
			{
				baseLowColor = Color.FromArgb(51, 255, 255, 255);
				baseHighColor = Colors.White;
			}

			var titleBar = SettingsViewModel.ViewServiceProvider.AppView.TitleBar;
			titleBar.BackgroundColor = Colors.Transparent;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
			titleBar.ButtonForegroundColor = baseHighColor;
			titleBar.ButtonHoverBackgroundColor = baseLowColor;
			titleBar.ButtonHoverForegroundColor = baseHighColor;
		}

#pragma warning disable CS0618
		private void SearchCharm_VisibilityChanged(SearchPane sender, SearchPaneVisibilityChangedEventArgs args)
		{
			// if (!args.Visible) sender.TrySetQueryText(string.Empty);
		}
#pragma warning restore CS0618

		private void Page_SizeChanged(object sender, SizeChangedEventArgs e)
		{
			if (SettingsViewModel.ViewServiceProvider.IsInFullScreen)
			{
				FullScreenMenuItem.Icon = new SymbolIcon(Symbol.BackToWindow);
				FullScreenMenuItem.Text = SettingsViewModel.Resources.GetString("ExitFullScreenText");
			}
			else
			{
				FullScreenMenuItem.Icon = new SymbolIcon(Symbol.FullScreen);
				FullScreenMenuItem.Text = SettingsViewModel.Resources.GetString("FullScreenText");
			}
		}

		private void Presenter_Navigated(object sender, NavigationEventArgs e)
		{
			AppMenuHomeItem.IsEnabled = Presenter.CurrentSourcePageType != typeof(HomePage);
			SettingsViewModel.ViewServiceProvider.NavigationManager.AppViewBackButtonVisibility = Presenter.CanGoBack ? AppViewBackButtonVisibility.Visible : AppViewBackButtonVisibility.Collapsed;
		}

		private void HomeItem_Click(object sender, RoutedEventArgs e)
		{
			Presenter.Navigated -= Presenter_Navigated;
			Presenter.Navigate(typeof(HomePage), null, App.PageTransition);
			Presenter.BackStack.Clear();
			Presenter.ForwardStack.Clear();
			Presenter_Navigated(null, null);
			Presenter.Navigated += Presenter_Navigated;
		}
	}
}