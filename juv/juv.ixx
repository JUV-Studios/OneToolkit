module;

#include "debug-trap.h"
#include <string_view>
#include <cstdint>
#include <memory>
#include <concepts>
#include <optional>
#include <algorithm>
#include <type_traits>
#ifdef _WIN32
#include <Unknwn.h>
#include <Debugapi.h>
#include <winrt/Windows.Foundation.h>
#elif __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

export module juv;

export namespace juv
{
#ifndef UNICODE
	using charN = char;
#else
	using charN = wchar_t;
#endif

	/// @brief Represents an 8 bit character suitable for storing UTF-8 code units.
	using char8 = char8_t;

	/// @brief Represents a 16 bit character suitable for storing UTF-16 code units.
	using char16 = char16_t;

	/// @brief Represents a 32 bit character suitable for storing UTF-32 code units.
	using char32 = char32_t;

	/// @brief Represents a platform-specific signed integer.
	using intN = intptr_t;

	/// @brief Represents a platform-specific unsigned integer.
	using uintN = uintptr_t;

	/// @brief Represents an 8 bit signed integer.
	using int8 = int8_t;

	/// @brief Represents an 8 bit unsigned integer.
	using uint8 = uint8_t;

	/// @brief Represents a 16 bit signed integer.
	using int16 = int16_t;

	/// @brief Represents a 16 bit unsigned integer.
	using uint16 = uint16_t;

	/// @brief Represents a 32 bit signed integer.
	using int32 = int32_t;

	/// @brief Represents a 32 bit unsigned integer.
	using uint32 = uint32_t;

	/// @brief Represents a 32 bit signed integer.
	using int64 = int64_t;

	/// @brief Represents a 64 bit unsigned integer.
	using uint64 = uint64_t;

#if INTPTR_MAX == INT32_MAX
	using floatN = float;
#else
	using floatN = double;
#endif

	/// @brief Represents a 32 bit floating-point number.
	using float32 = float;

	/// @brief Represents a 64 bit floating-point number.
	using float64 = double;

	template <typename T>
	concept pointer = std::is_pointer_v<T>;

#ifdef _WIN32
	template <typename T>
	concept windows_runtime_type = winrt::impl::has_category_v<T>;
#endif

	enum class line_ending : uint8
	{
		cr, lf, crlf, lfcr,
#ifdef _WIN32
		native = crlf
#else
		native = lf
#endif
	};

	/// @brief Represents an operating environment.
	enum class platform_id : uint8
	{
		windows, macintosh, linux, android, unknown,
#ifdef _WIN32
		native = windows
#elif __APPLE__
		native = macintosh
#elif __linux__
#ifdef __ANDROID__
		native = android
#else
		native = linux
#endif
#else	
		native = unknown
#endif
	};

	/// @brief Enables communication with a debugger.
	struct debugger
	{
		debugger() = delete;

		/// @brief Gets a value that indicates whether a debugger is attached to the process.
		static bool is_present() noexcept
		{
#ifdef _WIN32
			return IsDebuggerPresent() != false;
#elif __APPLE__
			kinfo_proc info;
			std::array<int, 4> mib;
			size_t structSize = sizeof(info);
			info.kp_proc.p_flag = 0;
			mib[0] = CTL_KERN;
			mib[1] = KERN_PROC;
			mib[2] = KERN_PROC_PID;
			mib[3] = getpid();
			structSize = sizeof(info);
			sysctl(mib.data(), static_cast<uint32>(mib.size()), &info, &structSize, nullptr, 0);
			return (info.kp_proc.p_flag & P_TRACED) != 0;
#else
			// TODO implement debugger::is_present on more platforms.
			return false;
#endif
		}

		/// @brief Signals a breakpoint to an attached debugger.
		static void break_process()
		{
			psnip_trap();
		}
	};	

	template <typename Value>
	struct auto_property final
	{
	public:
		auto_property(Value defaultValue = {}) : m_BackingField(defaultValue) {}

		auto_property(auto_property&&) = delete;

		auto_property(auto_property const&) = delete;

		Value operator()() const noexcept
		{
			return m_BackingField;
		}

		void operator()(Value newValue)
		{
			m_BackingField = newValue;
		}
	private:
		Value m_BackingField;
	};

