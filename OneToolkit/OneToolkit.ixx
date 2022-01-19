module;

#include "OneToolkit.h"
#include <span>
#include <memory>
#include <format>
#include <filesystem>
#include <functional>
#include <Windows.h>
#include <winrt/Windows.Storage.h>
#if __has_include(<winrt/Microsoft.UI.Xaml.Data.h>) && !force_platform_xaml
#include <winrt/Microsoft.UI.Xaml.Data.h>
#else
#include <winrt/Windows.UI.Xaml.Data.h>
#endif

export module OneToolkit;

export import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace OneToolkit::Data::Text;

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


export namespace winrt
{
	namespace Windows::Storage
	{
		inline bool operator==(IStorageItem2 const& storageItem, IStorageItem const& value) noexcept
		{
			return storageItem.IsEqual(value);
		}
	}

	namespace OneToolkit
	{
		namespace Data::Mvvm
		{
#if __has_include(<winrt/Microsoft.UI.Xaml.Data.h>) && !force_platform_xaml
			using PropertyChangedEventArgs = Microsoft::UI::Xaml::Data::PropertyChangedEventArgs;
			using PropertyChangedEventHandler = Microsoft::UI::Xaml::Data::PropertyChangedEventHandler;
#else 
			using PropertyChangedEventArgs = Windows::UI::Xaml::Data::PropertyChangedEventArgs;
			using PropertyChangedEventHandler = Windows::UI::Xaml::Data::PropertyChangedEventHandler;
#endif

			template <typename K, typename T>
			concept Raiser = requires(K key, T instance)
			{
				{ instance.Raise(key) };
			};

			/// <summary>
			/// Provides a base class for view models and observable objects.
			/// </summary>
			template <typename Derived>
			struct ObservableBase
			{
			public:
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
			protected:
				/// <summary>
				/// Creates a new instance of ObservableBase from a derived class. 
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
					if (!has_only_whitespaces(propertyName) && Decide(propertyName) && !m_SuppressEvents)
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
				template <rt_type T>
				bool SetProperty(T& field, T newValue, hstring const& propertyName)
				{
					if (field != newValue)
					{
						field = newValue;
						Raise(propertyName);
						return true;
					}

					return false;
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
			/// Provides a property that stores a field internally and raises the property changed event when its value is changed.
			/// </summary>
			template <rt_type T, Raiser<hstring> P>
			struct ObservableProperty final
			{
			public:
				ObservableProperty(P const& parent, hstring const& propertyName, T defaultValue = {}) : PropertyName(propertyName), m_Parent(parent), m_BackingField(defaultValue) {}

				ObservableProperty(ObservableProperty&&) = delete;

				ObservableProperty(ObservableProperty const&) = delete;

				auto_property<hstring> const PropertyName;

				T operator()() const noexcept
				{
					return m_BackingField;
				}

				void operator()(T newValue)
				{
					if (m_BackingField != newValue)
					{
						m_BackingField = newValue;
						m_Parent.Raise(PropertyName());
					}
				}
			private:
				P& m_Parent;
				T m_BackingField;
			};
		}

		namespace System
		{
			/// <summary>
			/// Indicates the method used to open a dynamic link library.
			/// </summary>
			enum class LibraryOpenOptions : uint8
			{
				OpenGlobal, OpenPackaged, OpenExisting
			};

			/// <summary>
			/// Represents a dynamic link library and enables the ability to use exported functions or variables.
			/// </summary>
			struct DynamicLibrary
			{
			public:
				DynamicLibrary(hstring const& fileName, LibraryOpenOptions openOptions = LibraryOpenOptions::OpenGlobal) : m_FileName(fileName), m_OpenOptions(openOptions)
				{
					Handle(OpenModule(fileName, openOptions));
				}

				DynamicLibrary(DynamicLibrary const& another)
				{
					if (another != *this)
					{
						m_FileName = another.m_FileName;
						m_OpenOptions = another.m_OpenOptions;
						Handle(OpenModule(another.m_FileName, another.m_OpenOptions));
					}
				}

				DynamicLibrary(DynamicLibrary&& another) noexcept
				{
					if (another != *this)
					{
						Handle(another.m_Handle);
						another.m_Handle = nullptr;
						m_FileName = another.m_FileName;
						m_OpenOptions = another.m_OpenOptions;
					}
				}

				/// <summary>
				/// Gets the handle to the loaded dynamic link library.
				/// </summary>
				auto Handle() const noexcept
				{
					return m_Handle;
				}

				/// <summary>
				/// Gets the file name of the dynamic link library.
				/// </summary>
				auto FileName() const noexcept
				{
					return m_FileName;
				}

