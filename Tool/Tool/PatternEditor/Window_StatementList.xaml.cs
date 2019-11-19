using System.Windows;

namespace Tool.PatternEditor
{
    public partial class Window_StatementList : Window
    {
        public Window_StatementList()
        {
            InitializeComponent();
        }

        private void onClosing_Window_StatementList(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = true;

            Hide();
        }

        private void onClosed_Window_StatementList(object sender, System.EventArgs e)
        {
        }
    }
}
