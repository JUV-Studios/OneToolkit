using System;
using System.Collections.Generic;
using System.Text;

namespace OneToolkit.UI.Input
{
	public static class TabSwitcher
	{
		public static ulong GetNewSelectionIndex(ulong currentIndex, ulong collectionSize, bool reverse)
		{
			if (collectionSize <= 1) return currentIndex;
			else if (reverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
			else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
		}
	}
}