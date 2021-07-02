// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in cross-platform C++ projects.

#pragma once
#include <array>
#include <optional>
#include <type_traits>
#include <string_view>

namespace juv
{
#ifdef __cpp_char8_t
    using char8 = char8_t;
#else
    using char8 = unsigned char;
#endif
    using char16 = char16_t;
    using char32 = char32_t;
    using wchar = wchar_t;
    using int8 = int8_t;
    using uint8 = uint8_t;
    using int16 = int16_t;
    using uint16 = uint16_t;
    using int32 = int32_t;
    using uint32 = uint32_t;
    using int64 = int64_t;
    using uint64 = uint64_t;

    /// <summary>
    /// Safely casts a pointer's value into an integer.
    /// </summary>
    template <typename Integer, typename Pointer, typename = std::enable_if_t<std::is_integral_v<Integer>&& std::is_pointer_v<Pointer>>>
    auto as_value(Pointer pointer) noexcept
    {
        return static_cast<Integer>(reinterpret_cast<size_t>(pointer));
    }

    /// <summary>
    /// Safely casts an integer value into a pointer type.
    /// </summary>
    template <typename Pointer, typename Integer, typename = std::enable_if_t<std::is_integral_v<Integer>&& std::is_pointer_v<Pointer>>>
    auto as_pointer(Integer value) noexcept
    {
        return reinterpret_cast<Pointer>(static_cast<size_t>(value));
    }

    /// <summary>
    /// Safely dereferences a pointer into an optional value.
    /// </summary>
    template <typename Pointer, typename = std::enable_if_t<std::is_pointer_v<Pointer>>>
    std::optional<typename std::remove_pointer_t<Pointer>> as_optional(Pointer pointer) noexcept
    {
        if (!pointer) return {};
        else return { *pointer };
    }

    /// <summary>
    /// Checks whether a character represents a space.
    /// </summary>
    constexpr bool is_space(char const character) noexcept
    {
        return character == ' ' || character == '\f' || character == '\n' || character == '\r' || character == '\t' || character == '\v';
    }

    /// <summary>
    /// Checks whether a character represents a space.
    /// </summary>
    constexpr bool is_space(wchar const character) noexcept
    {
        return character == L' ' || character == L'\f' || character == L'\n' || character == L'\r' || character == L'\t' || character == L'\v';
    }

    /// <summary>
    /// Checks whether a character represents a space.
    /// </summary>
    constexpr bool is_space(char8 const character) noexcept
    {
        return character == u8' ' || character == u8'\f' || character == u8'\n' || character == u8'\r' || character == u8'\t' || character == u8'\v';
    }

    /// <summary>
    /// Checks whether a character represents a space.
    /// </summary>
    constexpr bool is_space(char16 const character) noexcept
    {
        return character == u' ' || character == u'\f' || character == u'\n' || character == u'\r' || character == u'\t' || character == u'\v';
    }

    /// <summary>
    /// Checks whether a character represents a space.
    /// </summary>
    constexpr bool is_space(char32 const character) noexcept
    {
        return character == U' ' || character == U'\f' || character == U'\n' || character == U'\r' || character == U'\t' || character == U'\v';
    }

    /// <summary>
    /// Checks whether a string is empty or that it only contains whitespace characters.
    /// </summary>
    template <typename String>
    inline bool has_only_whitespaces(String const text)
    {
        return std::find_if(text.begin(), text.end(), [](auto const ch) { return !is_space(ch); }) == text.end();
    }
}

namespace winrt::OneToolkit::Data::Text
{
    /// <summary>
    /// Represents a line ending scheme.
    /// </summary>
    enum class LineEnding : juv::uint8
    {
        CR, LF, CRLF, Mixed
    };

    /// <summary>
    /// Provides helper methods for dealing with line endings.
    /// </summary>
    namespace LineEndingHelper
    {
        /// <summary>
        /// Gets whether a string represents a new line.
        /// </summary>
        bool IsNewLineString(std::string_view const text, LineEnding& lineEnding)
        {
            if (text == "\r")
            {
                lineEnding = LineEnding::CR;
                return true;
            }
            else if (text == "\n")
            {
                lineEnding = LineEnding::LF;
                return true;
            }
            else if (text == "\r\n" || text == "\n\r")
            {
                lineEnding = LineEnding::CRLF;
                return true;
            }
            else
            {
                lineEnding = LineEnding::Mixed;
                return false;
            }
        }