	/// @brief Safely casts a pointer into an integer.
	template <typename Integer>
	auto as_value(pointer auto pointer)
	{
		return static_cast<Integer>(reinterpret_cast<uintN>(pointer));
	}

	/// @brief Safely casts an integer into a pointer.
	template <pointer Pointer>
	auto as_pointer(std::integral auto value) noexcept
	{
		return reinterpret_cast<Pointer>(static_cast<uintN>(value));
	}

	/// @brief Safely dereferences and copies the value pointed by a pointer into an optional.
	template <typename Pointer> requires std::is_pointer_v<Pointer>
	std::optional<typename std::remove_pointer_t<Pointer>> as_optional(Pointer pointer) noexcept
	{
		if (!pointer) return std::nullopt;
		else return { *pointer };
	}

	/// @brief Determines whether a character represents a space.
	template <std::integral CharT>
	constexpr bool is_space(CharT character) noexcept
	{
		return character == ' ' || character == '\f' || character == '\n' || character == '\r' || character == '\t' || character == '\v';
	}

	/// @brief Determines whether a string is empty or only consists of spaces.
	bool has_only_whitespaces(auto text)
	{
		return std::all_of(std::begin(text), std::end(text), [](auto element)
			{
				return is_space(element);
			});
	}

	template <std::integral CharT>
	bool has_only_whitespaces(CharT const* text)
	{
		return has_only_whitespaces(std::basic_string_view<CharT>(text));
	}

	/// @brief Replaces all occurrences of a specific substring in a string.
	template <typename String>
	auto replace_all(String& text, std::basic_string_view<typename String::value_type, typename String::traits_type> replaced, std::basic_string_view<typename String::value_type, typename String::traits_type> replacedWith)
	{
		if (!replaced.empty())
		{
			auto position = text.find(replaced);
			while (position != String::npos)
			{
				text.replace(position, replaced.size(), replacedWith);
				position = text.find(replaced, position + replacedWith.size());
			}
		}
	}
}

#ifdef _WIN32
export namespace winrt
{
	namespace impl
	{
		constexpr hresult error_invalid_state{ hresult_from_win32(5023L) }; // ERROR_INVALID_STATE
		constexpr hresult error_object_closed{ static_cast<hresult>(0x80000013) }; // RO_E_CLOSED
		constexpr hresult error_platform_not_supported{ static_cast<hresult>(0x80131539) }; // COR_E_PLATFORMNOTSUPPORTED
	}

	struct hresult_invalid_state : hresult_error
	{
		hresult_invalid_state() noexcept : hresult_error(impl::error_invalid_state) {}
		hresult_invalid_state(param::hstring const& message) noexcept : hresult_error(impl::error_invalid_state, message) {}
		hresult_invalid_state(take_ownership_from_abi_t) noexcept : hresult_error(impl::error_invalid_state, take_ownership_from_abi) {}
	};

	struct hresult_object_closed : hresult_error
	{
		hresult_object_closed() noexcept : hresult_error(impl::error_object_closed) {}
		hresult_object_closed(param::hstring const& message) noexcept : hresult_error(impl::error_object_closed, message) {}
		hresult_object_closed(take_ownership_from_abi_t) noexcept : hresult_error(impl::error_object_closed, take_ownership_from_abi) {}
	};

	struct hresult_platform_not_supported : hresult_error
	{
		hresult_platform_not_supported() noexcept : hresult_error(impl::error_platform_not_supported) {}
		hresult_platform_not_supported(param::hstring const& message) noexcept : hresult_error(impl::error_platform_not_supported, message) {}
		hresult_platform_not_supported(take_ownership_from_abi_t) noexcept : hresult_error(impl::error_platform_not_supported, take_ownership_from_abi) {}
	};
}

export namespace winrt::OneToolkit
#else
export namespace OneToolkit
#endif
{
	namespace UI
	{
		/// @brief Provides static helper methods for building your own tabbing system.
		struct TabbingHelper
		{
			TabbingHelper() = delete;

			/// @brief Gets the tab index of the tab to be selected next.
			template <std::unsigned_integral T1, std::unsigned_integral T2>
			static std::common_type<T1, T2> GetNewSelectionIndex(T1 currentIndex, T2 collectionSize, bool isReverse)
			{
				if (collectionSize <= 1) return currentIndex;
				else if (isReverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
				else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
			}
		};
	}
}