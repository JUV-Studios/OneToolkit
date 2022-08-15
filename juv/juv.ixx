module;

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
	/// @brief Represents an 8-bit character suitable for storing UTF-8 code units.
	using char8 = char8_t;

	/// @brief Represents a 16-bit character suitable for storing UTF-16 code units.
	using char16 = char16_t;

	/// @brief Represents a 32-bit character suitable for storing UTF-32 code units.
	using char32 = char32_t;

	/// @brief Represents an 8-bit signed integer.
	using int8 = int8_t;

	/// @brief Represents an 8-bit unsigned integer.
	using uint8 = uint8_t;

	/// @brief Represents a 16-bit signed integer.
	using int16 = int16_t;

	/// @brief Represents a 16-bit unsigned integer.
	using uint16 = uint16_t;

	/// @brief Represents a 32-bit signed integer.
	using int32 = int32_t;

	/// @brief Represents a 32-bit unsigned integer.
	using uint32 = uint32_t;

	/// @brief Represents a 32-bit signed integer.
	using int64 = int64_t;

	/// @brief Represents a 64-bit unsigned integer.
	using uint64 = uint64_t;

	/// @brief Represents a 32-bit floating-point number.
	using float32 = float;

	/// @brief Represents a 64-bit floating-point number.
	using float64 = double;

	template <typename T>
	concept pointer = std::is_pointer_v<T>;

	template <typename Value>
	struct auto_property final
	{
	public:
		auto_property(auto_property&&) = delete;

		auto_property(auto_property const&) = delete;

		template <typename TValue = Value>
		auto_property(TValue&& defaultValue = {}) : m_BackingField(std::forward<TValue>(defaultValue)) {}

		Value operator()() const noexcept
		{
			return m_BackingField;
		}

		template <typename TValue = Value>
		void operator()(TValue&& newValue)
		{
			m_BackingField = std::forward<TValue>(newValue);
		}
	private:
		Value m_BackingField;
	};

	/// @brief Safely casts a pointer into an integer.
	template <typename Integer>
	auto as_value(pointer auto pointer)
	{
		return static_cast<Integer>(reinterpret_cast<intptr_t>(pointer));
	}

	/// @brief Safely casts an integer into a pointer.
	template <pointer Pointer>
	auto as_pointer(std::integral auto value) noexcept
	{
		return reinterpret_cast<Pointer>(static_cast<intptr_t>(value));
	}

	/// @brief Safely dereferences and copies the value pointed by a pointer into an optional.
	template <pointer Pointer>
	std::optional<typename std::remove_pointer_t<Pointer>> as_optional(Pointer pointer) noexcept
	{
		if (!pointer) return std::nullopt;
		else return { *pointer };
	}

	/// @brief Determines whether a character represents a space.
	template <std::integral Character>
	constexpr bool is_space(Character character) noexcept
	{
		return character == ' ' || character == '\f' || character == '\n' || character == '\r' || character == '\t' || character == '\v';
	}

	/// @brief Determines whether a string is empty or only consists of spaces.
	constexpr bool has_only_whitespaces(auto text)
	{
		return std::all_of(std::begin(text), std::end(text), [](auto element)
			{
				return is_space(element);
			});
	}

	/// @brief Determines whether a string is empty or only consists of spaces.
	template <std::integral Character>
	constexpr bool has_only_whitespaces(Character const* text)
	{
		return has_only_whitespaces(std::basic_string_view<Character>(text));
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
export namespace winrt::OneToolkit
#else
export namespace OneToolkit
#endif
{
	namespace UI
	{
		struct ColorUtility
		{
			ColorUtility() = delete;

			/// @brief Finds the inverse of a single component.
			static juv::uint8 InvertComponent(juv::uint8 component)
			{
				return 255 - component;
			}
		};

		namespace Input
		{
			/// @brief Provides static helper methods for building your own tabbing system.
			struct TabSwitcher
			{
				TabSwitcher() = delete;

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

	namespace System
	{
		template <std::integral Character, size_t Size>
		struct StringTemplateParameter
		{
			Character Value[Size];

			constexpr StringTemplateParameter(Character const(&string)[Size])
			{
				std::copy_n(string, Size, Value);
			}
		};
	}
}