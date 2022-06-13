namespace OneToolkit.UI
{
	/// <summary>
	/// Provides static helper methods for building your own tabbing system.
	/// </summary>
	public static class TabbingHelper
	{
		/// <summary>
		/// Gets the tab index of the tab to switch next.
		/// </summary>
		public static dynamic GetNewSelectionIndex(dynamic currentIndex, dynamic collectionSize, bool isReverse)
		{
			if (collectionSize <= 1) return currentIndex;
			else if (isReverse) return currentIndex == 0 ? collectionSize - 1 : currentIndex - 1;
			else return currentIndex == collectionSize - 1 ? 0 : currentIndex + 1;
		}
	}
}