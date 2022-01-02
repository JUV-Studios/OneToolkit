module;

#define onetoolkit_ixx
#include "OneToolkit.h"
#include <Windows.h>
#include <CorError.h>

export module OneToolkit;


using namespace juv;
using namespace winrt;
using namespace winrt::impl;
using namespace AppFramework::Data;
using namespace Windows::Foundation;
using namespace OneToolkit::Data::Text;

export namespace winrt::OneToolkit
{
	/// <summary>
	/// Represents a type which can be passed through the Windows Runtime ABI.
	/// </summary>
	template <typename T>
	concept WindowsRuntimeType = has_category_v<T>;

	namespace Runtime
	{
		/// <summary>
		/// Indicates the method used to open a dynamic link library.
		/// </summary>
		enum class LibraryOpenOptions : uint8
		{
			OpenGlobal, OpenPackaged, OpenExisting
		};

		struct TypeDetails
		{
		public:

		private:
			Windows::UI::Xaml::Interop::TypeName m_TypeName;
		};

		/// <summary>
		/// Represents a dynamic link library and enables the ability to use exported functions or variables.
		/// </summary>
		struct DynamicLibrary
		{
		public:
			/// <summary>
			/// Creates a new instance of DynamicLibrary from a DLL file name.
			/// </summary>
			/// <param name="fileName">The file name of the DLL that you want to load into the process.</param>
			/// <param name="isPackagedLibrary">Indicates the method used to open the specified DLL.</param>
			DynamicLibrary(hstring const& fileName, LibraryOpenOptions openOptions = LibraryOpenOptions::OpenGlobal) : m_FileName(fileName), m_OpenOptions(openOptions)
			{
				Handle(OpenModule(fileName, openOptions));
			}

			/// <summary>
			/// Creates a new instance of DynamicLibrary that's a copy of an existing instance.
			/// </summary>
			/// <param name="another">The existing instance to copy from.</param>
			DynamicLibrary(DynamicLibrary const& another)
			{
				Copy(another);
			}

			/// <summary>
			/// Creates a new instance instance of DynamicLibrary by moving an existing instance.
			/// </summary>
			/// <param name="another">The existing instance to move from.</param>
			DynamicLibrary(DynamicLibrary&& another) noexcept
			{
				Move(std::move(another));
			}

			/// <summary>
			/// Gets the handle to the loaded DLL.
			/// </summary>
			auto Handle() const noexcept
			{
				return m_Handle;
			}

			/// <summary>
			/// Gets the file name of the DLL.
			/// </summary>
			auto FileName() const noexcept
			{
				return m_FileName;
			}

			DynamicLibrary& operator=(const DynamicLibrary& another)
			{
				Copy(another);
				return *this;
			}

			DynamicLibrary& operator=(DynamicLibrary&& another) noexcept
			{
				Move(std::move(another));
				return *this;
			}

			bool operator==(DynamicLibrary const& another) const noexcept
			{
				return m_Handle == another.m_Handle;
			}

			bool operator!=(DynamicLibrary const& another) const noexcept
			{
				return !operator==(another);
			}

			/// <summary>
			/// Retrieves the address of an exported function or variable.
			/// </summary>
			template <typename Pointer>
			auto GetProcAddress(std::string_view procName) const requires std::is_pointer_v<Pointer>
			{
				auto result = WINRT_IMPL_GetProcAddress(m_Handle, procName.data());
				if (!result) throw_last_error();
				return reinterpret_cast<Pointer>(result);
			}

			~DynamicLibrary()
			{
				if (m_Handle && m_OpenOptions != LibraryOpenOptions::OpenExisting) WINRT_IMPL_FreeLibrary(m_Handle);
			}
		private:
			void* m_Handle;

			hstring m_FileName;

			LibraryOpenOptions m_OpenOptions;

			void Handle(void* newHandle)
			{
				if (newHandle) m_Handle = newHandle;
				else throw_last_error();
			}

			void Copy(DynamicLibrary const& another)
			{
				if (another != *this)
				{
					m_FileName = another.m_FileName;
					m_OpenOptions = another.m_OpenOptions;
					Handle(OpenModule(another.m_FileName, another.m_OpenOptions));
				}
			}

			void Move(DynamicLibrary&& another) noexcept
			{
				if (another != *this)
				{
					Handle(another.m_Handle);
					another.m_Handle = nullptr;
					m_FileName = another.m_FileName;
					m_OpenOptions = another.m_OpenOptions;
				}
			}

			static void* OpenModule(hstring const& fileName, LibraryOpenOptions openOptions = LibraryOpenOptions::OpenGlobal)
			{
				if (openOptions == LibraryOpenOptions::OpenGlobal) return WINRT_IMPL_LoadLibraryW(fileName.data());
				else if (openOptions == LibraryOpenOptions::OpenPackaged) return LoadPackagedLibrary(fileName.data(), 0);
				else return GetModuleHandleW(fileName.data());
			}
		};
	}

