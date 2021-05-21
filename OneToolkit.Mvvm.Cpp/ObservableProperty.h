// (c) 2021 JUV Studios. All rights reserved. Included as part of the OneToolkit MVVM framework for C++.

#pragma once
#include <winrt/OneToolkit.Mvvm.h>

namespace winrt
{
	namespace OneToolkit::Mvvm
	{
		template <typename T>
		constexpr T get_default_value()
		{
			if (std::is_trivially_constructible_v<T>) return {};
			else
			{
				if (std::is_base_of_v<Windows::Foundation::IUnknown, T> || std::is_pointer_v<T>) return nullptr;
				else static_assert(false, "There isn't a default value for this type.");
			}
		}

		/// <summary>
		/// Represents an property which automatically raises property changed/property changing events when it's value is set.
		/// </summary>
		/// <typeparam name="T">The underlying type of the property.</typeparam>
		/// <remarks>A field of this class should be only declared in a class that inherits from ObservableBase.</remarks>
		template <typename T, const wchar_t* PropertyName> requires impl::has_category_v<T>
		struct ObservableProperty
		{
		public:
			ObservableProperty(ObservableBase const& holder, T defaultValue = get_default_value<T>()) : m_Holder(holder), m_BackingField(defaultValue)
			{
			}

			/// <summary>
			/// Gets the underlying value for this ObservableProperty instance. Automatically raises PropertyChanged when needed.
			/// </summary
			T operator() const noexcept
			{
				return m_BackingField;
			}

			/// <summary>
			/// Sets the underlying value for this ObservableProperty instance and raises PropertyChanged if the holder is still alive.
			/// </summary
			bool operator(T value)
			{
				if (m_BackingField != value)
				{
					m_BackingField = value;
					if (auto base = m_Holder.get())
					{
						ObservableBaseHelper::Raise(base, ToString());
						return true;
					}
					else return false;
				}
			}

			/// <summary>
			/// Enables access to the underlying data storage.
			/// </summary>
			/// <returns>A reference to the data storage.</returns>
			T& operator*() noexcept
			{
				return m_BackingField;
			}

			hstring ToString() const noexcept
			{
				return { PropertyName };
			}
		private:
			T m_BackingField;
			weak_ref<ObservableBase> m_Holder;
		};
	}

	template <typename T>
	hstring to_hstring(ObservableProperty<T> const& value)
	{
		return value.ToString();
	}
}