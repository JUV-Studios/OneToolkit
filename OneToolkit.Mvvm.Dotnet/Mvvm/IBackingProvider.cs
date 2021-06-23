using System;

namespace OneToolkit.Mvvm
{
	public interface IBackingProvider<T> : IEquatable<T>
	{

		/// <summary>
		/// Enables access to the underlying data storage.
		/// </summary>
		/// <returns>A reference to the underlying data storage.</returns>
		public ref T GetBackingField();
	}
}