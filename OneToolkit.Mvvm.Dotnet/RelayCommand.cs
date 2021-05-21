using System;
using System.Windows.Input;

namespace OneToolkit.Mvvm
{
	public delegate void RelayCommandExecuteHandler(RelayCommand command, object parameter);

	public delegate bool RelayCommandCanExecuteHandler(RelayCommand command, object parameter);

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
		/// The default parameter that will be used for CanExecute and Execute methods respectively if an argument isn't provided.
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
	public sealed class RelayCommand : IDefaultCommand, IWeakHolder<ObservableBase>
	{
		private readonly string PropertyName;

		private readonly RelayCommandExecuteHandler Functor;

		private readonly RelayCommandCanExecuteHandler Decider;

		internal static bool CanExecuteDefault(RelayCommand command, object parameter) => true;

		public RelayCommand(RelayCommandExecuteHandler functor)
		{
			Functor = functor;
			Decider = CanExecuteDefault;
		}

		public RelayCommand(RelayCommandExecuteHandler functor, RelayCommandCanExecuteHandler decider)
		{
			Functor = functor;
			Decider = decider;
		}

		public RelayCommand(RelayCommandExecuteHandler functor, RelayCommandCanExecuteHandler decider, string propertyName)
		{
			Functor = functor;
			Decider = decider;
			PropertyName = propertyName;
		}

		public object DefaultParameter { get; set; }

		public event EventHandler CanExecuteChanged;

		public bool IsEnabled => CanExecute();

		public WeakPointer<ObservableBase> Holder { get; } = new();

		public bool CanExecute() => CanExecute(DefaultParameter);

		public bool CanExecute(object parameter) => Decider.Invoke(this, parameter);

		public void Execute() => Execute(DefaultParameter);

		public void Execute(object parameter)
		{
			if (CanExecute(parameter)) Functor(this, parameter);
		}

		/// <summary>
		/// Raises the CanExecuteChanged event and property changing/property changed for IsEnabled when used as a field in a class that derives from ObservableBase.
		/// </summary>
		public void Raise()
		{
			CanExecuteChanged(this, new());
			if (ObservableBase.Raise(Holder, PropertyName, PropertyEventType.PropertyChanging)) ObservableBase.Raise(Holder, PropertyName, PropertyEventType.PropertyChanged);
		}
		
		public override string ToString() => string.IsNullOrWhiteSpace(PropertyName) ? base.ToString() : PropertyName;
	}
}
