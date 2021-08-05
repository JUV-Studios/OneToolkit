using System;
using System.Linq;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using OneToolkit.Showcase.Models;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Lifecycle;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class NamespacePage : Page
	{
		private TypeGroup GroupData;

		public NamespacePage() => InitializeComponent();

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			GroupData = e.Parameter as TypeGroup;
		}

		protected override void OnNavigatedFrom(NavigationEventArgs e)
		{
			base.OnNavigatedFrom(e);
			MemoryManager.Delete(ref Bindings, _ => Bindings.StopTracking());
#if DEBUG
			GC.Collect();
			GC.WaitForPendingFinalizers();
#endif
		}

		private void Classes_Loaded(object sender, RoutedEventArgs e)
		{
			Classes.Loaded -= Classes_Loaded;
			if (GroupData.Classes.Count() == 0) HideSection(ref Classes);
		}

		private void Enumerations_Loaded(object sender, RoutedEventArgs e)
		{
			Enumerations.Loaded -= Enumerations_Loaded;
			if (GroupData.Enumerations.Count() == 0) HideSection(ref Enumerations);
		}

		private void Interfaces_Loaded(object sender, RoutedEventArgs e)
		{
			Interfaces.Loaded -= Interfaces_Loaded;
			if (GroupData.Interfaces.Count() == 0) HideSection(ref Interfaces);
		}

		private void Structures_Loaded(object sender, RoutedEventArgs e)
		{
			Structures.Loaded -= Interfaces_Loaded;
			if (GroupData.Structures.Count() == 0) HideSection(ref Structures);
		}

		private void HideSection(ref HubPanelSection section) => MemoryManager.Delete(ref section, (section) => Panel.Sections.Remove(section));

		private void DescriptiveButton_Click(object sender, RoutedEventArgs e)
		{
			(App.PageTypeCache.GetInstance(typeof(ApiReference)) as ApiReference).Navigate((sender as Button).DataContext as IContentInfo);
		}
	}
}