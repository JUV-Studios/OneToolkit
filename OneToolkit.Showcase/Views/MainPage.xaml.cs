using System;
using System.Linq;
using System.ComponentModel;
using Windows.System;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Controls;
using OneToolkit.UI.Input;
using OneToolkit.UI.Xaml.Controls;
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
			var titleBar = SettingsViewModel.ViewServiceProvider.AppView.TitleBar;
			titleBar.BackgroundColor = Colors.Transparent;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
			SettingsViewModel.Instance.Initialize();
			Microsoft.UI.Xaml.Controls.BackdropMaterial.SetApplyToRootOrPageBackground(this, true);
		}

		public static ContentFrame ContentFrame => (Window.Current.Content as MainPage).Presenter;

		public static ComboBox NavigationMenu => (Window.Current.Content as MainPage).NavigationBox;

		private void Page_Loaded(object sender, RoutedEventArgs e)
		{
			SetTitleBarMargin();
			Window.Current.SetTitleBar(DragRegion);
			NavigationBox.SelectedIndex = 0;
			Presenter.PropertyChanged += HandlePropertyChanged;
			SettingsViewModel.Instance.PropertyChanged += HandlePropertyChanged;
			SettingsViewModel.ViewServiceProvider.CoreAppView.CoreWindow.Dispatcher.AcceleratorKeyActivated += Dispatcher_AcceleratorKeyActivated;
		}

		private void Dispatcher_AcceleratorKeyActivated(CoreDispatcher sender, AcceleratorKeyEventArgs args)
		{
			if (!SettingsViewModel.ViewServiceProvider.IsDialogShown && args.EventType == CoreAcceleratorKeyEventType.KeyDown)
			{
				if (KeyStateHelper.IsKeyDown(VirtualKey.Control))
				{
					if (args.VirtualKey == VirtualKey.Tab)
					{
						args.Handled = true;
						NavigationBox.SelectedIndex = Convert.ToInt32(TabSwitcher.GetNewSelectionIndex(Convert.ToUInt32(NavigationBox.SelectedIndex), Convert.ToUInt32(NavigationBox.Items.Count),
							KeyStateHelper.IsKeyDown(VirtualKey.Shift)));
					}
				}
			}
		}

		private void NavigationBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			var type = Type.GetType((e.AddedItems[0] as FrameworkElement).Tag.ToString());
			if (Presenter.Content?.GetType() != type) Presenter.Navigate(App.PageTypeCache.GetInstance(type, (typeId) => Activator.CreateInstance(type)) as UIElement);
		}

		private void Presenter_ContentChanged(ContentFrame sender, ContentFrameContentChangedEventArgs args)
		{
			NavigationBox.SelectedItem = (from item in NavigationBox.Items
										 where (item as FrameworkElement).Tag.ToString() == args.NewContent.GetType().FullName
										 select item).First();
		}

		private void BackAccelerator_Invoked(KeyboardAccelerator sender, KeyboardAcceleratorInvokedEventArgs args)
		{
			args.Handled = true;
			Presenter.GoBack();
		}

		private void BackClearAccelerator_Invoked(KeyboardAccelerator sender, KeyboardAcceleratorInvokedEventArgs args)
		{
			args.Handled = true;
			NavigationBox.SelectedIndex = 0;
			Presenter.ClearBackStack();
		}

		private void HandlePropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			if (e.PropertyName == "CanGoBack")
			{
				SettingsViewModel.NavigationManager.AppViewBackButtonVisibility = Presenter.CanGoBack ? AppViewBackButtonVisibility.Visible : AppViewBackButtonVisibility.Collapsed;
			}
			else if (e.PropertyName == "TitleBarInset")
			{
				SetTitleBarMargin();
			}
		}

		private void SetTitleBarMargin()
		{
			var leftInset = SettingsViewModel.ViewServiceProvider.CoreAppView.TitleBar.SystemOverlayLeftInset;
			(DragRegion.Children[0] as FrameworkElement).Margin = leftInset == 0 ? new(12, 0, 12, 0) : new(8, 0, 8, 0);
		}
	}
}
