using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Threading;

namespace Tool.GamePreviewWindow
{
    public partial class Window_GamePreview : Window
    {
        private GamePreviewHwndHost mHwndHost = null;
        private DispatcherTimer mDispatcherTimer = null;

        public Window_GamePreview()
        {
            InitializeComponent();

            mDispatcherTimer = new DispatcherTimer();
            mDispatcherTimer.Tick += new EventHandler(runGame);
            mDispatcherTimer.Interval = TimeSpan.FromMilliseconds(1);
        }

        private void onLoded_Window_GamePreview(object sender, RoutedEventArgs e)
        {
            mHwndHost = new GamePreviewHwndHost((int)Border_GamePreview.ActualWidth, (int)Border_GamePreview.ActualHeight);
            Border_GamePreview.Child = mHwndHost;

            mDispatcherTimer.Start();
        }

        private void runGame(object sender, EventArgs e)
        {
            Debug.Assert(mHwndHost != null);

            mHwndHost.RunGame();
        }

        private void onClosed_Window_GamePreview(object sender, System.EventArgs e)
        {
            if (mDispatcherTimer != null)
            {
                mDispatcherTimer.Stop();
            }

            mHwndHost.Dispose();
        }
    }
}
