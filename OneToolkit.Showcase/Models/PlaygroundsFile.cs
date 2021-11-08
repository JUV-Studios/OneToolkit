using Windows.Storage;

namespace OneToolkit.Showcase.Models
{
	public sealed record PlaygroundsFile(StorageFile File, bool UseWinUI3)
	{
	}
}
