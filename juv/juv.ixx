module;

#include "debug-trap.h"
#include <span>
#include <string>
#include <string_view>
#include <sstream>
#include <concepts>
#include <optional>
#include <algorithm>
#include <type_traits>
#ifdef _WIN32
#define platform_windows
#include <Unknwn.h>
#include <Debugapi.h>
#include <WinString.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Security.Cryptography.h>
#if __has_include(<winrt/Microsoft.UI.h>) && !force_platform_xaml
#include <winrt/Microsoft.UI.h>
#endif
#elif __APPLE__
#define platform_macintosh
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#define return_encoding_specific_string(characterType, string_literal) if constexpr (::is_narrow_character_v<characterType>) return string_literal;\
else if constexpr (::is_wide_character_v<characterType>) L##string_literal;\
else if constexpr (std::is_same_v<characterType, juv::char8>) return u8##string_literal;\
else if constexpr (std::is_same_v<characterType, juv::char16>) return u##string_literal;\
else if constexpr (std::is_same_v<characterType, juv::char32>) return U##string_literal;

export module juv;

export namespace juv
{
	/// @brief Represents an 8-bit character suitable for storing UTF-8 code points.
	using char8 = char8_t;

	/// @brief Represents a 16-bit character suitable for storing UTF-16 code points.
	using char16 = char16_t;

	/// @brief Represents a 32-bit character suitable for storing UTF-32 code points.
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

	/// @brief Represents a platform-specific signed integer that's used as a pointer.
	using intptr = intptr_t;

	/// @brief Represents a platform-specific unsigned integer that's used as a pointer.
	using uintptr = uintptr_t;

	/// @brief Represents a static class that can't be instantiated.
	struct static_t
	{
		static_t() = delete;
	};

	/// @brief Represents a base class that uses the CRTP pattern to access the derived class.
	template <typename Derived>
	struct abstract_t
	{
	protected:
		abstract_t() = default;

		/// @brief Gets a reference to the derived object.
		Derived& get_derived() noexcept
		{
			return static_cast<Derived&>(*this);
		}

		/// @brief Gets a read-only reference to the derived object.
		Derived const& get_derived() const noexcept
		{
			return static_cast<Derived const&>(*this);
		}
	};

	template <typename T>
	struct is_narrow_character : static_t
	{
		static constexpr bool value = false;
	};

	template <>
	struct is_narrow_character<char> : static_t
	{
		static constexpr bool value = true;
	};

	template <>
	struct is_narrow_character<signed char> : static_t
	{
		static constexpr bool value = true;
	};

	template <>
	struct is_narrow_character<unsigned char> : static_t
	{
		static constexpr bool value = true;
	};

	template <typename T>
	struct is_wide_character : static_t
	{
		static constexpr bool value = false;
	};

	template <>
	struct is_wide_character<wchar_t> : static_t
	{
		static constexpr bool value = true;
	};

	template <typename T>
	static constexpr bool is_narrow_character_v = is_narrow_character<T>::value;

	template <typename T>
	static constexpr bool is_wide_character_v = is_wide_character<T>::value;

	/// @brief Represents a pointer type.
	template <typename T>
	concept pointer = std::is_pointer_v<T>;

	/// @brief Represents a character type.
	template <typename T>
	concept character = is_narrow_character_v<T> || is_wide_character_v<T> || std::is_same_v<T, char8> || std::is_same_v<T, char16> || std::is_same_v<T, char32>;
}

using namespace juv;

