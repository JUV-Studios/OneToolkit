using System;
using System.Collections.Concurrent;

namespace OneToolkit.Lifecycle
{
	/// <summary>
	/// Represents a collection that caches object instances.
	/// </summary>
	/// <typeparam name="Key"> </typeparam>
	/// <typeparam name="Value"></typeparam>
	public abstract class ValueCache<Key, Value>
	{
		public abstract void SetInstance(Key key, Value value);

		public abstract Value GetInstance(Key key, Func<Value> fallbackProvider);

		protected static bool TryGetFallbackValue(Func<Value> fallbackProvider, out Value result)
		{
			if (fallbackProvider != null)
			{
				result = fallbackProvider();
				return true;
			}
			else
			{
				result = default;
				return false;
			}
		}
	}

	public sealed class WeakValueCache<Key, Value> : ValueCache<Key, Value> where Value : class
	{
		private readonly ConcurrentDictionary<Key, WeakReference<Value>> Instances = new();
		
		public override Value GetInstance(Key key, Func<Value> fallbackProvider)
		{
			if (Instances.TryGetValue(key, out var value))
			{
				if (value.TryGetTarget(out var result)) return result;
			}

			if (TryGetFallbackValue(fallbackProvider, out var fallback)) SetInstance(key, fallback);
			return fallback;
		}

		public override void SetInstance(Key key, Value value)
		{
			Instances[key] = new(value);
		}
	}

	public sealed class StrongValueCache<Key, Value> : ValueCache<Key, Value>
	{
		private readonly ConcurrentDictionary<Key, Value> Instances = new();

		public override void SetInstance(Key key, Value value)
		{
			Instances[key] = value;
		}

		public override Value GetInstance(Key key, Func<Value> fallbackProvider)
		{
			if (Instances.TryGetValue(key, out var value)) return value;
			if (TryGetFallbackValue(fallbackProvider, out var fallback)) SetInstance(key, fallback);
			return fallback;
		}
	}
}