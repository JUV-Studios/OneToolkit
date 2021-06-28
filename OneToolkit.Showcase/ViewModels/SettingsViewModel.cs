using System;
using Windows.Storage;
using OneToolkit.UI;
using OneToolkit.Mvvm;
using OneToolkit.Storage;
using System.Collections.Generic;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : ObservableBase
	{
		private readonly SettingsService SettingsService = new(ApplicationData.Current.RoamingSettings);

		private SettingsViewModel()
		{
			SettingsService.DataContainer.Values.Clear();
		}

		public static SettingsViewModel Instance { get; } = new();

		public static readonly IViewServiceProvider ViewServiceProvider = ViewService.GetForCurrentView();

		public static readonly IEnumerable<string> ProgrammingLanguages = new string[]
		{
			"C#", "C++/CX", "C++/WinRT", "Rust", "Visual Basic"
		};

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
