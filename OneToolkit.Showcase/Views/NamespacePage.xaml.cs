using OneToolkit.Showcase.Models;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace OneToolkit.Showcase.Views
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class NamespacePage : Page
	{
		public NamespacePage() => InitializeComponent();

		public TypeGroup GroupData;

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			GroupData = e.Parameter as TypeGroup;
		}

		protected override void OnNavigatedFrom(NavigationEventArgs e)
		{
			base.OnNavigatedFrom(e);
			/* Bindings.StopTracking();
			Bindings = null; */
		}
	}
}