#ifdef platform_windows
export namespace winrt::OneToolkit::Data
#else
export namespace OneToolkit::Data
#endif
{
	/// @brief Represents the order of bytes.
	enum class Endianness : uint8
	{
		None, Little, Big
	};

	/// @brief Represents the sign of a number.
	/// </summary>
	enum class Signedness : uint8
	{
		Signed, Unsigned
	};

	namespace Text
	{
		/// @brief Represents the kind of character encoding, independent of endianness.
		enum class TextEncodingKind : uint8
		{
			Utf8,
			Utf16,
			Utf32
		};

		/// <summary>
		/// Represents a character encoding scheme.
		/// </summary>
		struct TextEncoding
		{
			TextEncoding(TextEncodingKind const kind, Endianness const endianness) : Kind(kind), ByteOrder(endianness) {}

#ifdef platform_windows
			explicit TextEncoding(Windows::Storage::Streams::UnicodeEncoding const unicodeEncoding) : TextEncoding(GetFromRtEncoding(unicodeEncoding)) {}

			explicit TextEncoding(Windows::Security::Cryptography::BinaryStringEncoding const binaryStringEncoding) : TextEncoding(GetFromRtEncoding(binaryStringEncoding)) {}

			explicit operator Windows::Storage::Streams::UnicodeEncoding() const
			{
				return ConvertToRtEncoding<Windows::Storage::Streams::UnicodeEncoding>(*this);
			}

			explicit operator Windows::Security::Cryptography::BinaryStringEncoding() const
			{
				return ConvertToRtEncoding<Windows::Security::Cryptography::BinaryStringEncoding>(*this);
			}
#endif

			Endianness const ByteOrder;

			TextEncodingKind const Kind;

			template <character CharT, typename Traits = std::char_traits<CharT>>
			std::basic_string_view<CharT, Traits> Name() const noexcept
			{
				if (*this == Utf8())
				{
					return_encoding_specific_string(CharT, "UTF-8");
				}
				else if (*this == Utf16LE())
				{
					return_encoding_specific_string(CharT, "UTF-16LE");
				}
				else if (*this == Utf16BE())
				{
					return_encoding_specific_string(CharT, "UTF-16BE");
				}
				else if (*this == Utf32LE()) 
				{
					return_encoding_specific_string(CharT, "UTF-32LE");
				}
				else if (*this == Utf32BE()) 
				{
					return_encoding_specific_string(CharT, "UTF-32BE");
				}
			}

			bool operator==(TextEncoding const another) const noexcept
			{
				if (Kind == another.Kind)
				{
					if (Kind == TextEncodingKind::Utf8) return true;
					else return ByteOrder == another.ByteOrder;
				}

				return false;
			}

			static TextEncoding Utf8() noexcept
			{
				return { TextEncodingKind::Utf8, Endianness::None };
			}

			static TextEncoding Utf16LE() noexcept
			{
				return { TextEncodingKind::Utf16, Endianness::Little };
			}

			static TextEncoding Utf16BE() noexcept
			{
				return { TextEncodingKind::Utf16, Endianness::Big };
			}

			static TextEncoding Utf32LE() noexcept
			{
				return { TextEncodingKind::Utf32, Endianness::Little };
			}

			static TextEncoding Utf32BE() noexcept
			{
				return { TextEncodingKind::Utf32, Endianness::Big };
			}

			static std::optional<TextEncoding> TryGetFromName(std::string_view const name)
			{
				if (name == "UTF-8") return Utf8();
				else if (name == "UTF-16LE") return Utf16LE();
				else if (name == "UTF-16BE") return Utf16BE();
				else if (name == "UTF-32LE") return Utf32LE();
				else if (name == "UTF-32BE") return Utf32BE();
				else return std::nullopt;
			}

			static std::optional<TextEncoding> TryGetFromName(std::wstring_view const name)
			{
				if (name == L"UTF-8") return Utf8();
				else if (name == L"UTF-16LE") return Utf16LE();
				else if (name == L"UTF-16BE") return Utf16BE();
				else if (name == L"UTF-32LE") return Utf32LE();
				else if (name == L"UTF-32BE") return Utf32BE();
				else return std::nullopt;
			}

			static std::optional<TextEncoding> TryGetFromName(std::u8string_view const name)
			{
				if (name == u8"UTF-8") return Utf8();
				else if (name == u8"UTF-16LE") return Utf16LE();
				else if (name == u8"UTF-16BE") return Utf16BE();
				else if (name == u8"UTF-32LE") return Utf32LE();
				else if (name == u8"UTF-32BE") return Utf32BE();
				else return std::nullopt;
			}

			static std::optional<TextEncoding> TryGetFromName(std::u16string_view const name)
			{
				if (name == u"UTF-8") return Utf8();
				else if (name == u"UTF-16LE") return Utf16LE();
				else if (name == u"UTF-16BE") return Utf16BE();
				else if (name == u"UTF-32LE") return Utf32LE();
				else if (name == u"UTF-32BE") return Utf32BE();
				else return std::nullopt;
			}

			static std::optional<TextEncoding> TryGetFromName(std::u32string_view const name)
			{
				if (name == U"UTF-8") return Utf8();
				else if (name == U"UTF-16LE") return Utf16LE();
				else if (name == U"UTF-16BE") return Utf16BE();
				else if (name == U"UTF-32LE") return Utf32LE();
				else if (name == U"UTF-32BE") return Utf32BE();
				else return std::nullopt;
			}
		private:
#ifdef platform_windows
			template <typename RtEncoding>
			static TextEncoding GetFromRtEncoding(RtEncoding const rtEncoding)
			{
				switch (rtEncoding)
				{
				case RtEncoding::Utf8: return Utf8();
				case RtEncoding::Utf16LE: return Utf16LE();
				case RtEncoding::Utf16BE: return Utf16BE();
				default: throw std::bad_cast();
				}
			};

			template <typename RtEncoding>
			static RtEncoding ConvertToRtEncoding(TextEncoding const textEncoding)
			{
				if (textEncoding == Utf8()) return RtEncoding::Utf8;
				else if (textEncoding == Utf16LE()) return RtEncoding::Utf16LE;
				else if (textEncoding == Utf16BE()) return RtEncoding::Utf16BE;
				else throw std::bad_cast();
			}
#endif
		};

		/// <summary>
		/// Represents a line ending scheme.
		/// </summary>
		enum class LineEnding : uint8
		{
			CR, LF, CRLF, LFCR, Mixed
		};
	}
}

