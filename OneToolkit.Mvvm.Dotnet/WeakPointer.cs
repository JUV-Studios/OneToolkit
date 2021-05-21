using System;

namespace OneToolkit
{
	/// <summary>
	/// Represents a weak pointer to .NET object that can be attempted to dereferenced when required.
	/// </summary>
	/// <typeparam name="T">The target type of the pointer. Must be a reference type.</typeparam>
	public sealed class WeakPointer<T> where T : class
	{
		private WeakReference<T> _Reference;

		public WeakPointer() { }

		public WeakPointer(T value) => _Reference = new(value);

		public T Target
		{
			get
			{
				if (_Reference == null) return null;
				else if (_Reference.TryGetTarget(out T val)) return val;
				else return null;
			}
			set
			{
				if (value == null) _Reference = null;
				else if (_Reference == null) _Reference = new(value);
				else _Reference.SetTarget(value);
			}
		}

		public bool IsTargetValid => Target != null;

		public void Reset() => _Reference = null;

		public ref WeakReference<T> AsWeakReference()
		{
			return ref _Reference;
		}
	}

	/// <summary>
	/// Represents an object that holds a weak pointer to its parent.
	/// </summary>
	public interface IWeakHolder<T> where T : class
	{
		/// <summary>
		/// A weak pointer to an instance of the parent class.
		/// </summary>
		public WeakPointer<T> Holder { get; }
	}
}