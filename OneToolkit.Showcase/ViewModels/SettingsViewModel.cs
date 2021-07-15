using Windows.Storage;
using OneToolkit.UI;
using OneToolkit.Mvvm;
using OneToolkit.Storage;
using System.Collections.Generic;
using Windows.UI.Xaml;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : ObservableBase
	{
		private readonly SettingsService SettingsService = new(ApplicationData.Current.RoamingSettings);

		private SettingsViewModel()
		{
			ElementSoundPlayer.State = DisableSoundEffects ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
		}

		public static SettingsViewModel Instance { get; } = new();

		public static readonly IViewServiceUniversal ViewServiceProvider = ViewService.GetForCurrentView();

		public static readonly IEnumerable<string> ProgrammingLanguages = new string[]
		{
			"C#", "C++/CX", "C++/WinRT", "Rust", "Visual Basic"
		};

		public bool DisableSoundEffects
		{
			get => (bool)SettingsService.GetValue(nameof(DisableSoundEffects), false);
			set
			{
				if (DisableSoundEffects != value)
				{
					ElementSoundPlayer.State = value ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
					SettingsService.SetValue(nameof(DisableSoundEffects), value);
					Raise(nameof(DisableSoundEffects));
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
					Raise(nameof(SelectedProgrammingLanguage));
				}
			}
		}
	}
}
