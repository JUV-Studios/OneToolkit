module;

#define juv_ixx
#include "juv.h"
#include "debug-trap.h"
#include <span>
#include <algorithm>
#ifdef _WIN32
#include <Debugapi.h>
#elif __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#if __has_include(<winrt/Windows.Security.Cryptography.h>)
#include <winrt/Windows.Security.Cryptography.h>
#endif

export module juv;

using namespace juv;
using namespace juv::type_traits;
using namespace OneToolkitLib::Data;

#ifndef WINRT_Windows_Security_Cryptography_H
#endif

struct ByteOrderMark
{
	ByteOrderMark() = delete;
	static constexpr std::array Utf8{ std::byte(0xEF), std::byte(0xBB), std::byte(0xBF) };
	static constexpr std::array Utf16LE{ std::byte(0xFF), std::byte(0xFE) };
	static constexpr std::array Utf16BE{ std::byte(0xFE), std::byte(0xFF) };
};

bool IsUtf16Regular(std::span<std::byte const> const buffer, Endianness& endianness) noexcept
{
	auto bufferSize = buffer.size();
	if (bufferSize < 2)
	{
		endianness = Endianness::None;
		return false;
	}

	bufferSize--; // Reduce size by 1 so we don't need to worry about bounds checking for pairs of bytes.
	size leControlCharacters = 0;
	size beControlCharacters = 0;
	uint8 ch1, ch2;
	size pos = 0;
	while (pos < bufferSize)
	{
		ch1 = std::to_integer<uint8>(buffer[pos++]);
		ch2 = std::to_integer<uint8>(buffer[pos++]);
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

bool IsUtf16Ascii(std::span<std::byte const> const buffer, Endianness& endianness) noexcept
{
	// Get the number of even and odd nulls.
	size oddNulls = 0;
	size evenNulls = 0;
	for (size position = 0; position < buffer.size(); ++position)
	{
		if (std::to_integer<uint8>(buffer[position]) == 0)
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

template <typename Character>
struct debug_out
{
	debug_out& operator<<(auto const value)
	{
#ifdef _WIN32
		if constexpr (std::is_same_v<Character, char>) OutputDebugStringA(std::format("{}", value).data());
		else OutputDebugStringW(std::format(L"{}", value).data());
#endif
		return *this;
	}
};

/// <summary>
/// Enables communication with a debugger.
/// </summary>
export namespace juv::debugger
{
	debug_out<char> out;

	debug_out<wchar> wout;

	/// <summary>
	/// Gets a value that indicates whether a debugger is attached to the process.
	/// </summary>
	/// <returns>true if a debugger is attached; otherwise, false.</returns>
	bool is_present() noexcept
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
		return ((info.kp_proc.p_flag & P_TRACED) != 0);
#else
		static_assert(false, "juv::debugger::is_present hasn't yet been implemented on your platform.");
#endif
	}

	/// <summary>
	/// Signals a breakpoint to an attached debugger.
	/// </summary>
	void break_process()
	{
		psnip_trap();
	}
}

export namespace OneToolkitLib::Data
{
	namespace Text
	{
#ifdef WINRT_Windows_Security_Cryptography_H
		using BinaryStringEncoding = Windows::Security::Cryptography::BinaryStringEncoding;
#else
		/// <summary>
		/// Specifies the type of character encoding for a stream.
		/// </summary>
		enum class BinaryStringEncoding
		{
			Utf8 = 0,
			Utf16LE = 1,
			Utf16BE = 2,
		};
#endif

		/// <summary>
		/// Provides high performance extension methods for strings.
		/// </summary>
		namespace StringExtensions
		{
			/// <summary>
			/// Creates a copy of a string and returns an handle to it.
			/// </summary>
			template <typename StringView>
			auto Copy(StringView const text)
			{
				return std::basic_string<typename StringView::value_type>(text.data(), text.size());
			}

			/// <summary>
			/// Determines whether a string is empty or that it only contains whitespace characters.
			/// </summary>
			bool HasOnlyWhitespaces(auto const text)
			{
				return std::find_if(std::begin(text), std::end(text), [](auto const character) { return !is_space(character); }) == std::end(text);
			}

			template <typename String>
			void Trim(String& text, string_view_t<String> const trimmed)
			{
				TrimEnd(text, trimmed);
				TrimStart(text, trimmed);
			}

			template <typename String>
			void TrimStart(String& text, string_view_t<String> const trimmed)
			{

			}

			template <typename String>
			void TrimEnd(String& text, string_view_t<String> const trimmed)
			{

			}

			/// <summary>
			/// Replaces all occurrences of a specific character in a string without allocating a new one.
			/// </summary>
			template <typename String>
			void ReplaceCharacter(String& text, typename String::value_type replaced, typename String::value_type replacedWith)
			{
				std::replace(std::begin(text), std::end(text), replaced, replacedWith);
			}

			/// <summary>
			/// Replaces all occurrences of a specific substring in a string without allocating a new one.
			/// </summary>
			template <typename String>
			void ReplaceSubstring(String& text, string_view_t<String> const replaced, string_view_t<String> const replacedWith)
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

		/// <summary>
		/// Provides the ability to find out and detect the encoding of a text buffer.
		/// </summary>
		namespace TextEncodingHelper
		{
			/// <summary>
			/// Gets whether the buffer contains UTF8 encoded text or not.
			/// </summary>
			bool IsUtf8(std::span<std::byte const> const buffer) noexcept
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

				size position = 0;
				uint8 moreCharacters;
				while (position < buffer.size())
				{
					auto character = std::to_integer<uint8>(buffer[position++]);
					if (character == 0) return false;
					else if (character <= 127) moreCharacters = 0;
					else if (character >= 194 && character <= 223) moreCharacters = 1;
					else if (character >= 224 && character <= 239) moreCharacters = 2;
					else if (character >= 240 && character <= 244) moreCharacters = 3;
					else return false;

					// Check secondary characters are in range if we are expecting any.
					while (moreCharacters > 0 && position < buffer.size())
					{
						character = std::to_integer<uint8>(buffer[position++]);
						if (character < 128 || character > 191) return false;
						moreCharacters--;
					}
				}

				return true;
			}

			/// <summary>
			/// Gets whether the buffer contains UTF16 encoded text or not.
			/// </summary>
			bool IsUtf16(std::span<std::byte const> const buffer, Endianness& endianness) noexcept
			{
				if (IsUtf16Regular(buffer, endianness)) return true;
				else return IsUtf16Ascii(buffer, endianness);
			}

			/// <summary>
			/// Determines the the encoding of a text buffer using various methods.
			/// </summary>
			std::optional<BinaryStringEncoding> Detect(std::span<std::byte const> const buffer)
			{
				if (buffer.size() >= 2 && buffer[0] == ByteOrderMark::Utf16LE[0] && buffer[1] == ByteOrderMark::Utf16LE[1]) return BinaryStringEncoding::Utf16LE;
				else if (buffer.size() >= 2 && buffer[0] == ByteOrderMark::Utf16BE[0] && buffer[1] == ByteOrderMark::Utf16BE[1]) return BinaryStringEncoding::Utf16BE;
				else if (buffer.size() >= 3 && buffer[0] == ByteOrderMark::Utf8[0] && buffer[1] == ByteOrderMark::Utf8[1] && buffer[2] == ByteOrderMark::Utf8[2]) return BinaryStringEncoding::Utf8;
				else if (IsUtf8(buffer)) return BinaryStringEncoding::Utf8;
				else
				{
					Endianness endianness;
					if (IsUtf16(buffer, endianness)) return endianness == Endianness::Little ? BinaryStringEncoding::Utf16LE : BinaryStringEncoding::Utf16BE;
					else return std::nullopt;
				}
			}
		}

		/// <summary>
		/// Represents a line ending scheme.
		/// </summary>
		enum class LineEnding : uint8
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
			constexpr std::basic_string_view<Character> GetNewLineString(LineEnding const lineEnding) noexcept
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
				else if constexpr (std::is_same_v<Character, wchar>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return L"\r";
					case LineEnding::LF: return L"\n";
					case LineEnding::CRLF: return L"\r\n";
					case LineEnding::LFCR: return L"\n\r";
					}
				}
				else if constexpr (std::is_same_v<Character, char8> || std::is_same_v<Character, unsigned char>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return u8"\r";
					case LineEnding::LF: return u8"\n";
					case LineEnding::CRLF: return u8"\r\n";
					case LineEnding::LFCR: return u8"\n\r";
					}
				}
				else if constexpr (std::is_same_v<Character, char16>)
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return u"\r";
					case LineEnding::LF: return u"\n";
					case LineEnding::CRLF: return u"\r\n";
					case LineEnding::LFCR: return u"\n\r";
					}
				}
				else if constexpr (std::is_same_v<Character, char32>)
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
			auto Normalize(StringView const text, LineEnding const lineEnding)
			{
				std::basic_string<typename StringView::value_type> result{ text };
				if (lineEnding == LineEnding::CR)
				{
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::CRLF), GetNewLineString<typename StringView::value_type>(LineEnding::CR));
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::LFCR), GetNewLineString<typename StringView::value_type>(LineEnding::CR));
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::LF), GetNewLineString<typename StringView::value_type>(LineEnding::CR));
				}
				else if (lineEnding == LineEnding::LF)
				{
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::CRLF), GetNewLineString<typename StringView::value_type>(LineEnding::LF));
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::LFCR), GetNewLineString<typename StringView::value_type>(LineEnding::LF));
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::CR), GetNewLineString<typename StringView::value_type>(LineEnding::LF));
				}
				else if (lineEnding == LineEnding::CRLF)
				{
					result = Normalize(text, LineEnding::CR);
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::CR), GetNewLineString<typename StringView::value_type>(LineEnding::CRLF));
				}
				else if (lineEnding == LineEnding::LFCR)
				{
					result = Normalize(text, LineEnding::LF);
					StringExtensions::ReplaceSubstring(result, GetNewLineString<typename StringView::value_type>(LineEnding::LF), GetNewLineString<typename StringView::value_type>(LineEnding::LFCR));
				}

				return result;
			}
		};
	}
}