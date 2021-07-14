using System;
using System.Windows.Input;

namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Defines a command with a default argument.
	/// </summary>
	public interface IDefaultCommand : ICommand
	{
		/// <summary>
		/// Defines the method to be called when the command is invoked using the default argument.
		/// </summary>
		public void Execute();

		/// <summary>
		/// Defines the method that determines whether the command can execute in its current state using the default argument.
		/// </summary>
		/// <returns>true if this command can be executed; otherwise, false.</returns>
		public bool CanExecute();

		/// <summary>
		/// The default argument that will be used for CanExecute and Execute methods respectively if an argument isn't provided.
		/// </summary>
		public object DefaultParameter { get; set; }

		/// <summary>
		/// Gets whether the command can be executed with the default argument.
		/// </summary>
		public bool IsEnabled { get; }
	}

	/// <summary>
	/// Provides a bindable command which can choose to execute or not.
	/// </summary>
	public sealed class RelayCommand : IDefaultCommand, IObservableHolder
	{
		private readonly Action<RelayCommand, object> Functor;

		private readonly Func<RelayCommand, object, bool> Decider;

		internal static bool CanExecuteDefault(RelayCommand command, object parameter) => true;

		/// <summary>
		/// Creates a new instance of RelayCommand using a functor and decider.
		/// </summary>
		public RelayCommand(Action<RelayCommand, object> functor)
		{
			Functor = functor;
			Decider = CanExecuteDefault;
		}

		/// <summary>
		/// Creates a new instance of RelayCommand using a functor and decider.
		/// </summary>
		public RelayCommand(Action<RelayCommand, object> functor, Func<RelayCommand, object, bool> decider)
		{
			Functor = functor;
			Decider = decider;
		}

		/// <summary>
		/// Creates a new instance of RelayCommand using a functor and decider along with a name.
		/// </summary>
		public RelayCommand(Action<RelayCommand, object> functor, Func<RelayCommand, object, bool> decider, string propertyName)
		{
			if (string.IsNullOrWhiteSpace(propertyName)) throw new ArgumentException("PropertyName for an RelayCommand instance must not be empty or only full of whitespaces.");
			Functor = functor;
			Decider = decider;
			PropertyName = propertyName;
		}

		/// <summary>
		/// Gets the name of this RelayCommand which will be used when raising property events. 
		/// </summary>
		public readonly string PropertyName;

		/// <summary>
		/// The default argument that will be used for CanExecute and Execute methods respectively if an argument isn't provided.
		/// </summary>
		public object DefaultParameter { get; set; }

		private WeakReference<ObservableBase> _Holder;

		WeakReference<ObservableBase> IObservableHolder.Holder => _Holder ??= new(null);

		/// <summary>
		/// Occurs when changes occur that affect whether or not the command should execute.
		/// </summary>
		public event EventHandler CanExecuteChanged;

		/// <summary>
		/// Gets whether the command can be executed with the default argument.
		/// </summary>
		public bool IsEnabled => CanExecute();

		/// <summary>
		/// Defines the method that determines whether the command can execute in its current state using the default argument.
		/// </summary>
		/// <returns>true if this command can be executed, otherwise false.</returns>
		public bool CanExecute() => CanExecute(DefaultParameter);

		/// <summary>
		/// Defines the method that determines whether the command can execute in its current state.
		/// </summary>
		/// <param name="parameter">Data used by the command. If the command does not require data to be passed, this object can be set to null.</param>
		/// <returns>true if this command can be executed; otherwise, false.</returns>
		public bool CanExecute(object parameter) => (Decider?.Invoke(this, parameter)).GetValueOrDefault(true);

		/// <summary>
		/// Defines the method to be called when the command is invoked using the default argument.
		/// </summary>
		public void Execute() => Execute(DefaultParameter);

		/// <summary>
		/// Defines the method to be called when the command is invoked.
		/// </summary>
		/// <param name="parameter">Data used by the command. If the command does not require data to be passed, this object can be set to null.</param>
		public void Execute(object parameter)
		{
			if (CanExecute(parameter)) Functor(this, parameter);
		}

		/// <summary>
		/// Raises the CanExecuteChanged event and PropertyChanged for IsEnabled when used as a field in a class that derives from ObservableBase.
		/// </summary>
		public void Raise()
		{
			CanExecuteChanged(this, new());
			if (_Holder != null)
			{
				if (_Holder.TryGetTarget(out ObservableBase holder)) ObservableBase.Raise(holder, $"{PropertyName}.IsEnabled");
			}
		}
	}
}
