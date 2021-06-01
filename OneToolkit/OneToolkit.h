// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++/WinRT projects.

#pragma once
#include <juv.h>
#include <concepts>
#include <unknwn.h>
#include <winerror.h>
#include <winrt/OneToolkit.Mvvm.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/OneToolkit.Lifecycle.h>
#include <winrt/OneToolkit.Storage.h>
#include <winrt/OneToolkit.Data.Text.h>
#include <winrt/OneToolkit.UI.Converters.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace juv
{
	/// <summary>
	/// Represents a type which can be passed through the Windows Runtime ABI.
	/// </summary>
	template <typename T>
	concept WindowsRuntimeType = winrt::impl::has_category_v<T>;

	namespace runtime
	{
		class dynamic_module
		{
		public:
			dynamic_module(winrt::hstring const& fileName) : FileName(fileName)
			{
				set_handle(WINRT_IMPL_LoadLibraryW(fileName.data()));
			}

			dynamic_module(dynamic_module const& another)
			{
				set_handle(WINRT_IMPL_LoadLibraryW(another.FileName.data()));
				FileName = another.FileName;
			}

			dynamic_module(dynamic_module&& another)
			{
				set_handle(another.Handle);
				FileName = another.FileName;
				another.Handle = nullptr;
			}

			template <typename Func>
			auto get_proc_address(std::string_view procName) const
			{
				auto result = WINRT_IMPL_GetProcAddress(Handle, procName.data());
				if (!result) winrt::throw_last_error();
				return reinterpret_cast<Func>(result);
			}

			template <typename Func>
			auto get_proc_address(std::wstring_view procName) const
			{
				auto procNameA = winrt::to_string(procName);
				return get_proc_address(procNameA);
			}

			~dynamic_module()
			{
				if (Handle) WINRT_IMPL_FreeLibrary(Handle);
			}

			void* Handle;
			winrt::hstring FileName;
		private:
			inline void set_handle(void* newHandle)
			{
				if (!newHandle) winrt::throw_last_error();
				Handle = newHandle;
			}
		};
	}

	namespace collections
	{
		namespace details
		{
			template <WindowsRuntimeType T>
			struct RawArrayIterator : winrt::implements<RawArrayIterator<T>, winrt::Windows::Foundation::Collections::IIterator<T>>
			{
			public:
				RawArrayIterator(winrt::array_view<T> arrayView) : m_ArrayView(arrayView)
				{
				}

				T Current() const
				{
					return m_ArrayView[m_CurrentIndex];
				}

				bool HasCurrent() const noexcept
				{
					return m_CurrentIndex < m_ArrayView.size();
				}

				uint32_t GetMany(winrt::array_view<T> items)
				{
					if (items.size() > m_ArrayView.size()) throw winrt::hresult_out_of_bounds();
					for (uint32_t index = 0; index < items.size(); ++index) items[index] = m_ArrayView[index];
					return items.size();
				}

				bool MoveNext() noexcept
				{
					m_CurrentIndex++;
					return m_CurrentIndex < m_ArrayView.size();
				}
			private:
				uint32_t m_CurrentIndex = 0;
				winrt::array_view<T> m_ArrayView;
			};
		}

		/// <summary>
		/// Creates a Windows Runtime collection iterator over a raw array.
		/// </summary>
		template <WindowsRuntimeType T>
		winrt::Windows::Foundation::Collections::IIterator<T> raw_array_iterator(winrt::array_view<T> const& arrayView) noexcept
		{
			return winrt::make<details::RawArrayIterator<T>>(arrayView);
		}
	}
}

namespace winrt::OneToolkit
{
	namespace Lifecycle
	{
		/// <summary>
		/// Provides a base class to conveniently implement the IClosable interface.
		/// <remarks>Your derived class must provide a public or protected Dispose method which will be called by Close when the object hasn't been closed yet.</remarks>
		/// </summary>
		template <typename Derived>
		struct Disposable
		{
		public:
			void Close() noexcept
			{
				if (!m_IsDisposed)
				{
					static_cast<Derived*>(this)->Dispose();
					m_IsDisposed = true;
				}
			}

			~Disposable()
			{
				Close();
			}
		protected:
			Disposable() = default;

			bool m_IsDisposed = false;

			inline void ThrowIfDisposed() const
			{
				if (m_IsDisposed) throw hresult_error(RO_E_CLOSED);
			}
		};

		/// <summary>
		/// Provides a base class to conveniently implement the ISuspendable interface.
		/// </summary>
		template <typename Derived>
		struct Suspendable
		{
		public:
			/// <summary>
			/// Gets whether the current object is suspended.
			/// </summary>
			bool IsSuspended() const noexcept
			{
				return m_IsSuspended;
			}

			event_token StateChanged(SuspendableStateChangedEventHandler const& handler)
			{
				return m_StateChanged.add(handler);
			}

			void StateChanged(event_token token) noexcept
			{
				m_StateChanged.remove(token);
			}
		protected:
			Suspendable() = default;

			bool m_IsSuspended = false;

			/// <summary>
			/// Raises the state changed event.
			/// </summary>
			void Raise(SuspendableEventType eventType)
			{
				m_StateChanged(*static_cast<Derived*>(this), eventType);
			}
		private:
			SuspendableStateChangedEventHandler m_StateChanged;
		};
	}
}

namespace winrt::OneToolkit::Mvvm
{
	enum class PropertyEventType : uint8_t
	{
		PropertyChanging, PropertyChanged
	};