#ifdef platform_windows
export namespace winrt
{
	namespace impl
	{
		inline constexpr hresult error_object_closed{ static_cast<hresult>(0x80000013) }; // RO_E_CLOSED
		inline constexpr hresult error_platform_not_supported{ static_cast<hresult>(0x80131539) }; // COR_E_PLATFORMNOTSUPPORTED
	}

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
#endif

#ifdef platform_windows
using namespace winrt;
#endif

export namespace juv
{
#ifdef platform_windows
	/// <summary>
	/// Represents a Windows Runtime type.
	/// </summary>
	template <typename T>
	concept rt_type = winrt::impl::has_category_v<T>;

	/// <summary>
	/// Represents a Windows Runtime value type.
	/// </summary>
	template <typename T>
	concept rt_value_type = rt_type<T> && !std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>;

	/// <summary>
	/// Represents a Windows Runtime reference type.
	/// </summary>
	template <typename T>
	concept rt_reference_type = rt_type<T> && std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>;
#else
	/// <summary>
	/// Represents a Windows Runtime type.
	/// </summary>
	template <typename T>
	concept rt_type = false;

	/// <summary>
	/// Represents a Windows Runtime value type.
	/// </summary>
	template <typename T>
	concept rt_value_type = false;

	/// <summary>
	/// Represents a Windows Runtime reference type.
	/// </summary>
	template <typename T>
	concept rt_reference_type = false;
#endif

	template <uint8 Size>
	struct sized_character;

	template <>
	struct sized_character<1>
	{
		using type = char8;
	};

	template <>
	struct sized_character<2>
	{
		using type = char16;
	};

	template <>
	struct sized_character<4>
	{
		using type = char32;
	};

	template <uint8 Size, OneToolkit::Data::Signedness Sign>
	struct sized_integer;
	
	template <>
	struct sized_integer<1, OneToolkit::Data::Signedness::Signed>
	{
		using type = int8;
	};

	template <>
	struct sized_integer<1, OneToolkit::Data::Signedness::Unsigned>
	{
		using type = uint8;
	};

	template <>
	struct sized_integer<2, OneToolkit::Data::Signedness::Signed>
	{
		using type = int16;
	};

	template <>
	struct sized_integer<2, OneToolkit::Data::Signedness::Unsigned>
	{
		using type = uint16;
	};

	template <>
	struct sized_integer<4, OneToolkit::Data::Signedness::Signed>
	{
		using type = int32;
	};

	template <>
	struct sized_integer<4, OneToolkit::Data::Signedness::Unsigned>
	{
		using type = uint32;
	};

	template <>
	struct sized_integer<8, OneToolkit::Data::Signedness::Signed>
	{
		using type = int64;
	};

	template <>
	struct sized_integer<8, OneToolkit::Data::Signedness::Unsigned>
	{
		using type = uint64;
	};

	template <uint8 Size>
	struct sized_floating_point;

	template <>
	struct sized_floating_point<4>
	{
		using type = float32;
	};

	template <>
	struct sized_floating_point<8>
	{
		using type = float64;
	};

	template <uint8 Size>
	using sized_character_t = sized_character<Size>::type;

	template <uint8 Size, OneToolkit::Data::Signedness Sign>
	using sized_integer_t = sized_integer<Size, Sign>::type;

