using System.Windows;
using System.Windows.Input;

namespace Tool
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            initialize_PatternEditor();
            initialize_PrefabEditor();
            initialize_SceneEditor();

            Left = SystemParameters.PrimaryScreenWidth / 2 - Width / 2;
            Top = SystemParameters.PrimaryScreenHeight / 2 - Height / 2;
        }

        private void onMouseDown_Window_Main(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            Keyboard.ClearFocus();
        }

        private void onLoaded_Window_Main(object sender, RoutedEventArgs e)
        {
            mWindow_Popup.Owner = this;
        }

        private void onClosed_Window_Main(object sender, System.EventArgs e)
        {
            if (mDispatcherTimer != null)
            {
                mDispatcherTimer.Stop();
            }
        }
    }
}