				bool operator==(DynamicLibrary const& another) const noexcept
				{
					return m_Handle == another.m_Handle;
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

				static void* OpenModule(hstring const& fileName, LibraryOpenOptions openOptions = LibraryOpenOptions::OpenGlobal)
				{
					if (openOptions == LibraryOpenOptions::OpenGlobal) return WINRT_IMPL_LoadLibraryW(fileName.data());
					else if (openOptions == LibraryOpenOptions::OpenPackaged) return LoadPackagedLibrary(fileName.data(), 0);
					else return GetModuleHandleW(fileName.data());
				}
			};
		}

		namespace Storage
		{
			template <typename T, ApplicationDataLocality Store>
			struct AppSettingProperty
			{
				AppSettingProperty(hstring const& propertyName, std::function<T()> defaultValueProvider)
				{

				}

				AppSettingProperty(AppSettingProperty&&) = delete;

				AppSettingProperty(AppSettingProperty const&&) = delete;
			};

			struct FileLanguageInfo
			{
				uint16 LanguageId;

				uint16 CodePage;

				std::wstring StringTableId() const
				{
					return std::format(L"{:04x}{:04x}", LanguageId, CodePage);
				}
			};

			struct FileVersionInfo
			{
			public:
				FileVersionInfo(std::filesystem::path const& executableFilePath)
				{
					auto const versionInfoSize = GetFileVersionInfoSizeW(executableFilePath.c_str(), nullptr);
					check_bool(versionInfoSize);
					m_InfoBuffer = std::make_shared<uint8[]>(versionInfoSize);
				}

				std::span<FileLanguageInfo const> Translations() const noexcept
				{
					uint32 languageInfoSize;
					FileLanguageInfo* languageInfo;
					if (VerQueryValueW(m_InfoBuffer.get(), L"\\VarFileInfo\\Translation", reinterpret_cast<void**>(&languageInfo), &languageInfoSize) && languageInfo)
					{
						return { languageInfo, languageInfoSize / sizeof(FileLanguageInfo) };
					}
					else
					{
						return {};
					}
				}

				std::wstring_view GetStringResource(std::wstring_view key, FileLanguageInfo const languageInfo) const noexcept
				{
					wchar_t* text;
					uint32 textSize;
					if (VerQueryValueW(m_InfoBuffer.get(), std::format(L"\\StringFileInfo\\{}\\{}", languageInfo.StringTableId(), key).data(), reinterpret_cast<void**>(&text), &textSize) && text)
					{
						return { text, textSize };
					}
					else
					{
						return {};
					}
				}

				static FileVersionInfo GetFromStorageFile(StorageFile const& storageFile)
				{
					return { storageFile.Path().data() };
				}
			private:
				std::shared_ptr<uint8[]> m_InfoBuffer;
			};
		}

		namespace Lifecycle
		{
			/// <summary>
			/// Provides a base class to conveniently implement the IEquatable interface.
			/// </summary>
			/// <remarks>Your derived class must provide a public const equals operator that takes in an IInspectable.
			template <typename Derived>
			struct Equatable
			{
			public:
				bool Equals(IInspectable const& another) const noexcept
				{
					return *static_cast<Derived const* const>(this) == another;
				}
			protected:
				Equatable() = default;
			};

			/// <summary>
			/// Provides a base class to conveniently implement the IComparable interface.
			/// </summary>
			/// <remarks>Your derived class must provide a public const spaceship operator that takes in an IInspectable.
			template <typename Derived>
			struct Comparable
			{
			public:
				int CompareTo(IInspectable const& another) const noexcept
				{
					return *static_cast<Derived const* const>(this) <=> another;
				}
			protected:
				Comparable() = default;
			};

			/// <summary>
			/// Provides a base class to conveniently implement the IClosable interface.
			/// </summary>
			/// <remarks>Your derived class must provide a public Dispose method.</remarks>
			template <typename Derived>
			struct Disposable
			{
			public:
				/// <summary>
				/// Releases system resources that are exposed by a Windows Runtime object.
				/// </summary>
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

				/// <summary>
				/// Throws an exception if the object is closed.
				/// </summary>
				inline void ThrowIfDisposed() const
				{
					if (m_IsDisposed) throw hresult_object_closed();
				}
			private:
				bool m_IsDisposed;
			};

			/// <summary>
			/// Provides a base class to conveniently implement the SuspendableBase interface.
			/// </summary>
			template <typename Derived>
			struct SuspendableBase
			{
			public:
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
				SuspendableBase() = default;

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
			/// Creates an asynchronous operation that returns a pre-determined value.
			/// </summary>
			/// <param name="value">The pre-determined value that will be returned from the async operation.</param>
			/// <returns>A reference to an object that implements the IAsyncOperation interface.</returns>
			template <rt_type Result>
			auto AsyncOperationFromResult(Result value)
			{
				return make<AsyncOperationWithResult<Result>>(value);
			}

			inline bool operator==(IEquatable const& equatable, IInspectable const& value) noexcept
			{
				if (equatable) return equatable.Equals(value);
				else return !value;
			}


			inline int operator<=>(IComparable const& comparable, IInspectable const& value) noexcept
			{
				return comparable.CompareTo(value);
			}
		}
	}
}