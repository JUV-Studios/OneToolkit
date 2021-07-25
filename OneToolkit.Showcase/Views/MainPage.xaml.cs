using System;
using System.Linq;
using System.ComponentModel;
using Windows.System;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using OneToolkit.UI.Input;
using OneToolkit.UI.Xaml.Controls;
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
			SettingsViewModel.ViewServiceProvider.AppView.SetPreferredMinSize(new(500, 500));
			SettingsViewModel.ViewServiceProvider.CoreAppView.TitleBar.ExtendViewIntoTitleBar = true;
			var titleBar = SettingsViewModel.ViewServiceProvider.AppView.TitleBar;
			titleBar.BackgroundColor = Colors.Transparent;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
			SettingsViewModel.Instance.Initialize();
		}

		public static ContentFrame Frame => (Window.Current.Content as MainPage).Presenter;

		public static ComboBox NavigationMenu => (Window.Current.Content as MainPage).NavigationBox;

		private void UserControl_Loaded(object sender, RoutedEventArgs e)
		{
			NavigationBox.SelectedIndex = 0;
			Presenter.PropertyChanged += Presenter_PropertyChanged;
			SettingsViewModel.ViewServiceProvider.CoreAppView.CoreWindow.Dispatcher.AcceleratorKeyActivated += Dispatcher_AcceleratorKeyActivated;
			Window.Current.SetTitleBar(TitleBar);
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
						NavigationBox.SelectedIndex = TabSwitcher.GetNewSelectionIndex(Convert.ToUInt32(NavigationBox.SelectedIndex), Convert.ToUInt32(NavigationBox.Items.Count), KeyStateHelper.IsKeyDown(VirtualKey.Shift));
					}
				}
			}
		}

		private void Presenter_PropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			if (e.PropertyName == "CanGoBack")
			{
				SettingsViewModel.NavigationManager.AppViewBackButtonVisibility = Presenter.CanGoBack ? AppViewBackButtonVisibility.Visible : AppViewBackButtonVisibility.Collapsed;
			}
		}

		private void NavigationBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			var type = Type.GetType((e.AddedItems[0] as FrameworkElement).Tag.ToString());
			if (Presenter.Content.GetType() != type) Presenter.Navigate(App.PageTypeCache.GetInstance(type, (typeId) => Activator.CreateInstance(type)) as UIElement);
		}

		private void Presenter_ContentChanged(ContentFrame sender, ContentFrameContentChangedEventArgs args)
		{
			NavigationBox.SelectedItem = (from item in NavigationBox.Items
										 where (item as FrameworkElement).Tag.ToString() == args.NewContent.GetType().FullName
										 select item).First();
		}

		private void BackAccelerator_Invoked(Windows.UI.Xaml.Input.KeyboardAccelerator sender, Windows.UI.Xaml.Input.KeyboardAcceleratorInvokedEventArgs args)
		{
			args.Handled = true;
			Presenter.GoBack();
		}

		private void BackClearAccelerator_Invoked(Windows.UI.Xaml.Input.KeyboardAccelerator sender, Windows.UI.Xaml.Input.KeyboardAcceleratorInvokedEventArgs args)
		{
			args.Handled = true;
			NavigationBox.SelectedIndex = 0;
			Presenter.ClearBackStack();
		}
	}
}
