// Copyright (c) Adnan Umer. All rights reserved. Follow me @aztnan
// Email: aztnan@outlook.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

namespace OneToolkit.UI.Xaml.Controls.TextEditor.Languages
{
    internal sealed class PythonIndentationProvider //: IndentationProvider
    {
        bool NeedIndentation(string text)
        {
            if (text.TrimEnd(' ').EndsWith(":")) return true;
            int j = 0;
            while (j < text.Length && text[j] != '#') { j++; }

            if (j == text.Length)
                return false;

            text = text.Substring(0, j);

            return text.TrimEnd(' ').EndsWith(":");
        }

        /* int GuessIndentLevel(string text)
        {
            int indentLevel = GetIndentLevel(text);

            if (NeedIndentation(text))
                indentLevel += 4;

            return indentLevel;
        } */


        /* public override int GuessIndentLevel(string text, int index)
        {
            var lineText = ExtractLineText(ref text, index - 2).TrimEnd('\r');
            int indentLevel = GetIndentLevel(lineText);
            if (NeedIndentation(lineText)) return indentLevel + TabWidth;
            lineText = lineText.TrimStart(' ');
            if (indentLevel >= TabWidth && (lineText.StartsWith("return") || lineText.StartsWith("pass"))) return indentLevel - TabWidth;
            return indentLevel;
        }*/
    }
}