	template <uint8 Size>
	using sized_floating_point_t = sized_floating_point<Size>::type;

	/// @brief Represents the string type for the specified string view type.
	template <typename StringView, typename Alloc = std::allocator<typename StringView::value_type>>
	using string_t = std::basic_string<typename StringView::value_type, typename StringView::char_traits, Alloc>;

	/// @brief Represents the string view type for the specified string type.
	template <typename String>
	using string_view_t = std::basic_string_view<typename String::value_type, typename String::char_traits>;

	/// @brief Represents an operating environment.
	enum class platform_id : uint8
	{
		windows, macintosh, other
	};

	/// @brief Provides information about, and means to manipulate, the current environment and platform.
	struct environment : static_t
	{
#ifdef platform_windows
		static constexpr platform_id current_platform = platform_id::windows;
#elif platform_macintosh
		static constexpr platform_id current_platform = platform::macintosh;
#else
		static constexpr platform_id current_platform = platform::other;
#endif

#ifdef platform_windows
		static constexpr OneToolkit::Data::Text::LineEnding default_line_ending = OneToolkit::Data::Text::LineEnding::CRLF;
#else
		static constexpr OneToolkit::Data::Text::LineEnding default_line_ending = OneToolkit::Data::Text::LineEnding::LF;
#endif
	};

	/// @brief Enables communication with a debugger.
	struct debugger : static_t
	{
		template <bool UseWideCharacter>
		struct debug_out
		{
		public:
			debug_out& operator<<(auto const value)
			{
				if constexpr (UseWideCharacter)
				{
					std::wstringstream stream;
					stream << value;
					if constexpr (environment::current_platform == platform_id::windows)
					{
						OutputDebugStringW(stream.str().data());
					}
				}
				else
				{
					std::stringstream stream;
					stream << value;
					if constexpr (environment::current_platform == platform_id::macintosh)
					{
						OutputDebugStringA(stream.str().data());
					}
				}

				return *this;
			}
		private:
			debug_out() = default;
		};

		/// @brief Represents an output stream that can be used to write data as a wide string to the debugger's output window.
		static debug_out<true> wout;

		/// @brief Represents an output stream that can be used to write data as a narrow string to the debugger's output window.
		static debug_out<false> out;

		/// @brief Gets a value that indicates whether a debugger is attached to the process.
		/// @return true if a debugger is attached; otherwise, false.
		static bool is_present() noexcept
		{
#ifdef platform_windows
			return IsDebuggerPresent() != false;
#elif platform_macintosh
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

	/// @brief Represents a property that stores a field internally and exposes it's value to clients through getter and setter methods.
	template <typename T>
	struct auto_property	
	{
	public:
		auto_property(T defaultValue = {}) : m_BackingField(defaultValue) {}

		auto_property(auto_property&&) = delete;

		auto_property(auto_property const&) = delete;

		T operator()() const noexcept
		{
			return m_BackingField;
		}

		void operator()(T newValue)
		{
			m_BackingField = newValue;
		};
	private:
		T m_BackingField;
	};

	/// @brief Safely casts a pointer's value into an integer.
	template <std::integral Integer>
	auto as_value(pointer auto pointer)
	{
		return static_cast<Integer>(reinterpret_cast<uintptr>(pointer));
	}

	/// @brief Safely casts an integer value into a pointer type.
	template <pointer Pointer>
	auto as_pointer(std::integral auto value) noexcept
	{
		return reinterpret_cast<Pointer>(static_cast<uintptr>(value));
	}

	/// @brief Safely dereferences a pointer into an optional value.
	template <pointer Pointer>
	std::optional<typename std::remove_pointer_t<Pointer>> as_optional(Pointer pointer) noexcept
	{
		if (!pointer) return std::nullopt;
		else return { *pointer };
	}

	/// @brief Determines whether a character represents a space.
	template <character Character>
	constexpr bool is_space(Character const character) noexcept
	{
		if constexpr (is_narrow_character_v<Character>)
		{
			return character == ' ' || character == '\f' || character == '\n' || character == '\r' || character == '\t' || character == '\v';
		}
		else if constexpr (is_wide_character_v<Character>)
		{
			return character == L' ' || character == L'\f' || character == L'\n' || character == L'\r' || character == L'\t' || character == L'\v';
		}
		else if constexpr (std::is_same_v<Character, char8>)
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
	}

	/// @brief Determines whether a string is empty or only consists of spaces.
	template <typename StringView>
	constexpr bool has_only_whitespaces(StringView const text) noexcept
	{
		if constexpr (character<typename std::remove_const_t<std::remove_pointer_t<StringView>>>)
		{
			return has_only_whitespaces(std::basic_string_view<typename std::remove_const_t<std::remove_pointer_t<StringView>>>(text));
		}
		else
		{
			return std::all_of(std::begin(text), std::end(text), [](auto const element)
				{
					return is_space(element);
				});
		}
	}

	/// @brief Replaces all occurences of a specific substring in a string.
	template <typename String>
	auto replace_all(String const& text, string_view_t<String> const replaced, string_view_t<String> const replacedWith)
	{
		if (replaced.empty()) return;
		auto position = text.find(replaced);
		while (position != String::npos)
		{
			text.replace(position, replaced.size(), replacedWith);
			position = text.find(replaced, position + replacedWith.size());
		}
	}

#ifdef platform_windows
	hstring trim_end(hstring text, hstring const& trimmed)
	{
		check_hresult(WindowsTrimStringEnd(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(trimmed)), reinterpret_cast<HSTRING*>(put_abi(text))));
		return text;
	}

