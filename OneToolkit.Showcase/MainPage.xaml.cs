using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.ViewManagement;
using OneToolkit.UI;
using System.ComponentModel;
using System.Diagnostics;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace OneToolkit.Showcase
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainPage : Page
	{
		private readonly IViewServiceProvider ViewServiceProvider = ViewService.GetForCurrentView();
		
		public MainPage()
		{
			InitializeComponent();
			ViewServiceProvider.ExtendViewIntoTitleBar = true;
			var titleBar = ApplicationView.GetForCurrentView().TitleBar;
			titleBar.BackgroundColor = Colors.Transparent;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
		}

		private void NavView_DisplayModeChanged(Microsoft.UI.Xaml.Controls.NavigationView sender, Microsoft.UI.Xaml.Controls.NavigationViewDisplayModeChangedEventArgs args)
		{
			NavView.IsDragRegionEnabled = NavView.PaneDisplayMode != Microsoft.UI.Xaml.Controls.NavigationViewPaneDisplayMode.Left;
		}

		private void Page_Loaded(object sender, RoutedEventArgs e)
		{
			NavView.PropertyChanging += NavView_PropertyChanging;
			NavView.PropertyChanged += NavView_PropertyChanged;
		}

		private void NavView_PropertyChanging(object sender, OneToolkit.Mvvm.PropertyChangingEventArgs args)
		{
			if (args.PropertyName == "CurrentContent")
			{
				if (NavView.CurrentContent != null) NavView.CurrentContent.Opacity = 0;
			}
		}

		private void NavView_PropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			if (e.PropertyName == "CurrentContent")
			{
				if (NavView.CurrentContent != null) NavView.CurrentContent.Opacity = 1;
			}
		}
	}
}
