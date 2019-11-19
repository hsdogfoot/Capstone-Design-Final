using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Tool.PrefabEditor;

namespace Tool
{
    public partial class MainWindow : Window
    {
        private PrefabData mLoadedPrefabData = null;

        private void initialize_PrefabEditor()
        {
            double iconSize = 0.75;

            Image image_addFile = new Image();
            image_addFile.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\AddFile.png"));
            image_addFile.Width = Border_AddPatternFile.Width * iconSize;
            image_addFile.Height = Border_AddPatternFile.Height * iconSize;

            Border_AddPrefabFile.Child = image_addFile;

            Image image_saveFile = new Image();
            image_saveFile.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\SaveFile.png"));
            image_saveFile.Width = Border_SavePatternFile.Width * iconSize;
            image_saveFile.Height = Border_SavePatternFile.Height * iconSize;

            Border_SavePrefabFile.Child = image_saveFile;
        }

        private void refresh_WrapPanel_PrefabEditor_PrefabList()
        {
            WrapPanel_PrefabEditor_PrefabList.Children.Clear();

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

                    List<string> patterns = new List<string>();
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

                        patterns.Add(line);
                    }

                    PrefabData data = new PrefabData(fileName, path, imagePath, scale, rotation, patterns);

                    Grid_PrefabFile prefabFile = new Grid_PrefabFile(data);
                    prefabFile.MouseLeftButtonDown += onMouseLeftButtonDown_PrefabEditor_Grid_PrefabFile;
                    prefabFile.Grid_XButton.MouseLeftButtonUp += onMouseLeftButtonUp_Grid_XButton_PrefabFile;
                    WrapPanel_PrefabEditor_PrefabList.Children.Add(prefabFile);
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

        private void refresh_WrapPanel_PrefabEditor_ImageList()
        {
            WrapPanel_PrefabEditor_ImageList.Children.Clear();

            string directory = "Resources\\Textures\\";
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
                return;
            }

            string[] filePaths = Directory.GetFiles(directory);
            foreach (string path in filePaths)
            {
                if (!Path.GetExtension(path).Equals(".jpg") && !Path.GetExtension(path).Equals(".png"))
                {
                    continue;
                }

                Grid_ImageFile imageFile = new Grid_ImageFile(path);
                imageFile.MouseLeftButtonDown += onMouseLeftButtonDown_PrefabEditor_Grid_ImageFile;

                WrapPanel_PrefabEditor_ImageList.Children.Add(imageFile);
            }
        }

        private void refresh_WrapPanel_PrefabEditor_PatternList()
        {
            WrapPanel_PrefabEditor_PatternList.Children.Clear();

            string directory = "Datas\\Patterns\\";
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
                return;
            }

