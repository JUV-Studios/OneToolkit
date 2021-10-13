using System;
using System.Collections.Generic;
using Windows.System;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.ApplicationModel.Resources;
using Microsoft.Toolkit.Mvvm.ComponentModel;
using OneToolkit.UI;
using OneToolkit.Storage;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : ObservableObject
	{
		private readonly SettingsService SettingsService = new(ApplicationData.Current.RoamingSettings);

		private SettingsViewModel()
		{
			ElementSoundPlayer.State = DisableSoundEffects ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
		}

		public static SettingsViewModel Instance { get; } = new();

		public static readonly SystemNavigationManager NavigationManager = SystemNavigationManager.GetForCurrentView();

		public static readonly ResourceLoader Resources = ResourceLoader.GetForViewIndependentUse();

		public static readonly ViewService ViewServiceProvider = ViewService.GetForCurrentView();

		public static readonly IEnumerable<string> ProgrammingLanguages = new string[]
		{
			"C#", "C++/CX", "C++/WinRT", "Rust", "Visual Basic"
		};

		public bool DisableSoundEffects
		{
			get => Convert.ToBoolean(SettingsService.GetValue(nameof(DisableSoundEffects), false));
			set
			{
				if (DisableSoundEffects != value)
				{
					ElementSoundPlayer.State = value ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
					SettingsService.SetValue(nameof(DisableSoundEffects), value);
					OnPropertyChanged(nameof(DisableSoundEffects));
				}
			}
		}

		public bool PreviewAutoRefresh
		{
			get => Convert.ToBoolean(SettingsService.GetValue(nameof(PreviewAutoRefresh), true));
			set
			{
				if (PreviewAutoRefresh != value)
				{
					SettingsService.SetValue(nameof(PreviewAutoRefresh), value);
					OnPropertyChanged(nameof(PreviewAutoRefresh));
				}
			}
		}

		public string SelectedProgrammingLanguage
		{
			get => SettingsService.GetValue(nameof(SelectedProgrammingLanguage), "C#").ToString();
			set
			{
				if (SelectedProgrammingLanguage != value)
				{
					SettingsService.SetValue(nameof(SelectedProgrammingLanguage), value);
					OnPropertyChanged(nameof(SelectedProgrammingLanguage));
				}
			}
		}

		public static async void Suspend()
		{
			await (await AppDiagnosticInfo.RequestInfoForAppAsync())[0].GetResourceGroups()[0].StartSuspendAsync();
		}
	}
}