	/// <summary>
	/// Represents a ViewModel which can raise property changed and property changing events.
	/// <remarks>You don't usually need to derive from this class directly. If you just want features of a view model, you can derive from the ObservableBase class.</remarks>
	/// </summary>
	struct ComposableObservableBase
	{
	public:
		/// <summary>
		/// Represents whether the property changing/property changed events are raised or not when you call the Raise method.
		/// </summary>
		/// <returns></returns>
		bool SuppressEvents() const noexcept
		{
			return m_SuppressEvents;
		}

		/// <summary>
		/// Automatically sets a property value and raises property changing/property changed when required.
		/// </summary>
		template <typename T>
		void SetProperty(T& field, T newValue, hstring const& propertyName)
		{
			if (field != newValue)
			{
				Raise(propertyName, PropertyEventType::PropertyChanging);
				field = newValue;
				Raise(propertyName, PropertyEventType::PropertyChanged);
			}
		}

		/// <summary>
		/// Raises the specified property event for a specified property name.
		/// </summary>
		/// <param name="propertyName">The name of the property. Shouldn't be empty or only full of whitespaces.</param>
		/// <param name="type">The type of the property event.</param>
		virtual void Raise(hstring const& propertyName, PropertyEventType eventType = PropertyEventType::PropertyChanged) = 0;
	protected:
		ComposableObservableBase() = default;

		bool m_SuppressEvents = false;
	};

	/// <summary>
	/// Provides a base class for view models and observable objects.
	/// If you're implementing a runtime class, declare that you implement the IViewModel interface in metadata or another interface that's similar.
	/// </summary>
	template <typename Derived, juv::WindowsRuntimeType ChangedDelegate = Windows::UI::Xaml::Data::PropertyChangedEventHandler, 
		juv::WindowsRuntimeType ChangedArgs = Windows::UI::Xaml::Data::PropertyChangedEventArgs>
	struct ObservableBase : ComposableObservableBase
	{
	public:
		event_token PropertyChanged(ChangedDelegate const& handler)
		{
			return m_PropertyChanged.add(handler);
		}

		void PropertyChanged(event_token token) noexcept
		{
			m_PropertyChanged.remove(token);
		}

		event_token PropertyChanging(PropertyChangingEventHandler const& handler)
		{
			return m_PropertyChanging.add(handler);
		}

		void PropertyChanging(event_token token) noexcept
		{
			m_PropertyChanging.remove(token);
		}

		void Raise(hstring const& propertyName, PropertyEventType eventType = PropertyEventType::PropertyChanged) final
		{
			if (!juv::has_only_whitespaces(propertyName) && Decide(propertyName, eventType) && !m_SuppressEvents)
			{
				if (eventType == PropertyEventType::PropertyChanging)
				{
					PropertyChangingEventArgs args{ propertyName };
					m_PropertyChanging(*static_cast<Derived*>(this), args);
					WhenPropertyChanging(args);
				}
				else
				{
					ChangedArgs args{ propertyName };
					m_PropertyChanged(*static_cast<Derived*>(this), args);
					WhenPropertyChanged(args);
				}
			}
		}
	protected:
		ObservableBase() = default;

		/// <summary>
		/// Override this method to determine whether to raise property changed/property changing or not.
		/// </summary>
		virtual bool Decide(hstring const&, PropertyEventType) const noexcept
		{
			return true;
		}

		/// <summary>
		/// Override this method to perform custom actions after raising the property changed event.
		/// </summary>
		virtual void WhenPropertyChanged(ChangedArgs const&)
		{
			// Do nothing here.
		}

		/// <summary>
		/// Override this method to perform custom actions after raising the property changing event.
		/// </summary>
		virtual void WhenPropertyChanging(PropertyChangingEventArgs const&)
		{
			// Do nothing here.
		}
	private:
		event<ChangedDelegate> m_PropertyChanged;
		event<PropertyChangingEventHandler> m_PropertyChanging;
	};

	/// <summary>
	/// Represents an property which automatically raises property changed/property changing events when it's value is set.
	/// </summary>
	/// <typeparam name="T">The underlying type of the property.</typeparam>
	template <typename T>
	struct ObservableProperty
	{
	public:
		ObservableProperty(ComposableObservableBase* const holder, hstring const& propertyName, T value = {}) : m_Holder(*holder), m_PropertyName(propertyName), m_BackingField(value)
		{
		}

		/// <summary>
		/// Gets the underlying value for this ObservableProperty instance.
		/// </summary>
		T operator()() const noexcept
		{
			return m_BackingField;
		}

		/// <summary>
		/// Sets the underlying value for this ObservableProperty instance. Automatically raises PropertyChanging/PropertyChanged when you set it unless suppress is enabled.
		/// </summary>
		void operator()(T value)
		{
			m_Holder.SetProperty(m_BackingField, value, m_PropertyName);
		}

		/// <summary>
		/// Returns a string that represents the current object.
		/// </summary>
		/// <returns>A string that represents the current object.</returns>
		hstring ToString() const noexcept
		{
			return m_PropertyName;
		}

		/// <summary>
		/// Enables access to the underlying data storage.
		/// </summary>
		/// <returns>A reference to the underlying data storage.</returns>
		T& GetBackingField() noexcept
		{
			return m_BackingField;
		}
	private:
		T m_BackingField;
		hstring m_PropertyName;
		ComposableObservableBase& m_Holder;
	};
}