using OneToolkit.Mvvm;
using System.ComponentModel;

namespace OneToolkit.Interop.WindowsRuntime
{
	internal sealed class ViewModelWrapper : IViewModel
	{
		public readonly ObservableBase Target;

		public ViewModelWrapper(ObservableBase target)
		{
			Target = target;
		}

		public bool SuppressEvents => Target.SuppressEvents;

		public event PropertyChangedEventHandler PropertyChanged
		{
			add => Target.PropertyChanged += value;
			remove => Target.PropertyChanged -= value;
		}

		public event Mvvm.PropertyChangingEventHandler PropertyChanging;
	}

	public static class InteropExtensions
	{
		/// <summary>
		/// Wraps an ObservableBase instance in an object implementing the IViewModel interface.
		/// </summary>
		public static IViewModel AsViewModel(this ObservableBase observableBase) => new ViewModelWrapper(observableBase);

		/// <summary>
		/// Tries to convert an IViewModel into an ObservableBase if it was wrapped using AsViewModel.
		/// </summary>
		public static bool TryAsObservableBase(this IViewModel viewModel, out ObservableBase observableBase)
		{
			if (viewModel is ViewModelWrapper wrapper)
			{
				observableBase = wrapper.Target;
				return true;
			}
			else
			{
				observableBase = null;
				return true;
			}
		}

		/// <summary>
		/// Converts a non-generic ObservableValue to a generic ObservableValue of type object.
		/// </summary>
		public static ObservableValue<object> ToGeneric(this Mvvm.NonGeneric.ObservableValue nonGenericValue) => new(nonGenericValue.Value, nonGenericValue.SuppressEvents);

		/// <summary>
		/// Converts a non-generic ObservableValue to a generic ObservableValue of the type specified.
		/// </summary>
		public static ObservableValue<T> ToGeneric<T>(this Mvvm.NonGeneric.ObservableValue nonGenericValue) => new((T)nonGenericValue.Value, nonGenericValue.SuppressEvents);

 		/// <summary>
		/// Converts a generic ObservableValue to a non-generic ObservableValue which can be passed through the Windows Runtime ABI.
		/// </summary>
		public static Mvvm.NonGeneric.ObservableValue ToNonGeneric<T>(this Mvvm.ObservableValue<T> genericValue) => new(genericValue.Value, genericValue.SuppressEvents);
	}
}