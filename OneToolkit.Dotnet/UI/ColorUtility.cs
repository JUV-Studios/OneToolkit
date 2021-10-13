using System.Drawing;

namespace OneToolkit.UI
{
	/// <summary>
	/// Provides helper methods for manipulating colors.
	/// </summary>
	public static class ColorUtility
	{
		/// <summary>
		/// Finds the inverse of a single component.
		/// </summary>
		public static byte InvertComponent(byte component) => (byte)(byte.MaxValue - component);

		/// <summary>
		/// Finds the inverse of a color.
		/// </summary>
		public static Color Invert(this Color color) => Color.FromArgb(InvertComponent(color.A), InvertComponent(color.R), InvertComponent(color.G), InvertComponent(color.B));
	}
}