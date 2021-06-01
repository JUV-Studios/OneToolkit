#pragma once
#include <cctype>
#include <type_traits>
#include <string_view>

namespace juv
{
	/// <summary>
	/// Safely casts a pointer's value into an integer.
	/// </summary>
	template <typename Integer, typename Pointer, typename = std::enable_if_t<std::is_integral_v<Integer> && std::is_pointer_v<Pointer>>>
	auto as_value(Pointer pointer) noexcept 
	{
		return static_cast<Integer>(reinterpret_cast<size_t>(pointer));
	}

	/// <summary>
	/// Safely casts an integer value into a pointer type.
	/// </summary>
	template <typename Pointer, typename Integer, typename = std::enable_if_t<std::is_integral_v<Integer> && std::is_pointer_v<Pointer>>>
	auto as_pointer(Integer value) noexcept 
	{
		return reinterpret_cast<Pointer>(static_cast<size_t>(value));
	}

	/// <summary>
	/// Checks whether a string is empty or that it only contains whitespace characters.
	/// </summary>
	inline bool has_only_whitespaces(std::string_view text)
	{
		return std::find_if(text.begin(), text.end(), [](auto ch) { return !isspace(ch); }) == text.end();
	}

	/// <summary>
	/// Checks whether a string is empty or that it only contains whitespace characters.
	/// </summary>
	inline bool has_only_whitespaces(std::wstring_view text)
	{
		return std::find_if(text.begin(), text.end(), [](auto ch) { return !iswspace(ch); }) == text.end();
	}
}