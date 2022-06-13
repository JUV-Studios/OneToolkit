using Windows.UI;

namespace OneToolkit.UI.Xaml.Controls.TextEditor.Lexer
{
    public enum TokenType
	{
        None, Keyword, Identifier, Operator, Type, Delimiter, Attribute, Other
    }

    public struct HighlightToken
    {
        public int StartIndex;
        public uint Length;
        public TokenType TokenType;
        public Color Colour;
    }
}
