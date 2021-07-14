using System;

namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Notifies clients that the value has changed.
	/// </summary>
	public interface INotifyValueChanged
	{
		/// <summary>
		/// Occurs when the value held by this object changes. 
		/// </summary>
		public event EventHandler<ValueChangedEventArgs> ValueChanged;
	}
}