namespace OneToolkit.UI.Xaml.Controls.TextEditor.Languages
{
    /* public class CurlyBracesIndentationProvider : IndentationProvider
    {
        public override int GuessIndentLevel(string text, int index)
        {
            var lineText = ExtractLineText(ref text, index - 2).TrimEnd('\r');
            int indentLevel = GetIndentLevel(lineText);
            if (lineText.Trim().EndsWith("{")) return indentLevel + TabWidth;
            lineText = lineText.TrimStart(' ');
            if (indentLevel >= TabWidth && lineText.EndsWith('}')) return indentLevel - TabWidth;
            return indentLevel;
        }
    } */
}
