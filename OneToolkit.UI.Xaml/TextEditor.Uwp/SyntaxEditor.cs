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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.System;
using Windows.UI.Text;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls;
using OneToolkit.UI.Xaml.Controls.TextEditor.Lexer;

namespace OneToolkit.UI.Xaml.Controls.TextEditor
{
	public enum FindArea
	{
		Selection, WholeDocument
	}

	public enum SyntaxEditorMode
	{
		RichText, PlainText, SourceCode
	}

	public struct FindOptions
	{
		public bool MatchCase;
		public bool UseRegex;
		public FindArea Location;
	}

	public struct TextSelectionInfo
	{
		public int SelectionEnd;
		public int SelectionStart;
	}

	public static class TextSelectionHelper
	{
		public static bool IsValid(TextSelectionInfo textSelection) => textSelection.SelectionStart != textSelection.SelectionEnd;
	}

	public sealed class SyntaxEditor : RichEditBox
	{
		public SyntaxEditor()
		{
			KeyDown += Editor_KeyDown;
			var xamlControlResources = from dictionary in Application.Current.Resources.MergedDictionaries
									   where dictionary is XamlControlsResources
									   select dictionary;

			if (xamlControlResources.Any()) Style = xamlControlResources.First()["DefaultRichEditBoxStyle"] as Style;
		}

		private HistoryStack HistoryProvider;

		public int LinesCount => Text.Split('\r').Length;

		public string Text
		{
			get
			{
				TextDocument.GetText(EditorMode == SyntaxEditorMode.RichText ? TextGetOptions.FormatRtf : TextGetOptions.None, out string text);
				return text;
			}
			set
			{
				TextDocument.SetText(EditorMode == SyntaxEditorMode.RichText ? TextSetOptions.FormatRtf : TextSetOptions.None, value);
			}
		}

		public SyntaxEditorMode EditorMode
		{
			get => (SyntaxEditorMode)GetValue(EditorModeProperty);
			set => SetValue(EditorModeProperty, value);
		}

		public static DependencyProperty EditorModeProperty { get; } = DependencyProperty.Register(nameof(EditorMode), typeof(SyntaxEditorMode), typeof(SyntaxEditor), new(SyntaxEditorMode.RichText, DependencyPropertyChanged));

		public IDictionary<string, TokenType> SuggestionsList { get; } = new Dictionary<string, TokenType>();

		private async void Editor_KeyDown(object sender, KeyRoutedEventArgs e)
		{
			switch (e.Key)
			{
				case VirtualKey.Tab:
					e.Handled = true;
					TextDocument.Selection.TypeText("\t");
					break;
				case VirtualKey.Escape:
					e.Handled = true;
					await FocusManager.TryFocusAsync(FocusManager.FindNextFocusableElement(FocusNavigationDirection.Down), FocusState.Keyboard);
					break;
				default:
					e.Handled = false;
					break;
			}
		}

		private async void Editor_Paste(object sender, TextControlPasteEventArgs e)
		{
			e.Handled = true;
			var content = Clipboard.GetContent();
			if (content.Contains("Text"))
			{
				var text = await content.GetTextAsync();
				TextDocument.Selection.TypeText(text);
			}
		}

		private IAsyncAction PrintAsync() => Print().AsAsyncAction();

		private Task Print()
		{
			throw new NotImplementedException();
		}

		private static void DependencyPropertyChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
		{
			var target = sender as SyntaxEditor;
			if (e.Property == EditorModeProperty)
			{
				target.HistoryProvider?.Clear();
				var editorMode = (SyntaxEditorMode)e.NewValue;
				if (editorMode == SyntaxEditorMode.RichText)
				{
					target.Paste -= target.Editor_Paste;
					target.TextDocument.UndoLimit = uint.MaxValue;
					target.ClipboardCopyFormat = RichEditClipboardFormat.AllFormats;
					target.DisabledFormattingAccelerators = DisabledFormattingAccelerators.None;
					if (target.HistoryProvider is not RichTextHistoryStack) target.HistoryProvider = new RichTextHistoryStack(target);
				}
				else
				{
					target.Paste += target.Editor_Paste;
					target.TextDocument.UndoLimit = 0;
					target.ClipboardCopyFormat = RichEditClipboardFormat.PlainText;
					target.DisabledFormattingAccelerators = DisabledFormattingAccelerators.All;
					if (target.HistoryProvider is not PlainTextHistoryStack) target.HistoryProvider = new PlainTextHistoryStack(target);
				}
			}
		}

		#region Text suggestions

		public IEnumerable<KeyValuePair<string, TokenType>> FindSuggestions(string text)
		{
			return from suggestion in SuggestionsList
				   where suggestion.Key.Contains(text) || text.Contains(suggestion.Key)
				   select suggestion;
		}

		#endregion

		#region Highlighting

		private static volatile bool HighlightLock = false;

		public bool HighlightTokens(IReadOnlyList<HighlightToken> tokens)
		{
			if (tokens.Count <= 0 || HighlightLock) return false;
			HighlightLock = true;
			foreach (var token in tokens)
			{
				var range = TextDocument.GetRange(token.StartIndex, Convert.ToInt32(token.StartIndex + token.Length));
				if (range.CharacterFormat.ForegroundColor != token.Colour) range.CharacterFormat.ForegroundColor = token.Colour;
			}

			HighlightLock = false;
			return true;
		}

		#endregion

		#region Interaction

		public void SelectAll()
		{
			Focus(FocusState.Keyboard);
			TextDocument.Selection.StartPosition = 0;
			TextDocument.Selection.EndOf(TextRangeUnit.Story, true);
		}

		public void ClearSelection() => TextDocument.Selection.EndPosition = TextDocument.Selection.StartPosition;

		public string[] FindText(string text, FindOptions options)
		{
			return null;
		}

		public void ScrollToLine(int line, bool extend)
		{
			Focus(FocusState.Keyboard);
			TextDocument.Selection.HomeKey(TextRangeUnit.Story, false);
			TextDocument.Selection.MoveStart(TextRangeUnit.Line, line - 1);
			if (extend)
			{
				TextDocument.Selection.Expand(TextRangeUnit.Line);
				TextDocument.Selection.EndPosition = TextDocument.Selection.EndPosition - 1;
			}
		}

		#endregion
	}
}