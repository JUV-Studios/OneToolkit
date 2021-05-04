using System;

namespace OneToolkit
{
	public sealed class WeakPointer<T> where T : class
	{
		private WeakReference<T> _Reference;

		public T Target
		{
			get
			{
				if (_Reference == null)
				{
					return null;
				}
				else if (_Reference.TryGetTarget(out T target))
				{
					return target;
				}
				else
				{
					return null;
				}
			}
			set
			{
				if (value == null)
				{
					_Reference = null;
				}
				else if (_Reference == null)
				{
					_Reference = new(value);
				}
				else
				{
					_Reference.SetTarget(value);
				}
			}
		}

		public bool IsTargetValid => Target != null;

		public ref WeakReference<T> AsWeakReference()
		{
			return ref _Reference;
		}
	}

	interface IWeakHolder<T> where T : class
	{
		public WeakPointer<T> Holder { get; }
	}
}