	namespace Data::Mvvm
	{
		/// <summary>
		/// Provides a base struct for view models and observable objects.
		/// </summary>
		template <typename Derived>
		struct ObservableBase
		{
		public:
			ObservableBase(ObservableBase&&) = delete;

			ObservableBase(ObservableBase const&) = delete;

			/// <summary>
			/// Gets whether the property changed event will be raised or not.
			/// </summary>
			bool SuppressEvents() const noexcept
			{
				return m_SuppressEvents;
			}

			event_token PropertyChanged(PropertyChangedEventHandler const& delegate)
			{
				return m_PropertyChanged.add(delegate);
			}

			void PropertyChanged(event_token token) noexcept
			{
				m_PropertyChanged.remove(token);
			}

			/// <summary>
			/// Automatically sets a property value and raises the property changed event through an user provided raiser.
			/// </summary>
			/// <returns>True if the value was set, false if the value passed was equal to the existing value.</returns>
			template <WindowsRuntimeType T>
			static bool SetProperty(T& field, T newValue, hstring const& propertyName, std::function<void(hstring const&)> raiser)
			{
				if (field != newValue)
				{
					field = newValue;
					raiser(propertyName);
					return true;
				}

				return false;
			}
		protected:
			/// <summary>
			/// Creates a new instance of ObservableBase from a derived struct. 
			/// </summary>
			/// <param name="suppressEvents">Optional initial value for the SuppressEvents property.</param>
			ObservableBase(bool suppressEvents = false) : m_SuppressEvents(suppressEvents)
			{
			}

			/// <summary>
			/// Sets whether the property changed event will be raised or not.
			/// </summary>
			void SuppressEvents(bool value) noexcept
			{
				m_SuppressEvents = value;
			}

			/// <summary>
			/// Gets a delegate that can be used to raise the property changed on this object.
			/// </summary>
			/// <returns>A delegate that references the Raise member function.</returns>
			std::function<void(hstring const&)> Raiser()
			{
				return std::bind(&ObservableBase<Derived>::Raise, this, std::placeholders::_1);
			}

			/// <summary>
			/// Raises the property changed event for a specified property name.
			/// </summary>
			/// <param name="propertyName">The name of the property. Shouldn't be empty or only full of whitespaces.</param>
			void Raise(hstring const& propertyName)
			{
				if (!StringExtensions::HasOnlyWhitespaces(propertyName) && Decide(propertyName) && !m_SuppressEvents)
				{
					PropertyChangedEventArgs args{ propertyName };
					m_PropertyChanged(*static_cast<Derived*>(this), args);
					WhenPropertyChanged(args);
				}
			}

			/// <summary>
			/// Automatically sets a property value and raises property changed when required.
			/// </summary>
			/// <returns>True if the value was set, false if the value passed was equal to the existing value.</returns>
			template <WindowsRuntimeType T>
			bool SetProperty(T& field, T newValue, hstring const& propertyName)
			{
				return SetProperty(field, newValue, propertyName, std::bind(&ObservableBase<Derived>::Raise, this, std::placeholders::_1));
			}

			/// <summary>
			/// Override this method to determine whether to raise property changed or not.
			/// </summary>
			virtual bool Decide(hstring const&) noexcept
			{
				return true;
			}

			/// <summary>
			/// Override this method to perform custom actions after raising the property changed event.
			/// </summary>
			virtual void WhenPropertyChanged(PropertyChangedEventArgs const&)
			{
				// Do nothing here.
			}
		private:
			bool m_SuppressEvents;
			event<PropertyChangedEventHandler> m_PropertyChanged;
		};

		/// <summary>
		/// Provides a property that stores a field internally and raises the property changed event when changed.
		/// </summary>
		template <WindowsRuntimeType T>
		struct ObservableProperty : property<T>
		{
		public:
			ObservableProperty(hstring const& propertyName, std::function<void(hstring const&)> raiser, T defaultValue = {}) : property<T>(std::bind(&ObservableProperty<T>::get, this), std::bind(&ObservableProperty::set, this, std::placeholders::_1)), m_PropertyName(propertyName), m_Raiser(raiser), m_BackingField(defaultValue) {}

			/// <summary>
			/// Gets the name of this observable property.
			/// </summary>
			/// <returns>The name of this observable property, used for raising the property changed event.</returns>
			hstring PropertyName() const noexcept
			{
				return m_PropertyName;
			}

			/// <summary>
			/// Gets the delegate that's used to raise the property changed event.
			/// </summary>
			/// <returns>The delegate that's used to raise the property changed event.</returns>
			std::function<void(hstring const&)> Raiser() const noexcept
			{
				return m_Raiser;
			}
		private:
			T m_BackingField;

