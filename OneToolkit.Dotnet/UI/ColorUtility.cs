﻿using System.Drawing;

namespace OneToolkit.UI
{
	/// <summary>
	/// Provides static helper methods for manipulating colors.
	/// </summary>
	public static class ColorUtility
	{
		/// <summary>
		/// Finds the inverse of a color.
		/// <returns></returns>
		public static Color Invert(Color color) => Color.FromArgb(InvertComponent(color.A), InvertComponent(color.R), InvertComponent(color.G), InvertComponent(color.B));

		/// <summary>
		/// Finds the inverse of a single component.
		/// </summary>
		public static byte InvertComponent(byte component) => (byte)(byte.MaxValue - component);
	}
}