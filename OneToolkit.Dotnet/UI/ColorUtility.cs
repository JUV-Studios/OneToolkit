using System.Drawing;

namespace OneToolkit.UI
{
	public static class ColorUtility
	{
		public static Color Invert(Color color) => Color.FromArgb(InvertComponent(color.A), InvertComponent(color.R), InvertComponent(color.G), InvertComponent(color.B));

		public static byte InvertComponent(byte component) => (byte)(byte.MaxValue - component);
	}
}
