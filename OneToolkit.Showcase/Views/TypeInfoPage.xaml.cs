using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class TypeInfoPage : Page
	{
		private Models.TypeInfo InfoData;

		public TypeInfoPage() => InitializeComponent();

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			InfoData = e.Parameter as Models.TypeInfo;
		}
	}
}