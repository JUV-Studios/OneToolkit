#include "pch.h"
#include "TextEncodingHelper.h"
#include "Data.Text.TextEncodingHelper.g.cpp"

using namespace	winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

namespace winrt::OneToolkit::Data::Text::implementation
{
	namespace BOM
	{
		constexpr std::array<uint8_t, 3> Utf8 { 0xEF, 0xBB, 0xBF };
		constexpr std::array<uint8_t, 2> Utf16LE { 0xFF, 0xFE };
		constexpr std::array<uint8_t, 2> Utf16BE { 0xFE, 0xFF };
	}

	uint8_t TextEncodingHelper::GetBomLength(BinaryStringEncoding encoding) noexcept
	{
		return encoding == BinaryStringEncoding::Utf8 ? 3 : 2;
	}

	bool TextEncodingHelper::IsUtf8(IBuffer const& buffer)
	{
		/*
		UTF8 valid sequences:
		0xxxxxxx  ASCII
		110xxxxx 10xxxxxx  2-byte
		1110xxxx 10xxxxxx 10xxxxxx  3-byte
		11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  4-byte

		Width in UTF8:
		0-127 - 1 byte
		194-223	- 2 bytes
		224-239	- 3 bytes
		/240-244 - 4 bytes
		Subsequent chars are in the range 128-191
		*/

		uint32_t pos = 0;
		int	moreCharacters;
		auto bufferData = buffer.data();
		auto bufferSize = buffer.Length();
		while (pos < bufferSize)
		{
			auto character = bufferData[pos++];
			if (character == 0) throw hresult_invalid_argument(L"The buffer you passed doesn't contain valid text.");
			else if (character <= 127) moreCharacters = 0;
			else if (character >= 194 && character <= 223) moreCharacters = 1;
			else if (character >= 224 && character <= 239) moreCharacters = 2;
			else if (character >= 240 && character <= 244) moreCharacters = 3;
			else return false;

			// Check secondary chars are in range if we are expecting any
			while (moreCharacters && pos < bufferSize)
			{
				character = bufferData[pos++];
				if (character < 128 || character > 191) return false;
				moreCharacters--;
			}
		}

		// If we get here, only valid UTF-8 sequences have been processed
		return true;
	}

	bool TextEncodingHelper::IsUtf16(IBuffer const& buffer, Endianness& endianness)
	{
		auto bufferData = buffer.data();
		auto bufferSize = buffer.Length();
		if (IsUtf16Regular(bufferData, bufferSize, endianness)) return true;
		else return IsUtf16Ascii(bufferData, bufferSize, endianness);
	}

	bool TextEncodingHelper::IsUtf16Regular(uint8_t const* buffer, uint32_t size, Endianness& endianness) noexcept
	{
		if (size < 2) return false;
		// Reduce size by 1 so we don't need to worry about bounds checking for pairs of bytes
		size--;
		int leControlCharacters = 0;
		int beControlCharacters = 0;
		uint8_t ch1, ch2;
		uint32_t pos = 0;
		while (pos < size)
		{
			ch1 = buffer[pos++];
			ch2 = buffer[pos++];
			if (ch1 == 0 && (ch2 == 0x0a || ch2 == 0x0d)) beControlCharacters++;
			else if (ch2 == 0 && (ch1 == 0x0a || ch1 == 0x0d)) leControlCharacters++;
			// If we are getting both LE and BE control characters, then this file is not utf16
			if (leControlCharacters && beControlCharacters) return false;
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
		else return false;
	}

	bool TextEncodingHelper::IsUtf16Ascii(uint8_t const* buffer, uint32_t size, Endianness& endianness) noexcept
	{
		int oddNulls = 0;
		int evenNulls = 0;
		// Get even nulls
		uint32_t pos = 0;
		while (pos < size)
		{
			if (buffer[pos] == 0) evenNulls++;
			pos += 2;
		}

		// Get odd nulls
		pos = 1;
		while (pos < size)
		{
			if (buffer[pos] == 0) oddNulls++;
			pos += 2;
		}

		double evenNullThreshold = (evenNulls * 2.0) / size;
		double oddNullThreshold = (oddNulls * 2.0) / size;
		double expectedNullThreshold = 70 / 100.0;
		double unexpectedNullThreshold = 10 / 100.0;
		if (evenNullThreshold < unexpectedNullThreshold && oddNullThreshold > expectedNullThreshold) // Lots of odd nulls, low number of even nulls
		{
			endianness = Endianness::Little;
			return true;
		}
		else if (oddNullThreshold < unexpectedNullThreshold && evenNullThreshold > expectedNullThreshold) // Lots of even nulls, low number of odd nulls
		{
			endianness = Endianness::Big;
			return true;
		}
		else return false; // Don't know
	}

	IReference<BinaryStringEncoding> TextEncodingHelper::TryDetectWithBom(IBuffer const& buffer) noexcept
	{
		auto bufferData = buffer.data();
		auto bufferLength = buffer.Length();
		if (bufferLength >= 2 && bufferData[0] == BOM::Utf16LE[0] && bufferData[1] == BOM::Utf16LE[1]) return BinaryStringEncoding::Utf16LE;
		else if (bufferLength >= 2 && bufferData[0] == BOM::Utf16BE[0] && bufferData[1] == BOM::Utf16BE[1]) return BinaryStringEncoding::Utf16BE;
		else if (bufferLength >= 3 && bufferData[0] == BOM::Utf8[0] && bufferData[1] == BOM::Utf8[1] && bufferData[2] == BOM::Utf8[2]) return BinaryStringEncoding::Utf8;
		else return nullptr;
	}

	IReference<BinaryStringEncoding> TextEncodingHelper::TryDetect(IBuffer const& buffer) noexcept
	{
		auto bomValue = TryDetectWithBom(buffer);
		if (bomValue != nullptr) return bomValue.Value();
		else
		{
			try
			{
				if (IsUtf8(buffer)) return BinaryStringEncoding::Utf8;
				else
				{
					Endianness endianness;
					if (IsUtf16(buffer, endianness)) return endianness == Endianness::Little ? BinaryStringEncoding::Utf16LE : BinaryStringEncoding::Utf16BE;
					else return nullptr;
				}
			}
			catch (hresult_invalid_argument const&)
			{
				return nullptr;
			}
		}
	}
}