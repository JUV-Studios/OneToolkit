using System;
using Windows.Storage.Pickers;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;
using Windows.ApplicationModel.DataTransfer;
using Windows.UI.Text;
using Windows.Storage;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Markup;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class Playgrounds : Page
	{
		public Playgrounds() => InitializeComponent();

		private ContentPresenter PreviewPresenter;

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			SettingsViewModel.TransferManager.DataRequested += TransferManager_DataRequested;
		}

		protected override void OnNavigatedFrom(NavigationEventArgs e)
		{
			base.OnNavigatedFrom(e);
			SettingsViewModel.TransferManager.DataRequested -= TransferManager_DataRequested;
		}

		private async void OpenFromFile_Click(object sender, RoutedEventArgs e)
		{
			FileOpenPicker openPicker = new()
			{
				FileTypeFilter = { ".xaml" },
				SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
				CommitButtonText = SettingsViewModel.Resources.GetString("OpenText")
			};

			var file = await openPicker.PickSingleFileAsync();
			if (file != null) Editor.TextDocument.SetText(TextSetOptions.None, await FileIO.ReadTextAsync(file));
		}

		private async void SaveToFile_Click(object sender, RoutedEventArgs e)
		{
			FileSavePicker savePicker = new()
			{
				SuggestedFileName = "Untitled",
				SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
				FileTypeChoices = { new("XAML file", new string[] { ".xaml" }) },
				CommitButtonText = SettingsViewModel.Resources.GetString("SaveText")
			};

			var file = await savePicker.PickSaveFileAsync();
			if (file != null)
			{
				Editor.TextDocument.GetText(TextGetOptions.UseLf, out string text);
				await FileIO.WriteTextAsync(file, text);
			}
		}

		private void Share_Click(object sender, RoutedEventArgs e) => DataTransferManager.ShowShareUI();

		private void PreviewPresenter_Loaded(object sender, RoutedEventArgs e)
		{
			PreviewPresenter = sender as ContentPresenter;
			PreviewPresenter.Loaded -= PreviewPresenter_Loaded;
		}

		private void TransferManager_DataRequested(DataTransferManager sender, DataRequestedEventArgs args)
		{
			var deferral = args.Request.GetDeferral();
			Editor.TextDocument.GetText(TextGetOptions.UseLf, out string text);
			args.Request.Data.Properties.Title = SettingsViewModel.Resources.GetString("ShareButton/Title");
			args.Request.Data.Properties.Description = SettingsViewModel.Resources.GetString("ShareButton/Caption");
			args.Request.Data.SetText(text);
			deferral.Complete();
		}

		private void Editor_TextChanged(object sender, RoutedEventArgs e)
		{
			if (PreviewPresenter != null)
			{
				Editor.TextDocument.GetText(TextGetOptions.UseLf, out string text);
				try { PreviewPresenter.Content = XamlReader.Load(text); }
				catch (Exception ex) { PreviewPresenter.Content = ex.Message; }
			}
		}
	}
}
