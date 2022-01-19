using System;
using System.Linq;
using System.Reflection;
using System.Collections.Generic;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.ApplicationModel.DataTransfer;
using Windows.UI.Text;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Markup;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Microsoft.UI.Xaml.Controls;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;
using System.Threading.Tasks;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class Playgrounds : Page
	{
		public Playgrounds() => InitializeComponent();

		private Dictionary<string, IEnumerable<Type>> _ToolboxItems;

		private Dictionary<string, IEnumerable<Type>> ToolboxItems
		{
			get
			{
				if (_ToolboxItems == null)
				{
					_ToolboxItems = new();
					var controlsText = SettingsViewModel.Resources.GetString("ControlsText");
					var getXamlTypes = (Assembly assembly) =>
					{
						return from type in assembly.ExportedTypes
							   where type.IsClass && type.IsSubclassOf(typeof(UIElement)) && type.GetConstructor(Type.EmptyTypes) != null
							   select type;
					};

					var searchAssemblies = new[]
					{
						typeof(HubPanel).Assembly, typeof(TeachingTip).Assembly, typeof(UIElement).Assembly
					};

					foreach (var assembly in searchAssemblies)
					{
						string label = string.Empty;
						if (assembly == typeof(HubPanel).Assembly) label = "OneToolkit";
						else if (assembly == typeof(TeachingTip).Assembly) label = "WinUI";
						else if (assembly == typeof(UIElement).Assembly) label = "UWP XAML";
						_ToolboxItems.Add($"{label} {controlsText}", getXamlTypes(assembly));
					}
				}

				return _ToolboxItems;
			}
		}

		private ContentPresenter PreviewPresenter;

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			SettingsViewModel.TransferManager.DataRequested += TransferManager_DataRequested;
		}

		protected override void OnNavigatedFrom(NavigationEventArgs e)
		{
			base.OnNavigatedFrom(e);
			SettingsViewModel.ViewServiceProvider.Title = string.Empty;
			SettingsViewModel.TransferManager.DataRequested -= TransferManager_DataRequested;
		}

		private async Task OpenFromFileAsync()
		{
			FileOpenPicker openPicker = new()
			{
				FileTypeFilter = { ".xaml" },
				SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
				CommitButtonText = SettingsViewModel.Resources.GetString("PlaygroundsOpenNavItem/Content")
			};

			var file = await openPicker.PickSingleFileAsync();
			if (file != null) MarkupEditor.Text = await FileIO.ReadTextAsync(file);
		}

		private async Task SaveToFileAsync()
		{
			FileSavePicker savePicker = new()
			{
				SuggestedFileName = "Untitled",
				SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
				FileTypeChoices = { new("XAML file", new string[] { ".xaml" }) },
				CommitButtonText = SettingsViewModel.Resources.GetString("PlaygroundsSaveNavItem/Content")
			};

			var file = await savePicker.PickSaveFileAsync();
			if (file != null) await FileIO.WriteTextAsync(file, MarkupEditor.Text);
		}

		private void TransferManager_DataRequested(DataTransferManager sender, DataRequestedEventArgs args)
		{
			var deferral = args.Request.GetDeferral();
			MarkupEditor.TextDocument.GetText(TextGetOptions.UseLf, out string text);
			args.Request.Data.Properties.Title = SettingsViewModel.Resources.GetString("PlaygroundsShareNavItem/Content");
			args.Request.Data.Properties.Description = SettingsViewModel.Resources.GetString("ShareButton/Caption");
			args.Request.Data.SetText(text);
			deferral.Complete();
		}

		private void PreviewPresenter_Loaded(object sender, RoutedEventArgs e)
		{
			PreviewPresenter = sender as ContentPresenter;
			PreviewPresenter.Loaded -= PreviewPresenter_Loaded;
		}

		private void Editor_TextChanged(object sender, RoutedEventArgs e)
		{
			if (PreviewPresenter != null)
			{
				try { PreviewPresenter.Content = XamlReader.Load(MarkupEditor.Text); }
				catch (Exception ex) { PreviewPresenter.Content = ex.Message; }
			}
		}

		private async void PaneNavView_ItemInvoked(Microsoft.UI.Xaml.Controls.NavigationView sender, Microsoft.UI.Xaml.Controls.NavigationViewItemInvokedEventArgs args)
		{
			switch (args.InvokedItemContainer.Tag)
			{
				case "Open":
					await OpenFromFileAsync();
					break;
				case "Save":
					await SaveToFileAsync();
					break;
				case "Share":
					DataTransferManager.ShowShareUI();
					break;
			}
		}
	}
}
