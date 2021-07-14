namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Provides data for the ValueChanged event.
	/// </summary>
	public readonly struct ValueChangedEventArgs
	{
		/// <summary>
		/// Creates a new instance of ValueChangedEventArgs.
		/// </summary>
		public ValueChangedEventArgs(object oldValue, object newValue)
		{
			OldValue = oldValue;
			NewValue = newValue;
		}

		/// <summary>
		/// The value before the change.
		/// </summary>
		public readonly object OldValue;

		/// <summary>
		/// The value after the change.
		/// </summary>
		public readonly object NewValue;
	}
}
