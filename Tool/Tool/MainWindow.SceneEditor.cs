using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using Tool.SceneEditor;

namespace Tool
{
    public partial class MainWindow : Window
    {
        private GamePreviewHwndHost mHwndHost = null;
        private DispatcherTimer mDispatcherTimer = null;
        private bool mbGameLoaded = false;

        private void initialize_SceneEditor()
        {
            mDispatcherTimer = new DispatcherTimer();
            mDispatcherTimer.Tick += new EventHandler(runGame);
            mDispatcherTimer.Interval = TimeSpan.FromMilliseconds(1);

            double iconSize = 0.75;

            Image image_addFile = new Image();
            image_addFile.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\AddFile.png"));
            image_addFile.Width = Border_AddPatternFile.Width * iconSize;
            image_addFile.Height = Border_AddPatternFile.Height * iconSize;

            Border_AddSceneFile.Child = image_addFile;

            Image image_saveFile = new Image();
            image_saveFile.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\SaveFile.png"));
            image_saveFile.Width = Border_SavePatternFile.Width * iconSize;
            image_saveFile.Height = Border_SavePatternFile.Height * iconSize;

            Border_SaveSceneFile.Child = image_saveFile;

            Image image_openFile = new Image();
            image_openFile.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\OpenFile.png"));
            image_openFile.Width = Border_OpenSceneFile.Width * iconSize;
            image_openFile.Height = Border_OpenSceneFile.Height * iconSize;

            Border_OpenSceneFile.Child = image_openFile;

            Image image_play = new Image();
            image_play.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\Play.png"));
            image_play.Width = Border_PlayPreview.Height * iconSize * 0.8;
            image_play.Height = Border_PlayPreview.Height * iconSize * 0.8;

            Border_PlayPreview.Child = image_play;
        }

        private void runGame(object sender, EventArgs e)
        {
            Debug.Assert(mHwndHost != null);

            mHwndHost.RunGame();
        }

        private void refresh_WrapPanel_SceneEditor_PrefabList()
        {
            WrapPanel_SceneEditor_PrefabList.Children.Clear();

            string directory = "Datas\\Prefabs\\";
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
                return;
            }

            string[] filePaths = Directory.GetFiles(directory);
            foreach (string path in filePaths)
            {
                if (!Path.GetExtension(path).Equals(".pfb"))
                {
                    continue;
                }

                string fileName = Path.GetFileNameWithoutExtension(path);

                using (StreamReader reader = new StreamReader(new FileStream(path, FileMode.Open, FileAccess.Read)))
                {
                    string line = reader.ReadLine();
                    if (line == null)
                    {
                        goto WrongFormat;
                    }

                    if (!File.Exists(line))
                    {
                        goto InvalidImage;
                    }

                    string imagePath = line;

                    double scaleX, scaleY;
                    Vector scale = new Vector();
                    line = reader.ReadLine();
                    if (line == null)
                    {
                        goto WrongFormat;
                    }

                    string[] vector = line.Split(',');
                    if (vector.Length != 2)
                    {
                        goto WrongFormat;
                    }

                    if (!double.TryParse(vector[0], out scaleX) || !double.TryParse(vector[1], out scaleY))
                    {
                        goto WrongFormat;
                    }

                    scale.X = scaleX;
                    scale.Y = scaleY;

                    double rotation;
                    line = reader.ReadLine();
                    if (line == null)
                    {
                        goto WrongFormat;
                    }

                    if (!double.TryParse(line, out rotation))
                    {
                        goto WrongFormat;
                    }

                    int patternCount = 0;
                    line = reader.ReadLine();
                    if (line == null)
                    {
                        goto WrongFormat;
                    }

                    if (!int.TryParse(line, out patternCount))
                    {
                        goto WrongFormat;
                    }

                    for (int i = 0; i < patternCount; ++i)
                    {
                        line = reader.ReadLine();
                        if (line == null)
                        {
                            goto WrongFormat;
                        }

                        if (!File.Exists($"Datas\\Patterns\\{line}"))
                        {
                            goto InvalidPattern;
                        }
                    }

                    Grid_PrefabFile prefabFile = new Grid_PrefabFile(path, imagePath, scale, rotation);
                    prefabFile.MouseLeftButtonDown += onMouseLeftButtonDown_SceneEditor_Grid_PrefabFile;
                    WrapPanel_SceneEditor_PrefabList.Children.Add(prefabFile);
                }

                continue;

            WrongFormat:
                MessageBox.Show($"{path}\nPattern File Format Wrong", "Error Message", MessageBoxButton.OK, MessageBoxImage.Error);
                continue;

            InvalidImage:
                MessageBox.Show($"Invalid Image File", "Error Message", MessageBoxButton.OK, MessageBoxImage.Error);
                continue;

            InvalidPattern:
                MessageBox.Show($"Invalid Pattern File", "Error Message", MessageBoxButton.OK, MessageBoxImage.Error);
                continue;
            }
        }

