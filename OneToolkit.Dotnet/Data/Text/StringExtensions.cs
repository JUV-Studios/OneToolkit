namespace OneToolkit.Data.Text
{
	/// <summary>
	/// Provides high performance extension methods for strings.
	/// </summary>
	public static unsafe class StringExtensions
	{
		/// <summary>
		/// Copies a string into a new instance.
		/// </summary>
		public static string Clone(this string text)
		{
			fixed (char* buffer = text) return new string(buffer);
		}

		/// <summary>
		/// Replaces a single character in a string without allocating a new one.
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
