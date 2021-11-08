// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++/CX projects.

#pragma once
#include <string>
#include <algorithm>
#include <type_traits>
#include <winstring.h>

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

	/// <summary>
	/// Safely casts a pointer's value into an integer.
	/// </summary>
	template <typename Integer, typename Pointer, typename = std::enable_if<std::is_integral<Integer>::value && std::is_pointer<Pointer>::value>::type>
	auto as_value(Pointer pointer) noexcept
	{
		return static_cast<Integer>(reinterpret_cast<size_t>(pointer));
	}

	/// <summary>
	/// Safely casts an integer value into a pointer type.
	/// </summary>
	template <typename Pointer, typename Integer, typename = std::enable_if<std::is_pointer<Pointer>::value && std::is_integral<Integer>::value>::type>
	auto as_pointer(Integer value) noexcept
	{
		return reinterpret_cast<Pointer>(static_cast<size_t>(value));
	}

	/// <summary>
	/// Checks whether a character represents a space.
	/// </summary>
	constexpr bool is_space(char16 const character) noexcept
	{
		return character == L' ' || character == L'\f' || character == L'\n' || character == L'\r' || character == L'\t' || character == L'\v';
	}

	/// <summary>
	/// Determines whether a string is empty or that it only contains whitespace characters.
	/// </summary>
	inline bool has_only_whitespaces(Platform::String^ text)
	{
		return std::find_if(begin(text), end(text), [](auto const character) { return !is_space(character); }) == end(text);
	}

	/// <summary>
	/// Replaces all occurrences of a specific character in a string.
	/// </summary>
	void replace_all(Platform::String^& text, char16 const replaced, char16 const replacedWith)
	{
		std::wstring textCopy = { text->Data(), text->Length() };
		std::replace(textCopy.begin(), textCopy.end(), replaced, replacedWith);
		text = ref new Platform::String(textCopy.data(), textCopy.size());
	}

	/// <summary>
	/// Replaces all occurrences of a specific substring in a string.
	/// </summary>
	void replace_all(Platform::String^& text, Platform::String^ replaced, Platform::String^ replacedWith)
	{
		__abi_ThrowIfFailed(WindowsReplaceString(reinterpret_cast<HSTRING>(text), reinterpret_cast<HSTRING>(replaced), reinterpret_cast<HSTRING>(replacedWith), reinterpret_cast<HSTRING*>(&text)));
	}
}

namespace OneToolkit
{
	namespace Data
	{
		/// <summary>
		/// Represents the order of bytes.
		/// </summary>
		enum class Endianness : uint8
		{
			None, Little, Big
		};
	}

	namespace UI
	{
		/// <summary>
		/// Provides helper methods for building your own tabbing system.
		/// </summary>
		namespace TabSwitcher
		{
			/// <summary>
			/// Gets the tab index to be selected next.
			/// </summaray>
			constexpr uint64 GetNewSelectionIndex(uint64 const currentIndex, uint64 const collectionSize, bool const reverse)
			{
				if (collectionSize <= 1) return currentIndex;
				else if (reverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
				else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
			}
		}

		/// <summary>
		/// Provides helper methods for manipulating colors.
		/// </summary>
		namespace ColorUtility
		{
			/// <summary>
			/// Finds and returns the inverse of a single component.
			/// </summary>
			constexpr uint8 InvertComponent(uint8 const component) noexcept
			{
				return 255 - component;
			}

			/// <summary>
			/// Finds and returns the inverse of a color.
			/// </summary>
			inline auto Invert(Windows::UI::Color const color) noexcept
			{
				return Windows::UI::ColorHelper::FromArgb(InvertComponent(color.A), InvertComponent(color.R), InvertComponent(color.G), InvertComponent(color.B));
			}
		}
	}
}