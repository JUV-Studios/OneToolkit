using System;

namespace OneToolkit.Mvvm
{
	public interface INotifyValueChanged
	{
		/// <summary>
		/// Fired when the value held by this object changes. 
		/// </summary>
		public event EventHandler<ValueChangedEventArgs> ValueChanged;
	}
}