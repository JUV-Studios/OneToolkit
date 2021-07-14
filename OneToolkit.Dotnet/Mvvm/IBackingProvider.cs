using System;

namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Represents an object the provides access to its internal data storage.
	/// </summary>
	public interface IBackingProvider<T> : IEquatable<T>
	{

		/// <summary>
		/// Enables access to the underlying data storage.
		/// </summary>
		/// <returns>A reference to the underlying data storage.</returns>
		public ref T GetBackingField();
	}
}