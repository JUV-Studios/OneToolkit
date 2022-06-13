using System.Collections.Generic;
using Windows.UI.Xaml;
using Microsoft.Toolkit.Mvvm.ComponentModel;

namespace OneToolkit.UI.Xaml.Controls.TextEditor
{
	internal readonly struct PlainTextHistoryData
	{
		public PlainTextHistoryData(string text, int selectionIndex)
		{
			Text = text;
			SelectionIndex = selectionIndex;
		}

		public readonly string Text;

		public readonly int SelectionIndex;

		public void Apply(SyntaxEditor syntaxEditor)
		{
			syntaxEditor.Text = Text;
			syntaxEditor.Focus(FocusState.Keyboard);
			syntaxEditor.TextDocument.Selection.StartPosition = SelectionIndex;
		}
	}

	internal abstract class HistoryStack : ObservableObject
	{
		public HistoryStack(SyntaxEditor targetEditor)
		{
			_TargetEditor = targetEditor;
		}

		private SyntaxEditor _TargetEditor;

		protected SyntaxEditor TargetEditor => _TargetEditor;

		public abstract bool CanUndo { get; }

		public abstract bool CanRedo { get; }

		public abstract void Undo();

		public abstract void Redo();

		public abstract void Clear();

		public virtual bool TryPeekUndo(out string text)
		{
			text = string.Empty;
			return false;
		}

		public virtual bool TryPeekRedo(out string text)
		{
			text = string.Empty;
			return false;
		}
	}

	internal sealed class RichTextHistoryStack : HistoryStack
	{
		public RichTextHistoryStack(SyntaxEditor targetEditor) : base(targetEditor)
		{
			targetEditor.TextChanged += TargetEditor_TextChanged;
		}

		public override bool CanUndo => TargetEditor.Document.CanUndo();

		public override bool CanRedo => TargetEditor.Document.CanRedo();

		public override void Undo() => TargetEditor.Document.Undo();

		public override void Redo() => TargetEditor.Document.Redo();

		public override void Clear() => TargetEditor.TextDocument.ClearUndoRedoHistory();

		private void TargetEditor_TextChanged(object sender, RoutedEventArgs e)
		{
			OnPropertyChanged(nameof(CanUndo));
			OnPropertyChanged(nameof(CanRedo));
		}
	}

	internal sealed class PlainTextHistoryStack : HistoryStack
	{
		public PlainTextHistoryStack(SyntaxEditor targetEditor) : base(targetEditor)
		{
			targetEditor.TextChanged += TargetEditor_TextChanged;
		}

		private readonly Stack<PlainTextHistoryData> UndoStack = new();

		private readonly Stack<PlainTextHistoryData> RedoStack = new();

		public override bool CanUndo => UndoStack.Count > 0;

		public override bool CanRedo => RedoStack.Count > 0;

		public override void Undo()
		{
			if (CanUndo)
			{
				bool canRedoBefore = CanRedo;
				RedoStack.Push(UndoStack.Pop());
				UndoStack.Pop().Apply(TargetEditor);
				OnPropertyChanged(nameof(CanUndo));
				if (!canRedoBefore) OnPropertyChanged(nameof(CanRedo));
			}
		}

		public override void Redo()
		{
			if (CanRedo)
			{
				RedoStack.Pop().Apply(TargetEditor);
				OnPropertyChanged(nameof(CanRedo));
			}
		}

		public override void Clear()
		{
			UndoStack.Clear();
			RedoStack.Clear();
			OnPropertyChanged(nameof(CanUndo));
			OnPropertyChanged(nameof(CanRedo));
		}

		public override bool TryPeekUndo(out string text)
		{
			if (CanUndo)
			{
				text = UndoStack.Peek().Text;
				return true;
			}
			else
			{
				text = string.Empty;
				return false;
			}
		}

		public override bool TryPeekRedo(out string text)
		{
			if (CanRedo)
			{
				text = RedoStack.Peek().Text;
				return true;
			}
			else
			{
				text = string.Empty;
				return false;
			}
		}

		private void TargetEditor_TextChanged(object sender, RoutedEventArgs e)
		{
			var newText = TargetEditor.Text;
			if (TryPeekUndo(out string previousText) ? newText != previousText : true)
			{
				UndoStack.Push(new(newText, TargetEditor.TextDocument.Selection.EndPosition));
				OnPropertyChanged(nameof(CanUndo));
			}
		}
	}
}