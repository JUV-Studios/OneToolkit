using System;
using System.Runtime.CompilerServices;
using System.Text;

namespace OneToolkit.Data.Text
{
	/// <summary>
	/// Represents an encoding of a text buffer.
	/// </summary>
#pragma warning disable CS1591
	public enum TextEncoding : byte
	{
		Unknown, Ascii, Utf8, Utf16LE, Utf16BE
	}
#pragma warning restore CS1591

	/// <summary>
	/// Provides the ability to find out and detect the encoding of a text buffer.
	/// </summary>
	public static class TextEncodingHelper
	{
		static class BOM
		{
			public static readonly byte[] Utf8 = new byte[] { 0xEF, 0xBB, 0xBF };
			public static readonly byte[] Utf16LE = new byte[] { 0xFF, 0xFE };
			public static readonly byte[] Utf16BE = new byte[] { 0xFE, 0xFF };
		}

		/// <summary>
		/// Gets whether the buffer contains ASCII encoded text or not.
		/// </summary>
		public static bool IsAscii(ReadOnlySpan<byte> buffer)
		{
			foreach (var chr in buffer)
			{
				if (chr > 128) return false;
			}

			return true;
		}

		/// <summary>
		/// Gets whether the buffer contains UTF8 encoded text or not.
		/// </summary>
		public static bool IsUtf8(ReadOnlySpan<byte> buffer)
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

			int pos = 0;
			byte moreCharacters;
			while (pos < buffer.Length)
			{
				var character = buffer[pos++];
				if (character == 0) return false;
				else if (character <= 127) moreCharacters = 0;
				else if (character >= 194 && character <= 223) moreCharacters = 1;
				else if (character >= 224 && character <= 239) moreCharacters = 2;
				else if (character >= 240 && character <= 244) moreCharacters = 3;
				else return false;

				// Check secondary character are in range if we are expecting any.
				while (moreCharacters > 0 && pos < buffer.Length)
				{
					character = buffer[pos++];
					if (character < 128 || character > 191) return false;
					moreCharacters--;
				}
			}

			return true;
		}

		/// <summary>
		/// Gets whether the buffer contains UTF16 encoded text or not.
		/// </summary>
		public static bool IsUtf16(ReadOnlySpan<byte> buffer, out Endianness endianness)
		{
			if (IsUtf16Regular(buffer, out endianness)) return true;
			else return IsUtf16Ascii(buffer, out endianness);
		}

		private static bool IsUtf16Regular(ReadOnlySpan<byte> buffer, out Endianness endianness)
		{
			var size = buffer.Length;
			if (size < 2)
			{
				endianness = Endianness.None;
				return false;
			}

			size--; // Reduce size by 1 so we don't need to worry about bounds checking for pairs of bytes.
			int leControlCharacters = 0;
			int beControlCharacters = 0;
			byte ch1, ch2;
			int pos = 0;
			while (pos < size)
			{
				ch1 = buffer[pos++];
				ch2 = buffer[pos++];
				if (ch1 == 0 && (ch2 == 0x0a || ch2 == 0x0d)) beControlCharacters++;
				else if (ch2 == 0 && (ch1 == 0x0a || ch1 == 0x0d)) leControlCharacters++;
				if (leControlCharacters > 0 && beControlCharacters > 0)
				{
					// If we're finding both LE and BE control characters, it's not UTF16.
					endianness = Endianness.None;
					return false;
				}
			}

			if (leControlCharacters > 0)
			{
				endianness = Endianness.Little;
				return true;
			}
			else if (beControlCharacters > 0)
			{
				endianness = Endianness.Big;
				return true;
			}
			else
			{
				endianness = Endianness.None;
				return false;
			}
		}

		private static bool IsUtf16Ascii(ReadOnlySpan<byte> buffer, out Endianness endianness)
		{
			// Get the number of even and odd nulls.
			int oddNulls = 0;
			int evenNulls = 0;
			for (int position = 0; position < buffer.Length; ++position)
			{
				if (buffer[position] == 0)
				{
					if (position % 2 == 0) evenNulls++;
					else oddNulls++;
				}
			}

			double evenNullThreshold = (evenNulls * 2.0) / buffer.Length;
			double oddNullThreshold = (oddNulls * 2.0) / buffer.Length;
			double expectedNullThreshold = 70 / 100.0;
			double unexpectedNullThreshold = 10 / 100.0;
			if (evenNullThreshold < unexpectedNullThreshold && oddNullThreshold > expectedNullThreshold) // Lots of odd nulls, low number of even nulls.
			{
				endianness = Endianness.Little;
				return true;
			}
			else if (oddNullThreshold < unexpectedNullThreshold && evenNullThreshold > expectedNullThreshold) // Lots of even nulls, low number of odd nulls.
			{
				endianness = Endianness.Big;
				return true;
			}
			else
			{
				endianness = Endianness.None;
				return false;
			}
		}

		/// <summary>
		/// Attempts to detect the encoding using various methods.
		/// </summary>
		public static TextEncoding Detect(ReadOnlySpan<byte> buffer)
		{
			if (buffer.Length >= 2 && buffer[0] == BOM.Utf16LE[0] && buffer[1] == BOM.Utf16LE[1]) return TextEncoding.Utf16LE;
			else if (buffer.Length >= 2 && buffer[0] == BOM.Utf16BE[0] && buffer[1] == BOM.Utf16BE[1]) return TextEncoding.Utf16BE;
			else if (buffer.Length >= 3 && buffer[0] == BOM.Utf8[0] && buffer[1] == BOM.Utf8[1] && buffer[2] == BOM.Utf8[2]) return TextEncoding.Utf8;
			else if (IsUtf8(buffer)) return IsAscii(buffer) ? TextEncoding.Ascii : TextEncoding.Ascii;
			else if (IsUtf16(buffer, out Endianness endianness)) return endianness == Endianness.Little ? TextEncoding.Utf16LE : TextEncoding.Utf16BE;
			else return TextEncoding.Unknown;
		}

		/// <summary>
		/// Gets the bytes representation of a string for the specified encoding.
		/// </summary>
		public static byte[] GetBuffer(string text, TextEncoding textEncoding = TextEncoding.Unknown) => textEncoding.AsEncoding().GetBytes(text);

		/// <summary>
		/// Gets the string representation of a buffer for the specified encoding.
		/// </summary>
		public static unsafe string GetString(ReadOnlySpan<byte> buffer, TextEncoding textEncoding = TextEncoding.Unknown)
		{
			fixed (byte* data = buffer) return textEncoding.AsEncoding().GetString(data, buffer.Length);
		}

		/// <summary>
		/// Gets the encoding information for the specified text encoding.
		/// </summary>
		public static Encoding AsEncoding(this TextEncoding textEncoding) => textEncoding switch
		{
			TextEncoding.Ascii => Encoding.ASCII,
			TextEncoding.Utf8 => Encoding.UTF8,
			TextEncoding.Utf16LE => Encoding.Unicode,
			TextEncoding.Utf16BE => Encoding.BigEndianUnicode,
			_ => Encoding.Unicode
		};
	}
}