// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in cross-platform C++ projects.

#pragma once
#include <span>
#include <array>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <string_view>
#include <type_traits>

namespace juv
{
	using char8 = char8_t;
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
	template <typename Integer, typename Pointer>
	auto as_value(Pointer pointer) noexcept requires std::is_integral_v<Integer> && std::is_pointer_v<Pointer>
	{
		return static_cast<Integer>(reinterpret_cast<size_t>(pointer));
	}

	/// <summary>
	/// Safely casts an integer value into a pointer type.
	/// </summary>
	template <typename Pointer, typename Integer>
	auto as_pointer(Integer value) noexcept requires std::is_integral_v<Integer> && std::is_pointer_v<Pointer>
	{
		return reinterpret_cast<Pointer>(static_cast<size_t>(value));
	}

	/// <summary>
	/// Safely dereferences a pointer into an optional value.
	/// </summary>
	template <typename Pointer>
	std::optional<typename std::remove_pointer_t<Pointer>> as_optional(Pointer pointer) noexcept requires std::is_pointer_v<Pointer>
	{
		if (!pointer) return {};
		else return { *pointer };
	}

	/// <summary>
	/// Checks whether a character represents a space.
	/// </summary>
	template <typename Character>
	constexpr bool is_space(Character const character) noexcept
	{
		if constexpr (std::is_same_v<Character, char>)
		{
			return character == ' ' || character == '\f' || character == '\n' || character == '\r' || character == '\t' || character == '\v';
		}
		else if constexpr (std::is_same_v<Character, wchar>)
		{
			return character == L' ' || character == L'\f' || character == L'\n' || character == L'\r' || character == L'\t' || character == L'\v';
		}
		else if constexpr (std::is_same_v<Character, char8> || std::is_same_v<Character, unsigned char>)
		{
			return character == u8' ' || character == u8'\f' || character == u8'\n' || character == u8'\r' || character == u8'\t' || character == u8'\v';
		}
		else if constexpr (std::is_same_v<Character, char16>)
		{
			return character == u' ' || character == u'\f' || character == u'\n' || character == u'\r' || character == u'\t' || character == u'\v';
		}
		else if constexpr (std::is_same_v<Character, char32>)
		{
			return character == U' ' || character == U'\f' || character == U'\n' || character == U'\r' || character == U'\t' || character == U'\v';
		}
		else
		{
			static_assert(false, "is_space doesn't support the specified character type.");
		}
	}

	/// <summary>
	/// Checks whether a string is empty or that it only contains whitespace characters.
	/// </summary>
	template <typename String>
	bool has_only_whitespaces(String const text)
	{
		return std::find_if(text.begin(), text.end(), [](auto const character) { return !is_space(character); }) == text.end();
	}

	template <typename String>
	void replace_all(String& text, typename String::value_type replaced, typename String::value_type replacedWith)
	{
		std::replace(text.begin(), text.end(), replaced, replacedWith);
	}

	template <typename String>
	void replace_all(String& text, std::basic_string_view<typename String::value_type> const replaced, std::basic_string_view<typename String::value_type> const replacedWith)
	{
		if (replaced.empty()) return;
		auto position = text.find(replaced);
		while (position != String::npos)
		{
			text.replace(position, replaced.size(), replacedWith);
			position = text.find(replaced, position + replacedWith.size());
		}
	}
}

namespace winrt::OneToolkit::Data
{
	enum class Endianness : juv::uint8
	{
		None, Little, Big
	};

	namespace Text
	{
		/// <summary>
		/// Represents an encoding of a text buffer.
		/// </summary>
		enum class TextEncoding : juv::uint8
		{
			Unknown, Ascii, Utf8, Utf16LE, Utf16BE
		};

		/// <summary>
		/// Provides the ability to find out and detect the encoding of a text buffer.
		/// </summary>
		class TextEncodingHelper
		{
		public:
			TextEncodingHelper() = delete;

			/// <summary>
			/// Gets whether the buffer contains ASCII encoded text or not.
			/// </summary>
			static bool IsAscii(std::span<juv::uint8 const> const buffer) noexcept
			{
				for (auto chr : buffer)
				{
					if (chr > 128) return false;
				}

				return true;
			}

			/// <summary>
			/// Gets whether the buffer contains UTF8 encoded text or not.
			/// </summary>
			static bool IsUtf8(std::span<juv::uint8 const> const buffer) noexcept
			{
				/*
					UTF8 valid sequences:
					0xxxxxxx  ASCII
					110xxxxx 10xxxxxx 2-byte
					1110xxxx 10xxxxxx 10xxxxxx 3-byte
					11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 4-byte

					Width in UTF8:
					0-127 - 1 byte
					194-223	- 2 bytes
					224-239	- 3 bytes
					240-244 - 4 bytes
					Subsequent chars are in the range 128-191
				*/

				size_t position = 0;
				juv::uint8 moreCharacters;
				while (position < buffer.size())
				{
					auto character = buffer[position++];
					if (character == 0) return false;
					else if (character <= 127) moreCharacters = 0;
					else if (character >= 194 && character <= 223) moreCharacters = 1;
					else if (character >= 224 && character <= 239) moreCharacters = 2;
					else if (character >= 240 && character <= 244) moreCharacters = 3;
					else return false;

					// Check secondary characters are in range if we are expecting any.
					while (moreCharacters > 0 && position < buffer.size())
					{
						character = buffer[position++];
						if (character < 128 || character > 191) return false;
						moreCharacters--;
					}
				}

				return true;
			}

