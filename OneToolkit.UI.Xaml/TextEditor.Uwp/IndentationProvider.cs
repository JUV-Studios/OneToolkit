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

using OneToolkit.Data.Text;
using System.Collections.Generic;

namespace OneToolkit.UI.Xaml.Controls.TextEditor
{
    public interface IIndentationProvider
    {
        public byte TabSize { get; set; }

        public IReadOnlyList<char> BlockStart { get; }

        public IReadOnlyList<char> BlockEnd { get; }

        public bool NeedIndentation(string text);

        public ulong GuessIndentLevel(string text, long index);
    }

    public static class IndentationProvider
    {
        public static void ExtractLineText(string text, int loc, int lineEnding)
        {
            int i = loc, j = loc;
            while (i >= 0 && text[i] != '\r') i--;
            while (j < text.Length && text[j] != '\r') j++;
            text = i == j ? text = text.Substring(i + 1, j - i) : text.Substring(i + 1, j - i - 1);
        }

        public static ulong GetIndentLevel(this IIndentationProvider provider, string lineText)
        {
            ulong indentLevel = 0;
            foreach (var c in lineText)
            {
                if (c == ' ') indentLevel++;
                else break;
            }

            var tabSize = provider.TabSize;
            return indentLevel / tabSize * tabSize;
        }

        /* readonly List<int> blockStart = new List<int> { '[', '{', '(', '\'', '"' };
        readonly List<int> blockEnd = new List<int> { ']', '}', ')', '\'', '"' }; */

        /* protected bool IsBracketOpen(ref string text)
        {
            int chr = -2;
            var stack = new Stack<int>();

            using (var stream = new StringReader(text))
            {
                chr = stream.Read();

                while (chr != -1)
                {

                    if (blockStart.Contains(chr))
                        stack.Push(chr);

                    if (blockEnd.Contains(chr) &&
                        stack.Count > 0 &&
                        stack.Peek() == chr)
                    {
                        stack.Pop();
                    }
                }
            }

            return stack.Count > 0;
        } 
    } */
    }
}