            string[] filePaths = Directory.GetFiles(directory);
            foreach (string path in filePaths)
            {
                if (!Path.GetExtension(path).Equals(".pat"))
                {
                    continue;
                }

                Grid_PatternFile patternFile = new Grid_PatternFile(path);
                patternFile.MouseLeftButtonDown += onMouseLeftButtonDown_PrefabEditor_Grid_PatternFile;

                WrapPanel_PrefabEditor_PatternList.Children.Add(patternFile);
            }
        }

        private void refresh_Grid_PrefabEditor()
        {
            Debug.Assert(mLoadedPrefabData != null);

            TextBox_PrefabName.Text = mLoadedPrefabData.Name;

            Grid_PrefabEditor.Visibility = Visibility.Visible;
            Border_PrefabImage.Child = null;

            double imageSize = 1.0;

            Image image_prefab = new Image();

            if (!mLoadedPrefabData.ImagePath.Equals(""))
            {                
                image_prefab.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\{mLoadedPrefabData.ImagePath}"));
            }

            image_prefab.Effect = new ShaderEffect_Rotation();
            image_prefab.Width = Border_PrefabImage.ActualWidth * imageSize;
            image_prefab.Height = Border_PrefabImage.ActualHeight * imageSize;
            Border_PrefabImage.Child = image_prefab;

            if (image_prefab.Source != null)
            {
                Grid_PrefabTransform.Visibility = Visibility.Visible;
            }
            else
            {
                Grid_PrefabTransform.Visibility = Visibility.Hidden;
            }

            TextBox_PrefabScaleX.Text = Math.Abs(mLoadedPrefabData.Scale.X).ToString();
            TextBox_PrefabScaleY.Text = Math.Abs(mLoadedPrefabData.Scale.Y).ToString();

            if (mLoadedPrefabData.Scale.X < 0.0)
            {
                (image_prefab.Effect as ShaderEffect_Rotation).LeftRightSwap = 1.0;
            }

            if (mLoadedPrefabData.Scale.Y < 0.0)
            {
                (image_prefab.Effect as ShaderEffect_Rotation).TopDownSwap = 1.0;
            }

            (image_prefab.Effect as ShaderEffect_Rotation).Rotation = mLoadedPrefabData.Rotation;

            ListBox_EquippedPatternList.Items.Clear();
            foreach (string pattern in mLoadedPrefabData.Patterns)
            {
                ListBox_EquippedPatternList.Items.Add(new ListBoxItem() { Content = pattern });
            }
        }

        private void save_PrefabFile()
        {
            if (mLoadedPrefabData == null)
            {
                MessageBox.Show("작성된 프리팹이 존재하지 않습니다.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            if (TextBox_PrefabName.Text.Equals(""))
            {
                MessageBox.Show("프리팹의 이름을 먼저 설정해주세요.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            using (StreamWriter writer = new StreamWriter(new FileStream($"Datas\\Prefabs\\{TextBox_PrefabName.Text}.pfb", FileMode.OpenOrCreate, FileAccess.Write)))
            {
                string filePath = (Border_PrefabImage.Child as Image).Source.ToString();
                writer.WriteLine(filePath.Substring(filePath.IndexOf("Resources")).Replace('/', '\\'));

                if (Math.Abs((Border_PrefabImage.Child.Effect as ShaderEffect_Rotation).LeftRightSwap - 1.0) < Double.Epsilon)
                {
                    writer.Write($"-{TextBox_PrefabScaleX.Text},");
                }
                else
                {
                    writer.Write($"{TextBox_PrefabScaleX.Text},");
                }

                if (Math.Abs((Border_PrefabImage.Child.Effect as ShaderEffect_Rotation).TopDownSwap - 1.0) < Double.Epsilon)
                {
                    writer.WriteLine($"-{TextBox_PrefabScaleY.Text}");
                }
                else
                {
                    writer.WriteLine($"{TextBox_PrefabScaleY.Text}");
                }

                writer.WriteLine($"{(Border_PrefabImage.Child.Effect as ShaderEffect_Rotation).Rotation}");

                writer.WriteLine($"{ListBox_EquippedPatternList.Items.Count}");

                foreach (ListBoxItem item in ListBox_EquippedPatternList.Items)
                {
                    writer.WriteLine($"{item.Content.ToString()}");
                }
            }

            refresh_WrapPanel_PrefabEditor_PrefabList();
            MessageBox.Show("성공적으로 프리팹 파일을 저장했습니다.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void onLoaded_WrapPanel_PrefabEditor_PrefabList(object sender, RoutedEventArgs e)
        {
            refresh_WrapPanel_PrefabEditor_PrefabList();
        }

        private void onLoaded_WrapPanel_PrefabEditor_ImageList(object sender, RoutedEventArgs e)
        {
            refresh_WrapPanel_PrefabEditor_ImageList();
        }

        private void onLoaded_WrapPanel_PrefabEditor_PatternList(object sender, RoutedEventArgs e)
        {
            refresh_WrapPanel_PrefabEditor_PatternList();
        }

        private void onMouseEnter_Border_AddPrefabFile(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                Border_AddPrefabFile.Background = Brushes.LightSkyBlue;
            }
            else
            {
                Border_AddPrefabFile.Background = Brushes.Azure;
            }
        }

        private void onMouseLeave_Border_AddPrefabFile(object sender, MouseEventArgs e)
        {
            Border_AddPrefabFile.Background = Brushes.White;
        }

        private void onMouseLeftButtonDown_Border_AddPrefabFile(object sender, MouseButtonEventArgs e)
        {
            Border_AddPrefabFile.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeftButtonUp_Border_AddPrefabFile(object sender, MouseEventArgs e)
        {
            Border_AddPrefabFile.Background = Brushes.White;

            if (mLoadedPrefabData != null)
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
                        save_PrefabFile();
                        break;
                    case MessageBoxResult.No:
                        break;
                }
            }

            mLoadedPrefabData = new PrefabData("", "", "", new Vector(1.0, 1.0), 0.0, new List<string>());
            refresh_Grid_PrefabEditor();

            TabControl_PrefabEditor_ResourceList.SelectedIndex = 1;
        }

        private void onMouseEnter_Border_SavePrefabFile(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                Border_SavePrefabFile.Background = Brushes.LightSkyBlue;
            }
            else
            {
                Border_SavePrefabFile.Background = Brushes.Azure;
            }
        }

        private void onMouseLeave_Border_SavePrefabFile(object sender, MouseEventArgs e)
        {
            Border_SavePrefabFile.Background = Brushes.White;
        }

        private void onMouseLeftButtonDown_Border_SavePrefabFile(object sender, MouseButtonEventArgs e)
        {
            Border_SavePrefabFile.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeftButtonUp_Border_SavePrefabFile(object sender, MouseEventArgs e)
        {
            Border_SavePrefabFile.Background = Brushes.White;

            save_PrefabFile();
        }

        private void onMouseLeftButtonDown_PrefabEditor_Grid_PrefabFile(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount != 2)
            {
                return;
            }

            if (mLoadedPrefabData != null)
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
                        save_PrefabFile();
                        break;
                    case MessageBoxResult.No:
                        break;
                }
            }

            Grid_PrefabFile file = (Grid_PrefabFile)sender;

            mLoadedPrefabData = file.Data;
            refresh_Grid_PrefabEditor();
        }

        private void onMouseLeftButtonDown_PrefabEditor_Grid_ImageFile(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount != 2)
            {
                return;
            }

            if (mLoadedPrefabData == null)
            {
                return;
            }

            Grid_ImageFile file = (Grid_ImageFile)sender;
            Image image = (Image)file.Image_Source;
            Image image_prefab = (Image)Border_PrefabImage.Child;

            if (image_prefab.Source == null)
            {
                Grid_PrefabTransform.Visibility = Visibility.Visible;
            }

            image_prefab.Source = image.Source;
        }

        private void onMouseLeftButtonDown_PrefabEditor_Grid_PatternFile(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount != 2)
            {
                return;
            }

            if (mLoadedPrefabData == null)
            {
                return;
            }

            Grid_PatternFile patternFile = (Grid_PatternFile)sender;
            ListBox_EquippedPatternList.Items.Add(new ListBoxItem() { Content = Path.GetFileName(patternFile.FilePath) });
        }

        private void onMouseLeftButtonUp_Grid_XButton_PrefabFile(object sender, MouseButtonEventArgs e)
        {
            File.Delete(((((sender as Grid).Parent as Grid).Parent as Border).Parent as Grid_PrefabFile).Data.FilePath);
            MessageBox.Show("성공적으로 프리팹 파일을 삭제했습니다.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);

            refresh_WrapPanel_PrefabEditor_PrefabList();
        }

        private void onKeyDown_ListBox_EquippedPatternList(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Delete)
            {
                ListBox_EquippedPatternList.Items.Remove(ListBox_EquippedPatternList.SelectedItem);
            }
        }

        private void onMouseEnter_Border_LeftRightSwap(object sender, MouseEventArgs e)
        {
            Border_LeftRightSwap.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeave_Border_LeftRightSwap(object sender, MouseEventArgs e)
        {
            Border_LeftRightSwap.Background = Brushes.Gainsboro;
        }

        private void onMouseLeftButtonDown_Border_LeftRightSwap(object sender, MouseButtonEventArgs e)
        {
            if (Border_PrefabImage.Child == null)
            {
                return;
            }

            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).LeftRightSwap = 1.0 - ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).LeftRightSwap;
        }

        private void onMouseEnter_Border_TopDownSwap(object sender, MouseEventArgs e)
        {
            Border_TopDownSwap.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeave_Border_TopDownSwap(object sender, MouseEventArgs e)
        {
            Border_TopDownSwap.Background = Brushes.Gainsboro;
        }

        private void onMouseLeftButtonDown_Border_TopDownSwap(object sender, MouseButtonEventArgs e)
        {
            if (Border_PrefabImage.Child == null)
            {
                return;
            }

            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).TopDownSwap = 1.0 - ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).TopDownSwap;
        }

        private void onMouseEnter_Border_Left90(object sender, MouseEventArgs e)
        {
            Border_Left90.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeave_Border_Left90(object sender, MouseEventArgs e)
        {
            Border_Left90.Background = Brushes.Gainsboro;
        }

        private void onMouseLeftButtonDown_Border_Left90(object sender, MouseButtonEventArgs e)
        {
            if (Border_PrefabImage.Child == null)
            {
                return;
            }

            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).Rotation += 90.0;
            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).Rotation = (int)((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).Rotation % 360;
        }

        private void onMouseEnter_Border_Right90(object sender, MouseEventArgs e)
        {
            Border_Right90.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeave_Border_Right90(object sender, MouseEventArgs e)
        {
            Border_Right90.Background = Brushes.Gainsboro;
        }

        private void onMouseLeftButtonDown_Border_Right90(object sender, MouseButtonEventArgs e)
        {
            if (Border_PrefabImage.Child == null)
            {
                return;
            }

            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).Rotation += 270.0;
            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).Rotation = (int)((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).Rotation % 360;
        }

        private void onMouseEnter_Border_InitTransform(object sender, MouseEventArgs e)
        {
            Border_InitTransform.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeave_Border_InitTransform(object sender, MouseEventArgs e)
        {
            Border_InitTransform.Background = Brushes.Gainsboro;
        }

        private void onMouseLeftButtonDown_Border_InitTransform(object sender, MouseButtonEventArgs e)
        {
            if (Border_PrefabImage.Child == null)
            {
                return;
            }

            TextBox_PrefabScaleX.Text = "1.0";
            TextBox_PrefabScaleY.Text = "1.0";

            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).LeftRightSwap = 0.0;
            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).TopDownSwap = 0.0;

            ((Border_PrefabImage.Child as Image).Effect as ShaderEffect_Rotation).Rotation = 0.0;
        }        
    }
}
