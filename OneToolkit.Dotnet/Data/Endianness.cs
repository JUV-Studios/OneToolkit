using System;

namespace OneToolkit.Data
{
	/// <summary>
	/// Represents the order of bytes.
	/// </summary>
#pragma warning disable CS1591
	public enum Endianness : byte
	{
		Little, Big
	};
#pragma warning restore CS1591

	public static class EndiannessHelper
	{
		/// <summary>
		/// Gets the native endianness of the local machine.
		/// </summary>
		public static Endianness GetNativeEndianness() => BitConverter.IsLittleEndian ? Endianness.Little : Endianness.Big;
	}
}