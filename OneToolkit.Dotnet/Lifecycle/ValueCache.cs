using System;
using System.Collections.Concurrent;

namespace OneToolkit.Lifecycle
{
	public interface IValueCache<Key, Value>
	{
		Value GetInstance(Key key, Func<Value> fallbackProvider);

		void SetInstance(Key key, Value value);
	}

	public static class ValueCacheHelper
	{
		public static bool TryGetFallback<Value>(Func<Value> fallbackProvider, out Value result)
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

	public sealed class WeakValueCache<Key, Value> : IValueCache<Key, Value> where Value : class
	{
		private readonly ConcurrentDictionary<Key, WeakReference<Value>> Instances = new();

		public Value GetInstance(Key key, Func<Value> fallbackProvider)
		{
			if (Instances.TryGetValue(key, out var value))
			{
				if (value.TryGetTarget(out var result)) return result;
			}

			if (ValueCacheHelper.TryGetFallback(fallbackProvider, out var fallback)) SetInstance(key, fallback);
			return fallback;
		}

		public void SetInstance(Key key, Value value)
		{
			Instances[key] = new(value);
		}
	}

	public sealed class StrongValueCache<Key, Value> : IValueCache<Key, Value>
	{
		private readonly ConcurrentDictionary<Key, Value> Instances = new();
		public Value GetInstance(Key key, Func<Value> fallbackProvider)
		{
			if (Instances.TryGetValue(key, out var value))
			{
				if (value != null) return value;
			}

			if (ValueCacheHelper.TryGetFallback(fallbackProvider, out var fallback)) SetInstance(key, fallback);
			return fallback;
		}

		public void SetInstance(Key key, Value value)
		{
			Instances[key] = value;
		}
	}
}