			static bool IsUtf16(std::span<juv::uint8 const> const buffer, Endianness& endianness) noexcept
			{
				if (IsUtf16Regular(buffer, endianness)) return true;
				else return IsUtf16Ascii(buffer, endianness);
			}

			static TextEncoding Detect(std::span<juv::uint8 const> const buffer)
			{
				if (buffer.size() >= 2 && buffer[0] == BOM::Utf16LE[0] && buffer[1] == BOM::Utf16LE[1]) return TextEncoding::Utf16LE;
				else if (buffer.size() >= 2 && buffer[0] == BOM::Utf16BE[0] && buffer[1] == BOM::Utf16BE[1]) return TextEncoding::Utf16BE;
				else if (buffer.size() >= 3 && buffer[0] == BOM::Utf8[0] && buffer[1] == BOM::Utf8[1] && buffer[2] == BOM::Utf8[2]) return TextEncoding::Utf8;
				else if (IsUtf8(buffer)) return IsAscii(buffer) ? TextEncoding::Ascii : TextEncoding::Ascii;
				else
				{
					Endianness endianness;
					if (IsUtf16(buffer, endianness)) return endianness == Endianness::Little ? TextEncoding::Utf16LE : TextEncoding::Utf16BE;
					else return TextEncoding::Unknown;
				}
			}
		private:
			struct BOM
			{
				BOM() = delete;
				static constexpr std::array Utf8{ 0xEF, 0xBB, 0xBF };
				static constexpr std::array Utf16LE{ 0xFF, 0xFE };
				static constexpr std::array Utf16BE{ 0xFE, 0xFF };
			};

			static bool IsUtf16Regular(std::span<juv::uint8 const> const buffer, Endianness& endianness) noexcept
			{
				auto size = buffer.size();
				if (size < 2)
				{
					endianness = Endianness::None;
					return false;
				}

				size--; // Reduce size by 1 so we don't need to worry about bounds checking for pairs of bytes.
				size_t leControlCharacters = 0;
				size_t beControlCharacters = 0;
				juv::uint8 ch1, ch2;
				size_t pos = 0;
				while (pos < size)
				{
					ch1 = buffer[pos++];
					ch2 = buffer[pos++];
					if (!ch1 && (ch2 == 0x0a || ch2 == 0x0d)) beControlCharacters++;
					else if (!ch2 && (ch1 == 0x0a || ch1 == 0x0d)) leControlCharacters++;
					if (leControlCharacters && beControlCharacters)
					{
						// If we're finding both LE and BE control characters, it's not UTF16.
						endianness = Endianness::None;
						return false;
					}
				}

				if (leControlCharacters)
				{
					endianness = Endianness::Little;
					return true;
				}
				else if (beControlCharacters)
				{
					endianness = Endianness::Big;
					return true;
				}
				else
				{
					endianness = Endianness::None;
					return false;
				}
			}

			static bool IsUtf16Ascii(std::span<juv::uint8 const> const buffer, Endianness& endianness) noexcept
			{
				// Get the number of even and odd nulls.
				size_t oddNulls = 0;
				size_t evenNulls = 0;
				for (size_t position = 0; position < buffer.size(); ++position)
				{
					if (buffer[position] == 0)
					{
						if (position % 2 == 0) evenNulls++;
						else oddNulls++;
					}
				}

				double evenNullThreshold = (evenNulls * 2.0) / buffer.size();
				double oddNullThreshold = (oddNulls * 2.0) / buffer.size();
				double expectedNullThreshold = 70 / 100.0;
				double unexpectedNullThreshold = 10 / 100.0;
				if (evenNullThreshold < unexpectedNullThreshold && oddNullThreshold > expectedNullThreshold) // Lots of odd nulls, low number of even nulls.
				{
					endianness = Endianness::Little;
					return true;
				}
				else if (oddNullThreshold < unexpectedNullThreshold && evenNullThreshold > expectedNullThreshold) // Lots of even nulls, low number of odd nulls.
				{
					endianness = Endianness::Big;
					return true;
				}
				else
				{
					endianness = Endianness::None;
					return false;
				}
			}
		};

		/// <summary>
		/// Represents a line ending scheme.
		/// </summary>
		enum class LineEnding : juv::uint8
		{
			CR, LF, CRLF, LFCR, Mixed
		};

