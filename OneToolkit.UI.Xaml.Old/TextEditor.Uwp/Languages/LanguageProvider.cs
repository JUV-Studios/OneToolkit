using System.Collections.Generic;
using System.Linq;

namespace OneToolkit.UI.Xaml.Controls.TextEditor.Languages
{
    /* public static class LanguageProvider
    {
        public static readonly List<SyntaxLanguage> CodeLanguages = new() { new PythonSyntaxLanguage(), new PlainTextLanguage() };

        public static bool LocateLanguage(string fileType, out SyntaxLanguage foundLanguage)
        {
            for (int i = 0; i < CodeLanguages.Count; i++)
            {
                var language = CodeLanguages[i];
                if (language.Id == fileType.ToLower())
                {
                    foundLanguage = language;
                    return true;
                }
            }

            foundLanguage = CodeLanguages.Last();
            return false;
        }
    } */
}
