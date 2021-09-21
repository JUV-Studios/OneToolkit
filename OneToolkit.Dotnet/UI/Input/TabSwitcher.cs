﻿namespace OneToolkit.UI.Input
{
	/// <summary>
	/// Provides static helper methods for building your own tabbing system.
	/// </summary>
	public static class TabSwitcher
	{
		/// <summary>
		/// Gets the tab index to be selected next.
		/// </summaray>
		public static ulong GetNewSelectionIndex(ulong currentIndex, ulong collectionSize, bool reverse)
		{
			if (collectionSize <= 1) return currentIndex;
			else if (reverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
			else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
		}
	}
}