        private void onLoaded_WrapPanel_SceneEditor_PrefabList(object sender, RoutedEventArgs e)
        {
            refresh_WrapPanel_SceneEditor_PrefabList();
        }

        private void onMouseEnter_Border_SceneEditorButtons(object sender, MouseEventArgs e)
        {
            Border button = (Border)sender;
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                button.Background = Brushes.LightSkyBlue;
            }
            else
            {
                button.Background = Brushes.Azure;
            }
        }

        private void onMouseLeave_Border_SceneEditorButtons(object sender, MouseEventArgs e)
        {
            Border button = (Border)sender;
            button.Background = Brushes.White;
        }

        private void onMouseLeftButtonDown_Border_SceneEditorButtons(object sender, MouseButtonEventArgs e)
        {
            Border button = (Border)sender;
            button.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeftButtonUp_Border_AddSceneFile(object sender, MouseButtonEventArgs e)
        {
            Border_AddSceneFile.Background = Brushes.White;

            if (mbGameLoaded)
            {
                switch (MessageBox.Show("작업중인 내용을 저장하고 불러오시겠습니까?", "알림", MessageBoxButton.YesNoCancel, MessageBoxImage.Warning))
                {
                    case MessageBoxResult.None:
                        return;
                    case MessageBoxResult.OK:
                        Debug.Assert(false);
                        break;
                    case MessageBoxResult.Cancel:
                        return;
                    case MessageBoxResult.Yes:
                        //save_PrefabFile();
                        break;
                    case MessageBoxResult.No:
                        break;
                }

                mDispatcherTimer.Stop();
                mHwndHost.Dispose();
                Border_GamePreview.Child = null;
            }

            mbGameLoaded = true;

            mHwndHost = new GamePreviewHwndHost((int)Border_GamePreview.ActualWidth, (int)Border_GamePreview.ActualHeight);
            Border_GamePreview.Child = mHwndHost;

            mDispatcherTimer.Start();
        }

        private void onMouseLeftButtonUp_Border_SaveSceneFile(object sender, MouseButtonEventArgs e)
        {
            if (mbGameLoaded)
            {
                SaveData();
            }
        }

        private void onMouseLeftButtonDown_SceneEditor_Grid_PrefabFile(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount != 2)
            {
                return;
            }

            if (mbGameLoaded)
            {
                RegisterPrefab((sender as Grid_PrefabFile).FilePath, (sender as Grid_PrefabFile).FileName);
            }
        }

        private void onMouseLeftButtonUp_Border_PlayPreview(object sender, MouseButtonEventArgs e)
        {
            var preview = new Tool.GamePreviewWindow.Window_GamePreview();
            preview.Show();
        }

        [DllImport("GameFramework.dll", EntryPoint = "RegisterPrefab", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void RegisterPrefab([MarshalAs(UnmanagedType.LPWStr)] string path, [MarshalAs(UnmanagedType.LPWStr)] string name);

        [DllImport("GameFramework.dll", EntryPoint = "SaveData", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void SaveData();
    }
}
