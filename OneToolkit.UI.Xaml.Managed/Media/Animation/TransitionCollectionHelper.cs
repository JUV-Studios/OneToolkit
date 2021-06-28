using Windows.UI.Xaml.Media.Animation;

namespace OneToolkit.UI.Xaml.Media.Animation
{
	public static class TransitionCollectionHelper
	{
		public static void ApplySingleTransition(ref TransitionCollection transitionCollection, Transition transition)
		{
			if (transitionCollection == null) transitionCollection = new();
			else if (transitionCollection.Count > 0) transitionCollection.Clear();
			transitionCollection.Add(transition);
		}
	}
}
