module;

#include "OneToolkit.h"
#include <span>
#include <memory>
#include <format>
#include <cstddef>
#include <concepts>
#include <filesystem>
#include <Unknwn.h>
#include <Windows.h>
#include <winrt/OneToolkit.System.h>
#include <winrt/OneToolkit.UI.Windowing.h>
#include <winrt/OneToolkit.Storage.Logging.h>
#if __has_include(<winrt/Microsoft.UI.Xaml.Data.h>) && !force_platform_xaml
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#else
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#endif

export module OneToolkit;

import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace OneToolkit::System;

export namespace winrt::OneToolkit
{
	namespace Data
	{
#if __has_include(<winrt/Microsoft.UI.Xaml.Data.h>) && !force_platform_xaml
		using PropertyChangedEventArgs = Microsoft::UI::Xaml::Data::PropertyChangedEventArgs;
		using PropertyChangedEventHandler = Microsoft::UI::Xaml::Data::PropertyChangedEventHandler;
#else 
		using PropertyChangedEventArgs = Windows::UI::Xaml::Data::PropertyChangedEventArgs;
		using PropertyChangedEventHandler = Windows::UI::Xaml::Data::PropertyChangedEventHandler;
#endif

		/// @brief Provides a base class for view models and observable objects.
		template <typename Derived>
		struct Observable
		{
		public:
			event_token PropertyChanged(PropertyChangedEventHandler const& delegate)
			{
				return m_PropertyChanged.add(delegate);
			}

			void PropertyChanged(event_token token) noexcept
			{
				m_PropertyChanged.remove(token);
			}
		protected:
			Observable() = default;

			/// @brief Raises the property changed event for a specified property name.
			/// @param propertyNameThe name of the property. Shouldn't be empty or only full of whitespaces.
			void RaisePropertyChanged(hstring const& propertyName)
			{
				if (!has_only_whitespaces(propertyName))
				{
					PropertyChangedEventArgs args{ propertyName };
					m_PropertyChanged(*static_cast<Derived*>(this), args);
				}
			}

			/// @brief Compares the current and new values for a given property. If the value has changed, updates the property with the new value and raises the PropertyChanged event.
			/// @param field The field storing the property's value.
			/// @param newValue The field storing the property's value.
			/// @param propertyName The name of the property that changed.
			/// @return true if the property was changed, false otherwise.
			template <windows_runtime_type T>
			bool SetProperty(T& field, T newValue, hstring const& propertyName)
			{
				if (field != newValue)
				{
					field = newValue;
					RaisePropertyChanged(propertyName);
					return true;
				}

				return false;
			}
		private:
			event<PropertyChangedEventHandler> m_PropertyChanged;
		};
	}

	namespace UI
	{
		namespace Windowing
		{
			MIDL_INTERFACE("EF63725E-9950-4721-B57D-5E56E735B61A") IViewReferenceInterop : ::IUnknown
			{
				STDMETHOD(get_WindowHandle)(HWND* windowHandle) = 0;
			};

			struct ViewReferenceInterop
			{
				ViewReferenceInterop() = delete;

				static HWND GetWindowHandle(ViewReference const& viewReference)
				{
					HWND windowHandle;
					check_hresult(viewReference.as<IViewReferenceInterop>()->get_WindowHandle(&windowHandle));
					return windowHandle;
				}
			};
		}
	}

	namespace System
	{
		template <typename Derived>
		struct StrongSingleton
		{
		public:
			static auto Instance()
			{
				static auto instance = make<Derived>();
				return instance;
			}
		protected:
			StrongSingleton() = default;
		};

		template <typename Derived>
		struct WeakSingleton
		{
		public:
			static auto Instance()
			{
				static slim_mutex accessLock;
				static weak_ref<typename Derived::class_type> ref;
				slim_lock_guard const lockGuard{ accessLock };
				auto instance = ref.get();
				if (!instance)
				{
					instance = make<Derived>();
					ref = instance;
				}

				return instance;
			}
		protected:
			WeakSingleton() = default;
		};

		/// @brief Provides a base class to conveniently implement the IClosable interface.
		template <typename Derived>
		struct Disposable
		{
		public:
			void Close()
			{
				if (!m_IsDisposed)
				{
					static_cast<Derived&>(*this).close();
					m_IsDisposed = true;
				}
			}

			~Disposable()
			{
				Close();
			}
		protected:
			/// @brief Gets a value that indicates whether the object has been closed.
			/// @return true if this object is disposed, false otherwise.
			bool IsDisposed() const noexcept
			{
				return m_IsDisposed;
			}

			/// @brief Throws an exception if the object is already closed.
			void ThrowIfDisposed(hstring const& message = {}) const
			{
				if (m_IsDisposed) throw hresult_object_closed(message);
			}
		private:
			bool m_IsDisposed;
		};

		struct AsyncHelper
		{
		public:
			/// @brief Creates an asynchronous operation that returns a predetermined value.
			template <typename Result>
			auto CreateOperationFromResult(Result const result)
			{
				return make<AsyncOperationWithResult<Result>>(result);
			}
		private:
			template <typename Result>
			struct AsyncOperationWithResult : implements<AsyncOperationWithResult<Result>, IAsyncOperation<Result>>
			{
			public:
				AsyncOperationWithResult(Result result) : m_Result(result) {}

				auto Id() const noexcept
				{
					return as_value<uint32>(this);
				}

				auto ErrorCode() const noexcept
				{
					return hresult(0);
				}

				auto Status() const noexcept
				{
					return AsyncStatus::Completed;
				}

				auto Completed() const noexcept
				{
					return m_Completed;
				}

				void Completed(AsyncOperationCompletedHandler<Result> const& value)
				{
					value(*this, AsyncStatus::Completed);
					m_Completed = value;
				}

				auto GetResults() const noexcept
				{
					return m_Result;
				}

				void Cancel() const noexcept {}

				void Close() const noexcept {}
			private:
				Result m_Result;
				AsyncOperationCompletedHandler<Result> m_Completed;
			};
		};
	}
}