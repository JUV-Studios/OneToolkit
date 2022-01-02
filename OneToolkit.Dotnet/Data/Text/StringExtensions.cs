namespace OneToolkit.Data.Text
{
	/// <summary>
	/// Provides high performance extension methods for strings.
	/// </summary>
	public static unsafe class StringExtensions
	{
		/// <summary>
		/// Creates a copy of a string and returns an handle to it.
		/// </summary>
		public static string Copy(this string text)
		{
			fixed (char* buffer = text) return new string(buffer);
		}

		/// <summary>
		/// Replaces all occurrences of a specific character in a string without allocating a new one.
		/// </summary>
		public static void ReplaceCharacter(this string text, char replaced, char replacedWith)
		{
			fixed (char* buffer = text)
			{
				for (int i = 0; i < text.Length; i++)
				{
					if (buffer[i] == replaced) buffer[i] = replacedWith;
				}
			}
		}
	}
}
