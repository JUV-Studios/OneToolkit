namespace OneToolkit.Lifecycle
{
	public interface IParentHolder<Parent> where Parent : class
	{
		void Initialize(Parent parent);
	}
}