			hstring m_PropertyName;

			std::function<void(hstring const&)> m_Raiser;

			T get() const noexcept
			{
				return m_BackingField;
			}

			void set(T newValue)
			{
				if (m_BackingField != newValue)
				{
					m_BackingField = newValue;
					m_Raiser(m_PropertyName);
				}
			}
		};
	}

	namespace Lifecycle
	{
		/// <summary>
		/// Provides a base struct to conveniently implement the IClosable interface.
		/// </summary>
		/// <remarks>Your derived struct must provide a Dispose method which will be called by Close when the object hasn't been closed yet.</remarks>
		template <typename Derived>
		struct Disposable
		{
		public:
			Disposable(Disposable&&) = delete;

			Disposable(Disposable const&) = delete;

			/// <summary>
			/// Releases system resources that are exposed by a Windows Runtime object.
			/// </summary>
			void Close()
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

			/// <summary>
			/// Throws an exception if the object is closed.
			/// </summary>
			inline void ThrowIfDisposed() const
			{
				if (m_IsDisposed) throw hresult_error(RO_E_CLOSED);
			}
		private:
			bool m_IsDisposed;
		};

		/// <summary>
		/// Provides a base struct to conveniently implement the ISuspendable interface.
		/// </summary>
		template <typename Derived>
		struct Suspendable
		{
		public:
			Suspendable(Suspendable&&) = delete;

			Suspendable(Suspendable const&) = delete;

			/// <summary>
			/// Gets a value that indicates whether the current object is suspended.
			/// </summary>
			bool IsSuspended() const noexcept
			{
				return m_IsSuspended;
			}

			event_token StateChanged(Windows::Foundation::TypedEventHandler<ISuspendable, SuspendableEventType> const& handler)
			{
				return m_StateChanged.add(handler);
			}

			void StateChanged(event_token token) noexcept
			{
				m_StateChanged.remove(token);
			}
		protected:
			Suspendable() = default;

			/// <summary>
			/// Toggles the suspended state and raises the StateChanged event.	
			/// </summary>
			void ToggleState()
			{
				if (m_IsSuspended)
				{
					m_IsSuspended = false;
					m_StateChanged(*static_cast<Derived*>(this), SuspendableEventType::Resumed);
				}
				else
				{
					m_IsSuspended = true;
					m_StateChanged(*static_cast<Derived*>(this), SuspendableEventType::Suspended);
				}
			}
		private:
			bool m_IsSuspended;
			event<Windows::Foundation::TypedEventHandler<ISuspendable, SuspendableEventType>> m_StateChanged;
		};

		/// <summary>
		/// Represents an asynchronous operation that returns the result immediately when invoked.
		/// </summary>
		/// <typeparam name="Result">The type returned by the asynchronous operation.</typeparam>
		template <typename Result>
		struct AsyncOperationWithResult : implements<AsyncOperationWithResult<Result>, IAsyncOperation<Result>>
		{
		public:
			AsyncOperationWithResult(Result result) : m_Result(result) {}

			/// <summary>
			/// Gets the handle of the asynchronous operation.
			/// </summary>
			/// <returns>The handle of the asynchronous operation.</returns>
			auto Id() const noexcept
			{
				return as_value<uint32>(this);
			}

			/// <summary>
			/// Gets a string that describes an error condition of the asynchronous operation.
			/// </summary>
			/// <returns>The error string.</returns>
			auto ErrorCode() const noexcept
			{
				return hresult(0);
			}

			/// <summary>
			/// Gets a value that indicates the status of the asynchronous operation.
			/// </summary>
			/// <returns>The status of the operation, as a value of the enumeration.</returns>
			auto Status() const noexcept
			{
				return AsyncStatus::Completed;
			}

			/// <summary>
			/// Gets the method that handles the operation completed notification.
			/// </summary>
			/// <returns>The method that handles the notification.</returns>
			auto Completed() const noexcept
			{
				return m_Completed;
			}

			/// <summary>
			/// Sets the method that handles the operation completed notification.
			/// </summary>
			void Completed(AsyncOperationCompletedHandler<Result> const& value)
			{
				value(*this, AsyncStatus::Completed);
				m_Completed = value;
			}

			/// <summary>
			/// Returns the results of the operation.
			/// </summary>
			/// <returns>The results of the operation.</returns>
			auto GetResults() const noexcept
			{
				return m_Result;
			}

			/// <summary>
			/// Cancels the asynchronous operation.
			/// </summary>
			void Cancel() const noexcept {}

			/// <summary>
			/// Closes the asynchronous operation.
			/// </summary>
			void Close() const noexcept {}
		private:
			Result m_Result;
			AsyncOperationCompletedHandler<Result> m_Completed;
		};
	}
}