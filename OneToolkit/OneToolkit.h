// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++ projects targeting the Windows platform.

#pragma once
#ifdef __cplusplus_winrt
#include <type_traits>

namespace juv
{
	template <typename T>
	struct remove_hat
	{
		using type = T;
	};

	template <typename T>
	struct remove_hat<T^>
	{
		using type = typename remove_hat<T>::type;
	};
}

#else
#include <juv.h>
#include <concepts>
#include <functional>
#include <unknwn.h>
#include <Windows.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/OneToolkit.System.h>
#include <winrt/OneToolkit.Storage.h>
#include <winrt/OneToolkit.Lifecycle.h>
#include <winrt/OneToolkit.UI.Input.h>
#include <winrt/OneToolkit.UI.Converters.h>
#include <winrt/OneToolkit.Media.Imaging.h>

#define DeclareEvent(Type, Name) private: ::winrt::event<Type> m_##Name;\
public: ::winrt::event_token Name(Type const& handler) { return m_##Name.add(handler); }\
void Name(::winrt::event_token token) noexcept { m_##Name.remove(token); }

#define DeclareAutoProperty(Type, Name, DefaultValue) private: Type m_##Name = DefaultValue;\
public: Type Name() const noexcept { return m_##Name; }\
void Name(Type value) noexcept { m_##Name = value; }

#define DeclareObservableProperty(Type, Name, DefaultValue) private: Type m_##Name = DefaultValue;\
public: Type Name() const noexcept { return m_##Name; }\
void Name(Type value) { SetProperty<Type>(m_##Name, value, L#Name); }

namespace winrt::OneToolkit
{
	/// <summary>
	/// Represents a type which can be passed through the Windows Runtime ABI.
	/// </summary>
	template <typename T>
	concept WindowsRuntimeType = winrt::impl::has_category_v<T>;

	namespace Runtime
	{
		inline auto GetTypeId(hstring const& typeName)
		{
			Windows::UI::Xaml::Interop::TypeName result;
			result.Name = typeName;
			result.Kind = std::find(result.Name.begin(), result.Name.end(), L'.') == result.Name.end() ? Windows::UI::Xaml::Interop::TypeKind::Primitive : Windows::UI::Xaml::Interop::TypeKind::Metadata;
			return result;
		}

		struct TypeDetails
		{
		public:
			TypeDetails(hstring const& typeName) : TypeDetails(GetTypeId(typeName)) {}

			TypeDetails(Windows::UI::Xaml::Interop::TypeName const& typeId) : m_TypeId(typeId)
			{
				std::wstring_view typeNameView = typeId.Name;
				m_Name = typeNameView.substr(typeNameView.find_last_of(L'.') + 1);
				m_Namespace = typeNameView.substr(0, typeNameView.find_last_of(L'.') - 1);
			}

			std::wstring_view Name() const noexcept
			{
				return m_Name;
			}

			std::wstring_view Namespace() const noexcept
			{
				return m_Namespace;
			}
			
			std::wstring_view QualifiedName() const noexcept
			{
				return m_TypeId.Name;
			}

			Windows::UI::Xaml::Interop::TypeKind Kind() const noexcept
			{
				return m_TypeId.Kind;
			}

			size_t BaseSize() const
			{
				if (m_TypeId == xaml_typename<bool>()) return sizeof(bool);
				else if (m_TypeId == xaml_typename<juv::char16>()) return sizeof(juv::char16);
				else if (m_TypeId == xaml_typename<juv::uint8>()) return sizeof(juv::uint8);
				else if (m_TypeId == xaml_typename<juv::int8>()) return sizeof(juv::int8);
				else if (m_TypeId == xaml_typename<juv::uint16>()) return sizeof(juv::uint16);
				else if (m_TypeId == xaml_typename<juv::int16>()) return sizeof(juv::int16);
				else if (m_TypeId == xaml_typename<juv::uint32>()) return sizeof(juv::uint32);
				else if (m_TypeId == xaml_typename<juv::int32>()) return sizeof(juv::int32);
				else if (m_TypeId == xaml_typename<juv::uint64>()) return sizeof(juv::uint64);
				else if (m_TypeId == xaml_typename<juv::int64>()) return sizeof(juv::int64);
				else if (m_TypeId == xaml_typename<float>()) return sizeof(float);
				else if (m_TypeId == xaml_typename<double>()) return sizeof(double);
				else if (m_TypeId == xaml_typename<hstring>()) return sizeof(hstring);
				else if (m_TypeId == xaml_typename<guid>()) return sizeof(guid);
				else if (m_TypeId == xaml_typename<Windows::Foundation::Point>()) return sizeof(Windows::Foundation::Point);
				else if (m_TypeId == xaml_typename<Windows::Foundation::Size>()) return sizeof(Windows::Foundation::Size);
				else if (m_TypeId == xaml_typename<Windows::Foundation::Rect>()) return sizeof(Windows::Foundation::Rect);
				else if (m_TypeId == xaml_typename<Windows::Foundation::DateTime>()) return sizeof(Windows::Foundation::DateTime);
				else if (m_TypeId == xaml_typename<Windows::Foundation::TimeSpan>()) return sizeof(Windows::Foundation::TimeSpan);
				else return sizeof(size_t); // TODO Calculate type's size from metadata if it's a value type.
			}

			operator Windows::UI::Xaml::Interop::TypeName() const noexcept
			{
				return m_TypeId;
			}

			Windows::Foundation::IInspectable CreateInstance() const
			{
				if (m_TypeId == xaml_typename<bool>()) return box_value(bool());
				else if (m_TypeId == xaml_typename<juv::char16>()) return box_value(juv::char16());
				else if (m_TypeId == xaml_typename<juv::uint8>()) return box_value(juv::uint8());
				else if (m_TypeId == xaml_typename<juv::int8>()) return box_value(juv::int8());
				else if (m_TypeId == xaml_typename<juv::uint16>()) return box_value(juv::uint16());
				else if (m_TypeId == xaml_typename<juv::int16>()) return box_value(juv::int16());
				else if (m_TypeId == xaml_typename<juv::uint32>()) return box_value(juv::uint32());
				else if (m_TypeId == xaml_typename<juv::int32>()) return box_value(juv::int32());
				else if (m_TypeId == xaml_typename<juv::uint64>()) return box_value(juv::uint64());
				else if (m_TypeId == xaml_typename<juv::int64>()) return box_value(juv::int64());
				else if (m_TypeId == xaml_typename<float>()) return box_value(float());
				else if (m_TypeId == xaml_typename<double>()) return box_value(double());
				else if (m_TypeId == xaml_typename<hstring>()) return box_value(hstring());
				else if (m_TypeId == xaml_typename<guid>()) return box_value(guid());
				else if (m_TypeId == xaml_typename<Windows::Foundation::Point>()) return box_value(Windows::Foundation::Point());
				else if (m_TypeId == xaml_typename<Windows::Foundation::Size>()) return box_value(Windows::Foundation::Size());
				else if (m_TypeId == xaml_typename<Windows::Foundation::Rect>()) return box_value(Windows::Foundation::Rect());
				else if (m_TypeId == xaml_typename<Windows::Foundation::DateTime>()) return box_value(Windows::Foundation::DateTime());
				else if (m_TypeId == xaml_typename<Windows::Foundation::TimeSpan>()) return box_value(Windows::Foundation::TimeSpan());
				else
				{
					// TODO Implementing creating a value type defined in metadata.
				}
			}

			template <typename Result>
			auto CreateInstance()
			{
				return CreateInstance().try_as<Result>();
			}

			static auto GetTypeOf(Windows::Foundation::IInspectable const& value)
			{
				return TypeDetails(get_class_name(value));
			}
		private:
			std::wstring_view m_Name;
			std::wstring_view m_Namespace;
			Windows::UI::Xaml::Interop::TypeName m_TypeId;
		};

		/// <summary>
		/// Represents a dynamic link library and enables the ability to use exported functions or variables.
		/// </summary>
		struct DynamicLibrary
		{
		public:
			DynamicLibrary(hstring const& fileName, bool isPackagedLibrary = false) : m_FileName(fileName), m_IsPackagedLibrary(isPackagedLibrary)
			{
				Handle(isPackagedLibrary ? LoadPackagedLibrary(fileName.data(), 0) : WINRT_IMPL_LoadLibraryW(fileName.data()));
			}

			DynamicLibrary(DynamicLibrary const& another)
			{
				Copy(another);
			}

			DynamicLibrary(DynamicLibrary&& another) noexcept
			{
				Move(std::move(another));
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
				return m_FileName == another.m_FileName && m_IsPackagedLibrary == another.m_IsPackagedLibrary && m_Handle == another.m_Handle;
			}

			bool operator!=(DynamicLibrary const& another) const noexcept
			{
				return !operator==(another);
			}

			/// <summary>
			/// Returns an handle to the loaded dynamic link library.
			/// </summary>
			auto Handle() const noexcept
			{
				return m_Handle;
			}

			/// <summary>
			/// Returns the file name of the DLL.
			/// </summary>
			const auto FileName() const noexcept
			{
				return m_FileName;
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
				if (m_Handle) WINRT_IMPL_FreeLibrary(m_Handle);
			}
		private:
			HMODULE m_Handle;

			hstring m_FileName;

			bool m_IsPackagedLibrary;

			void Handle(void* newHandle)
			{
				if (!newHandle) throw_last_error();
				m_Handle = static_cast<HMODULE>(newHandle);
			}

			void Copy(DynamicLibrary const& another)
			{
				Handle(another.m_IsPackagedLibrary ? LoadPackagedLibrary(another.m_FileName.data(), 0) : WINRT_IMPL_LoadLibraryW(another.m_FileName.data()));
				m_FileName = another.m_FileName;
				m_IsPackagedLibrary = another.m_IsPackagedLibrary;

			}

			void Move(DynamicLibrary&& another) noexcept
			{
				Handle(another.m_Handle);
				another.m_Handle = nullptr;
				m_FileName = another.m_FileName;
				m_IsPackagedLibrary = another.m_IsPackagedLibrary;
			}
		};
	}

	namespace System
	{
		/// <summary>
		/// Provides the ability to find about and communicate with the debugger.
		/// </summary>
		namespace Debugger
		{
			/// <summary>
			/// Signals a breakpoint to an attached debugger for the current process.
			/// </summary>
			inline void Break() noexcept
			{
				DebugBreak();
			}

			/// <summary>
			/// Gets whether a debugger is attached to the current process.
			/// </summary>
			inline bool IsAttached() noexcept
			{
				return IsDebuggerPresent() != 0;
			}

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_SYSTEM)
			/// <summary>
			/// Signals a breakpoint to an attached debugger for the specified process using its handle.
			/// </summary>
			inline void Break(HANDLE processHandle)
			{
				check_bool(DebugBreakProcess(processHandle));
			}

			/// <summary>
			/// Gets whether a debugger is attached to a specified process using its handle.
			/// </summary>
			inline bool IsAttached(HANDLE processHandle)
			{
				int result;
				check_bool(CheckRemoteDebuggerPresent(processHandle, &result));
				return result != false;
			}
#endif

			/// <summary>
			/// Writes text to the output window.
			/// </summary>
			/// <remarks>The specified string must be null terminated.</remarks>
			inline void Write(std::string_view text)
			{
				OutputDebugStringA(text.data());
			}

			/// <summary>
			/// Writes text to the output window.
			/// </summary>
			/// <remarks>The specified string must be null terminated.</remarks>
			inline void Write(std::wstring_view text)
			{
				OutputDebugStringW(text.data());
			}

			/// <summary>
			/// Writes text to the output window.
			/// </summary>
			/// <remarks>The specified string must be null terminated.</remarks>
			inline void Write(std::u8string_view text)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(text.data()));
			}

			/// <summary>
			/// Writes text to the output window.
			/// </summary>
			/// <remarks>The specified string must be null terminated.</remarks>
			inline void Write(std::u16string_view text)
			{
				OutputDebugStringW(reinterpret_cast<const wchar_t*>(text.data()));
			}

			/// <summary>
			/// Writes text to the output window.
			/// </summary>
			/// <remarks>The specified string must be null terminated.</remarks>
			inline void Write(std::u32string_view text)
			{
			}

			/// <summary>
			/// Writes a line to the output window.
			/// </summary>
			template <typename String>
			inline void WriteLine(String& line, Data::Text::LineEnding lineEnding = Data::Text::LineEnding::LF)
			{
				auto newLine = Data::Text::LineEndingHelper::GetNewLineString<typename String::value_type>(lineEnding).data();
				Write(line + newLine);
			}
		}
	}

	namespace Mvvm
	{
		template <typename Args>
		concept PropertyChangedArgs = std::is_constructible_v<Args, hstring>;

		template <typename Delegate, typename Args>
		concept PropertyChangedDelegate = std::is_invocable_r_v<void, Delegate, Windows::Foundation::IInspectable, Args>;

		/// <summary>
		/// Provides a base struct for view models and observable objects.
		/// </summary>
		template <typename Derived, PropertyChangedArgs ChangedArgs = Windows::UI::Xaml::Data::PropertyChangedEventArgs, PropertyChangedDelegate<ChangedArgs> ChangedDelegate = Windows::UI::Xaml::Data::PropertyChangedEventHandler>
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

			DeclareEvent(ChangedDelegate, PropertyChanged);

			/// <summary>
			/// Automatically sets a property value and raises the property changed event through an user provided raiser.
			/// </summary>
			/// <returns>True if the value was set, false if the value passed was equal to the existing value.</returns>
			template <WindowsRuntimeType T>
			static bool SetProperty(T& field, T newValue, hstring const& propertyName, std::function<void(hstring)> raiser)
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
			/// Raises the property changed event for a specified property name.
			/// </summary>
			/// <param name="propertyName">The name of the property. Shouldn't be empty or only full of whitespaces.</param>
			void Raise(hstring const& propertyName)
			{
				if (!juv::has_only_whitespaces(propertyName) && Decide(propertyName) && !m_SuppressEvents)
				{
					ChangedArgs args{ propertyName };
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
				return SetProperty(field, newValue, propertyName, [this](hstring const& propertyName)
					{
						Raise(propertyName);
					});
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
			virtual void WhenPropertyChanged(ChangedArgs const&)
			{
				// Do nothing here.
			}
		private:
			bool m_SuppressEvents;
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
			/// Gets whether the current object is suspended.
			/// </summary>
			bool IsSuspended() const noexcept
			{
				return m_IsSuspended;
			}

			DeclareEvent(SuspendableStateChangedEventHandler, StateChanged);
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
		};

		template <typename Result>
		struct AsyncOperationWrapper : implements<AsyncOperationWrapper<Result>, Windows::Foundation::IAsyncOperation<Result>>
		{
		public:
			AsyncOperationWrapper(Result result) : m_Result(result) {}

			auto Id() const noexcept
			{
				return juv::as_value<juv::uint32>(this);
			}

			auto ErrorCode() const noexcept
			{
				return hresult(0);
			}

			auto Status() const noexcept
			{
				return Windows::Foundation::AsyncStatus::Completed;
			}

			auto Completed() const noexcept
			{
				return m_Completed;
			}

			void Completed(Windows::Foundation::AsyncOperationCompletedHandler<Result> const& value)
			{
				value(*this, Windows::Foundation::AsyncStatus::Completed);
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
			Windows::Foundation::AsyncOperationCompletedHandler<Result> m_Completed;
		};
	}

	namespace UI::Converters
	{
		template <typename Derived>
		struct TwoWayConverter
		{
			Windows::Foundation::IInspectable Convert(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language)
			{
				return static_cast<Derived*>(this)->ConvertValue(value, targetType, parameter, language);
			}

			Windows::Foundation::IInspectable ConvertBack(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language)
			{
				return static_cast<Derived*>(this)->ConvertValue(value, targetType, parameter, language);
			}
		};
	}
}

#endif