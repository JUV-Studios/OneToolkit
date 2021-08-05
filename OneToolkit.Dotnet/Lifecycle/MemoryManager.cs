using System;

namespace OneToolkit.Lifecycle
{
	/// <summary>
	/// Provides helpers to manage object lifetime.
	/// </summary>
	public static class MemoryManager
	{
		/// <summary>
		/// Deletes an object reference. Disposes the object if it implements IDisposable and nulls the reference out.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="reference"></param>
		public static void Delete<T>(ref T reference) where T : class => Delete(ref reference, reference =>
			{
				if (reference is IDisposable disposable) disposable.Dispose();
			});

		/// <summary>
		/// Deletes an object reference using a custom deleter and nulls out the reference.
		/// </summary>
		public static void Delete<T>(ref T reference, Action<T> deleter) where T : class
		{
			if (reference != null)
			{
				deleter(reference);
				reference = null;
			}
		}
	}
}