		/// <summary>
		/// Provides helper methods for dealing with line endings.
		/// </summary>
		namespace LineEndingHelper
		{
			/// <summary>
			/// Gets a string that represents the desired line ending.
			/// </summary>
			template <typename Character>
			constexpr std::basic_string_view<Character> GetNewLineString(LineEnding lineEnding) noexcept
			{
				if constexpr (std::is_same_v<Character, char>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return "\r";
					case LineEnding::LF: return "\n";
					case LineEnding::CRLF: return "\r\n";
					case LineEnding::LFCR: return "\n\r";
					}
				}
				else if constexpr (std::is_same_v<Character, juv::wchar>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return L"\r";
					case LineEnding::LF: return L"\n";
					case LineEnding::CRLF: return L"\r\n";
					case LineEnding::LFCR: return L"\n\r";
					}
				}
				else if constexpr (std::is_same_v<Character, juv::char8> || std::is_same_v<Character, unsigned char>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return u8"\r";
					case LineEnding::LF: return u8"\n";
					case LineEnding::CRLF: return u8"\r\n";
					case LineEnding::LFCR: return u8"\n\r";
					}
				}
				else if constexpr (std::is_same_v<Character, juv::char16>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return u"\r";
					case LineEnding::LF: return u"\n";
					case LineEnding::CRLF: return u"\r\n";
					case LineEnding::LFCR: return u"\n\r";
					}
				}
				else if constexpr (std::is_same_v<Character, juv::char32>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return U"\r";
					case LineEnding::LF: return U"\n";
					case LineEnding::CRLF: return U"\r\n";
					case LineEnding::LFCR: return U"\n\r";
					}
				}
				else
				{
					static_assert(false, "GetNewLineString doesn't support the specified character type.");
				}

				return {};
			}

			/// <summary>
			/// Gets whether a string represents a new line.
			/// </summary>
			template <typename StringView>
			constexpr bool IsNewLineString(StringView const text, LineEnding& lineEnding) noexcept
			{
				if (text == GetNewLineString<typename StringView::value_type>(LineEnding::CR))
				{
					lineEnding = LineEnding::CR;
					return true;
				}
				else if (text == GetNewLineString<typename StringView::value_type>(LineEnding::LF))
				{
					lineEnding = LineEnding::LF;
					return true;
				}
				else if (text == GetNewLineString<typename StringView::value_type>(LineEnding::CRLF))
				{
					lineEnding = LineEnding::CRLF;
					return true;
				}
				else if (text == GetNewLineString<typename StringView::value_type>(LineEnding::LFCR))
				{
					lineEnding = LineEnding::LFCR;
					return true;
				}
				else
				{
					lineEnding = LineEnding::Mixed;
					return false;
				}
			}

			/// <summary>
			/// Normalizes a string to use a single line ending for every new line.
			/// </summary>
			template <typename StringView>
			auto Normalize(StringView const text, LineEnding lineEnding)
			{
				std::basic_string<typename StringView::value_type> result{ text };
				if (lineEnding == LineEnding::CR)
				{
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::CRLF), GetNewLineString<typename StringView::value_type>(LineEnding::CR));
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::LFCR), GetNewLineString<typename StringView::value_type>(LineEnding::CR));
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::LF), GetNewLineString<typename StringView::value_type>(LineEnding::CR));
				}
				else if (lineEnding == LineEnding::LF)
				{
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::CRLF), GetNewLineString<typename StringView::value_type>(LineEnding::LF));
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::LFCR), GetNewLineString<typename StringView::value_type>(LineEnding::LF));
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::CR), GetNewLineString<typename StringView::value_type>(LineEnding::LF));
				}
				else if (lineEnding == LineEnding::CRLF)
				{
					result = Normalize(text, LineEnding::CR);
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::CR), GetNewLineString<typename StringView::value_type>(LineEnding::CRLF));
				}
				else if (lineEnding == LineEnding::LFCR)
				{
					result = Normalize(text, LineEnding::LF);
					juv::replace_all(result, GetNewLineString<typename StringView::value_type>(LineEnding::LF), GetNewLineString<typename StringView::value_type>(LineEnding::LFCR));
				}

				return result;
			}
		};
	}
}

namespace winrt::OneToolkit::UI
{
	struct ColorUtility
	{
		ColorUtility() = delete;
		
		static inline juv::uint8 InvertComponent(juv::uint8 component)
		{
			return 255 - component;
		}
	};

	namespace Input
	{
		struct TabSwitcher
		{
			TabSwitcher() = delete;

			static inline juv::uint64 GetNewSelectionIndex(juv::uint64 currentIndex, juv::uint64 collectionSize, bool reverse)
			{
				if (collectionSize <= 1) return currentIndex;
				else if (reverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
				else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
			}
		}
	}
}