        /// <summary>
        /// Gets whether a string represents a new line.
        /// </summary>
        bool IsNewLineString(std::wstring_view const text, LineEnding& lineEnding)
        {
            if (text == L"\r")
            {
                lineEnding = LineEnding::CR;
                return true;
            }
            else if (text == L"\n")
            {
                lineEnding = LineEnding::LF;
                return true;
            }
            else if (text == L"\r\n" || text == L"\n\r")
            {
                lineEnding = LineEnding::CRLF;
                return true;
            }
            else
            {
                lineEnding = LineEnding::Mixed;
                return false;
            }
        }

#ifdef __cpp_char8_t
        /// <summary>
        /// Gets whether a string represents a new line.
        /// </summary>
        bool IsNewLineString(std::u8string_view const text, LineEnding& lineEnding)
        {
            if (text == u8"\r")
            {
                lineEnding = LineEnding::CR;
                return true;
            }
            else if (text == u8"\n")
            {
                lineEnding = LineEnding::LF;
                return true;
            }
            else if (text == u8"\r\n" || text == u8"\n\r")
            {
                lineEnding = LineEnding::CRLF;
                return true;
            }
            else
            {
                lineEnding = LineEnding::Mixed;
                return false;
            }
        }
#endif

        /// <summary>
        /// Gets whether a string represents a new line.
        /// </summary>
        bool IsNewLineString(std::u16string_view const text, LineEnding& lineEnding)
        {
            if (text == u"\r")
            {
                lineEnding = LineEnding::CR;
                return true;
            }
            else if (text == u"\n")
            {
                lineEnding = LineEnding::LF;
                return true;
            }
            else if (text == u"\r\n" || text == u"\n\r")
            {
                lineEnding = LineEnding::CRLF;
                return true;
            }
            else
            {
                lineEnding = LineEnding::Mixed;
                return false;
            }
        }

        /// <summary>
        /// Gets whether a string represents a new line.
        /// </summary>
        bool IsNewLineString(std::u32string_view const text, LineEnding& lineEnding)
        {
            if (text == U"\r")
            {
                lineEnding = LineEnding::CR;
                return true;
            }
            else if (text == U"\n")
            {
                lineEnding = LineEnding::LF;
                return true;
            }
            else if (text == U"\r\n" || text == U"\n\r")
            {
                lineEnding = LineEnding::CRLF;
                return true;
            }
            else
            {
                lineEnding = LineEnding::Mixed;
                return false;
            }
        }

        /// <summary>
        /// Gets a string that represents the desired line ending.
        /// </summary>
        template <typename Character>
        constexpr std::basic_string_view<Character> GetNewLineString(LineEnding lineEnding)
        {
            if constexpr (std::is_same_v<Character, char>)
            {
                switch (lineEnding)
                {
                case LineEnding::CR: return "\r";
                case LineEnding::LF: return "\n";
                case LineEnding::CRLF: return "\r\n";
                }
            }
            else if constexpr (std::is_same_v<Character, juv::wchar>)
            {
                switch (lineEnding)
                {
                case LineEnding::CR: return L"\r";
                case LineEnding::LF: return L"\n";
                case LineEnding::CRLF: return L"\r\n";
                }
            }
            else if constexpr (std::is_same_v<Character, juv::char8> || std::is_same_v<Character, unsigned char>)
            {
                switch (lineEnding)
                {
                case LineEnding::CR: return u8"\r";
                case LineEnding::LF: return u8"\n";
                case LineEnding::CRLF: return u8"\r\n";
                }
            }
            else if constexpr (std::is_same_v<Character, juv::char16>)
            {
                switch (lineEnding)
                {
                case LineEnding::CR: return u"\r";
                case LineEnding::LF: return u"\n";
                case LineEnding::CRLF: return u"\r\n";
                }
            }
            else if constexpr (std::is_same_v<Character, juv::char32>)
            {
                switch (lineEnding)
                {
                case LineEnding::CR: return U"\r";
                case LineEnding::LF: return U"\n";
                case LineEnding::CRLF: return U"\r\n";
                }
            }
            else
            {
                static_assert(false, "GetNewLineString doesn't support the specified character type.");
            }

            return {};
        }
    };
}