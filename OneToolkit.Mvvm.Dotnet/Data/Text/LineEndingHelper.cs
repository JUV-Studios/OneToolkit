﻿namespace OneToolkit.Data.Text
{
	/// <summary>
	/// Represents a line ending scheme.
	/// </summary>
#pragma warning disable CS1591
	public enum LineEnding : byte
	{
		CR, LF, CRLF, Mixed
	}
#pragma warning restore CS1591

	/// <summary>
	/// Provides helper methods for dealing with line endings.
	/// </summary>
	public static class LineEndingHelper
	{
		/// <summary>
		/// Gets whether a string represents a new line.
		/// </summary>
		public static bool IsNewLineString(string text, out LineEnding lineEnding)
		{
			if (text == "\r")
			{
				lineEnding = LineEnding.CR;
				return true;
			}
			else if (text == "\n")
			{
				lineEnding = LineEnding.LF;
				return true;
			}
			else if (text == "\r\n" || text == "\n\r")
			{
				lineEnding = LineEnding.CRLF;
				return true;
			}
			else
			{
				lineEnding = LineEnding.Mixed;
				return false;
			}
		}

		/// <summary>
		/// Gets a string that represents the desired line ending.
		/// </summary>
		public static string GetNewLineString(LineEnding lineEnding) => lineEnding switch
		{
			LineEnding.CR => "\r",
			LineEnding.LF => "\n",
			LineEnding.CRLF => "\r\n",
			_ => string.Empty
		};

		/// <summary>
		/// Normalizes a string to use a single line ending for every new line.
		/// </summary>
		public static string Normalize(string text, LineEnding lineEnding)
		{
			if (lineEnding == LineEnding.CR)
			{
				text = text.Replace("\r\n", "\r").Replace("\n\r", "\r");
				text.ReplaceCharacter('\n', '\r');
			}
			else if (lineEnding == LineEnding.LF)
			{
				text = text.Replace("\r\n", "\n").Replace("\n\r", "\n");
				text.ReplaceCharacter('\r', '\n');
			}
			else if (lineEnding == LineEnding.CRLF)
			{
				text = Normalize(text, LineEnding.CR).Replace("\r", "\r\n");
			}

			return text;
		}
	}
}
