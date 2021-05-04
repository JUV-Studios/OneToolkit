using System.Collections.Generic;

namespace OneToolkit.Mvvm
{
	public sealed class ObservableProperty<T> : IWeakHolder<IPropertyEventRaiser>
	{
		private readonly string _PropertyName;

		private T _Value;

		public T Value
		{
			get => _Value;
			set
			{
				if (!Comparer.Equals(_Value, value))
				{
					var target = Holder.Target;
					target.Raise(_PropertyName, PropertyEventType.PropertyChanging);
					_Value = value;
					target.Raise(_PropertyName, PropertyEventType.PropertyChanged);
				}
			}
		}

		public WeakPointer<IPropertyEventRaiser> Holder { get; } = new();

		public EqualityComparer<T> Comparer { get; set; } = EqualityComparer<T>.Default;

		public ObservableProperty(string propertyName, T value = default)
		{
			_Value = value;
			_PropertyName = propertyName;
		}

		public override bool Equals(object obj)
		{
			if (obj is ObservableProperty<T> propertyRef)
			{
				return Comparer.Equals(_Value, propertyRef._Value);
			}
			else if (obj is T val)
			{
				return Comparer.Equals(_Value, val);
			}
			else
			{
				return false;
			}
		}

		public override int GetHashCode()
		{
			return (_Value?.GetHashCode()).GetValueOrDefault(base.GetHashCode());
		}

		public override string ToString() => _PropertyName;
	}
}