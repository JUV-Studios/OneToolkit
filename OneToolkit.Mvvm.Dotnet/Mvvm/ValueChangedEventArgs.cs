namespace OneToolkit.Mvvm
{
	public readonly struct ValueChangedEventArgs
	{
		public ValueChangedEventArgs(object oldValue, object newValue)
		{
			OldValue = oldValue;
			NewValue = newValue;
		}

		public readonly object OldValue;

		public readonly object NewValue;
	}
}
