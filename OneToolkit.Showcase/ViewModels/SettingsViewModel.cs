using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using Windows.System;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.ApplicationModel.Resources;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.Toolkit.Mvvm.ComponentModel;
using OneToolkit.UI;
using OneToolkit.Storage;
using OneToolkit.Showcase.Models;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : ObservableObject
	{
		private SettingsViewModel()
		{
			ElementSoundPlayer.State = DisableSoundEffects ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
		}

		private readonly SettingsService AppSettings = new(ApplicationData.Current.RoamingSettings);

		public static SettingsViewModel Instance { get; } = new();

		public static readonly SystemNavigationManager NavigationManager = SystemNavigationManager.GetForCurrentView();

		public static readonly ResourceLoader Resources = ResourceLoader.GetForViewIndependentUse();

		public static readonly ViewService ViewServiceProvider = ViewService.GetForCurrentView();

		public static readonly DataTransferManager TransferManager = DataTransferManager.GetForCurrentView();

		public static readonly IEnumerable<string> ProgrammingLanguages = new string[]
		{
			"C#", "C++/CX", "C++/CLI", "C++/WinRT", "Rust", "Visual Basic"
		};

		public readonly ObservableCollection<PlaygroundsFile> Playgrounds = new();

		public bool DisableSoundEffects
		{
			get => Convert.ToBoolean(AppSettings.GetValue(nameof(DisableSoundEffects), false));
			set
			{
				if (DisableSoundEffects != value)
				{
					ElementSoundPlayer.State = value ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
					AppSettings.SetValue(nameof(DisableSoundEffects), value);
					OnPropertyChanged(nameof(DisableSoundEffects));
				}
			}
		}

		public bool PreviewAutoRefresh
		{
			get => Convert.ToBoolean(AppSettings.GetValue(nameof(PreviewAutoRefresh), true));
			set
			{
				if (PreviewAutoRefresh != value)
				{
					AppSettings.SetValue(nameof(PreviewAutoRefresh), value);
					OnPropertyChanged(nameof(PreviewAutoRefresh));
				}
			}
		}

		public string SelectedProgrammingLanguage
		{
			get => AppSettings.GetValue(nameof(SelectedProgrammingLanguage), "C#").ToString();
			set
			{
				if (SelectedProgrammingLanguage != value)
				{
					AppSettings.SetValue(nameof(SelectedProgrammingLanguage), value);
					OnPropertyChanged(nameof(SelectedProgrammingLanguage));
				}
			}
		}

		public static async void Close() => await ViewServiceProvider.TryCloseAsync();

		public static async void Suspend() => await (await AppDiagnosticInfo.RequestInfoForAppAsync())[0].GetResourceGroups()[0].StartSuspendAsync();
	}
}