	hstring trim_start(hstring text, hstring const& trimmed)
	{
		check_hresult(WindowsTrimStringStart(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(trimmed)), reinterpret_cast<HSTRING*>(put_abi(text))));
		return text;
	}

	hstring trim(hstring text, hstring const& trimmed)
	{
		check_hresult(WindowsTrimStringEnd(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(trimmed)), reinterpret_cast<HSTRING*>(put_abi(text))));
		check_hresult(WindowsTrimStringStart(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(trimmed)), reinterpret_cast<HSTRING*>(put_abi(text))));
		return text;
	}

	hstring replace_all(hstring text, hstring const& replaced, hstring const& replacedWith)
	{
		check_hresult(WindowsReplaceString(static_cast<HSTRING>(get_abi(text)), static_cast<HSTRING>(get_abi(replaced)), static_cast<HSTRING>(get_abi(replacedWith)), reinterpret_cast<HSTRING*>(put_abi(text))));
		return text;
	}
#endif
}

#ifdef platform_windows
export namespace winrt::OneToolkit
#else
export namespace OneToolkit
#endif
{
	namespace Data
	{
		namespace Text
		{
			/// @brief Provides the ability to find out and detect the encoding of a text buffer.
			struct TextEncodingHelper : static_t
			{
			public:
				/// @brief Gets whether the buffer contains UTF8 encoded text or not.
				static bool IsUtf8(std::span<std::byte const> const buffer) noexcept
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

					uintptr position = 0;
					uint8 moreCharacters;
					while (position < buffer.size())
					{
						auto value = std::to_integer<uint8>(buffer[position++]);
						if (value == 0) return false;
						else if (value <= 127) moreCharacters = 0;
						else if (value >= 194 && value <= 223) moreCharacters = 1;
						else if (value >= 224 && value <= 239) moreCharacters = 2;
						else if (value >= 240 && value <= 244) moreCharacters = 3;
						else return false;

						// Check secondary characters are in range if we are expecting any.
						while (moreCharacters > 0 && position < buffer.size())
						{
							value = std::to_integer<uint8>(buffer[position++]);
							if (value < 128 || value > 191) return false;
							moreCharacters--;
						}
					}

					return true;
				}

				/// <summary>
				/// Gets whether the buffer contains UTF16 encoded text or not.
				/// </summary>
				static bool IsUtf16(std::span<std::byte const> const buffer, Endianness& endianness) noexcept
				{
					if (IsUtf16Regular(buffer, endianness)) return true;
					else return IsUtf16Ascii(buffer, endianness);
				}

				/// <summary>
				/// Determines the the encoding of a text buffer using various methods.
				/// </summary>
				static std::optional<TextEncoding> TryDetect(std::span<std::byte const> const buffer)
				{					
					if (buffer.size() >= 2 && buffer[0] == ByteOrderMark::Utf16LE[0] && buffer[1] == ByteOrderMark::Utf16LE[1]) return TextEncoding::Utf16LE();
					else if (buffer.size() >= 2 && buffer[0] == ByteOrderMark::Utf16BE[0] && buffer[1] == ByteOrderMark::Utf16BE[1]) return TextEncoding::Utf16BE();
					else if (buffer.size() >= 3 && buffer[0] == ByteOrderMark::Utf8[0] && buffer[1] == ByteOrderMark::Utf8[1] && buffer[2] == ByteOrderMark::Utf8[2]) return TextEncoding::Utf8();
					else if (IsUtf8(buffer)) return TextEncoding::Utf8();
					else
					{
						Endianness endianness;
						if (IsUtf16(buffer, endianness)) return TextEncoding(TextEncodingKind::Utf16, endianness);
						else return std::nullopt;
					}
				}
			private:
				struct ByteOrderMark : static_t
				{
					static constexpr std::array Utf8{ std::byte(0xEF), std::byte(0xBB), std::byte(0xBF) };
					static constexpr std::array Utf16LE{ std::byte(0xFF), std::byte(0xFE) };
					static constexpr std::array Utf16BE{ std::byte(0xFE), std::byte(0xFF) };
				};

				static bool IsUtf16Regular(std::span<std::byte const> const buffer, Endianness& endianness) noexcept
				{
					auto bufferSize = buffer.size();
					if (bufferSize < 2)
					{
						endianness = Endianness::None;
						return false;
					}

					bufferSize--; // Reduce size by 1 so we don't need to worry about bounds checking for pairs of bytes.
					uintptr leControlCharacters = 0;
					uintptr beControlCharacters = 0;
					uint8 ch1, ch2;
					uintptr pos = 0;
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

				static bool IsUtf16Ascii(std::span<std::byte const> const buffer, Endianness& endianness) noexcept
				{
					// Get the number of even and odd nulls.
					uintptr oddNulls = 0;
					uintptr evenNulls = 0;
					for (uintptr position = 0; position < buffer.size(); ++position)
					{
						if (std::to_integer<uint8>(buffer[position]) == 0)
						{
							if (position % 2 == 0) evenNulls++;
							else oddNulls++;
						}
					}

					double const oddNullThreshold = oddNulls * 2.0 / buffer.size();
					double const evenNullThreshold = evenNulls * 2.0 / buffer.size();
					double constexpr expectedNullThreshold = 0.7;
					double constexpr unexpectedNullThreshold = 0.1;
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
			/// Provides helper methods for dealing with line endings.
			/// </summary>
			struct LineEndingHelper : static_t
			{
				/// <summary>
				/// Gets a string that represents the desired line ending.
				/// </summary>
				template <character CharT, typename Traits = std::char_traits<CharT>>
				static constexpr std::basic_string_view<CharT, Traits> GetNewLineString(LineEnding const lineEnding) noexcept
				{
					switch (lineEnding)
					{
					case LineEnding::CR: return_encoding_specific_string(CharT, "\r");
					case LineEnding::LF: return_encoding_specific_string(CharT, "\n");
					case LineEnding::CRLF: return_encoding_specific_string(CharT, "\r\n");
					case LineEnding::LFCR: return_encoding_specific_string(CharT, "\n\r");
					case LineEnding::Mixed: return_encoding_specific_string(CharT, "");
					}
				}

				/// <summary>
				/// Gets whether a string represents a new line.
				/// </summary>
				template <typename StringView>
				static constexpr bool IsNewLineString(StringView const text, LineEnding& lineEnding) noexcept
				{
					if (text == GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CR))
					{
						lineEnding = LineEnding::CR;
						return true;
					}
					else if (text == GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LF))
					{
						lineEnding = LineEnding::LF;
						return true;
					}
					else if (text == GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CRLF))
					{
						lineEnding = LineEnding::CRLF;
						return true;
					}
					else if (text == GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LFCR))
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
				static auto Normalize(StringView const text, LineEnding const lineEnding)
				{
					string_t<StringView> result{ text };
					if (lineEnding == LineEnding::CR)
					{
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CRLF), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CR));
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LFCR), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CR));
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LF), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CR));
					}
					else if (lineEnding == LineEnding::LF)
					{
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CRLF), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LF));
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LFCR), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LF));
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CR), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LF));
					}
					else if (lineEnding == LineEnding::CRLF)
					{
						result = Normalize(text, LineEnding::CR);
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CR), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::CRLF));
					}
					else if (lineEnding == LineEnding::LFCR)
					{
						result = Normalize(text, LineEnding::LF);
						replace_all(result, GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LF), GetNewLineString<typename StringView::value_type, typename StringView::traits_type>(LineEnding::LFCR));
					}

					return result;
				}
			};
		}
	}
}