namespace OneToolkit.Data
{
	public static unsafe class HighPerformance
	{
		/// <summary>
		/// Replaces all occurences of a specific character with another character in a string without creating a new one.
		/// </summary>
		/// <remarks>The specified string must be truly yours. Modifing existing strings can cause issues later on.</remarks>
		public static void UnsafeReplace(this string text, char oldChar, char newChar)
		{
			fixed (char* textPtr = text)
			{
				for (int index = 0; index < text.Length; ++index)
				{
					if (textPtr[index] == oldChar) textPtr[index] = newChar;
				}
			}
		}
	}
}
