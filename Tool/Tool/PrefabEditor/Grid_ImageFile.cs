using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Tool.PrefabEditor
{
    public class Grid_ImageFile : Grid
    {
        public string FileName { get; private set; }
        public string FilePath { get; private set; }

        public Image Image_Source { get; private set; } = new Image();

        public Grid_ImageFile(string filePath)
        {
            FileName = Path.GetFileNameWithoutExtension(filePath);
            FilePath = filePath;

            RowDefinitions.Add(new RowDefinition { Height = new GridLength(5.0, GridUnitType.Pixel) });
            RowDefinitions.Add(new RowDefinition { Height = new GridLength(100.0, GridUnitType.Pixel) });
            RowDefinitions.Add(new RowDefinition { Height = new GridLength(45.0, GridUnitType.Pixel) });

            Width = 150.0;
            Margin = new Thickness(20.0, 0.0, 0.0, 5.0);

            Initialize_FileIcon();
            Initialize_FileName();
        }

        private void Initialize_FileIcon()
        {
            Border border_fileIcon = new Border();
            border_fileIcon.Width = 100.0;
            border_fileIcon.Height = 100.0;

            border_fileIcon.Background = Brushes.White;
            border_fileIcon.BorderBrush = Brushes.Black;
            border_fileIcon.BorderThickness = new Thickness(4.0);
            border_fileIcon.CornerRadius = new CornerRadius(5.0);

            border_fileIcon.HorizontalAlignment = HorizontalAlignment.Center;
            border_fileIcon.VerticalAlignment = VerticalAlignment.Center;

            Children.Add(border_fileIcon);
            Grid.SetRow(border_fileIcon, 1);

            Grid grid = new Grid();
            grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(20.0, GridUnitType.Pixel) });
            grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(1.0, GridUnitType.Star) });
            grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1.0, GridUnitType.Star) });
            grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(20.0, GridUnitType.Pixel) });

            border_fileIcon.Child = grid;

            double iconSize = 1.0;

            Image_Source.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\{FilePath}"));
            Image_Source.Width = border_fileIcon.Width * iconSize;
            Image_Source.Height = border_fileIcon.Height * iconSize;

            grid.Children.Add(Image_Source);
            Grid.SetRow(Image_Source, 0);
            Grid.SetRowSpan(Image_Source, 2);
            Grid.SetColumn(Image_Source, 0);
            Grid.SetColumnSpan(Image_Source, 2);
        }

        private void Initialize_FileName()
        {
            TextBlock textBlock_fileName = new TextBlock();
            textBlock_fileName.Height = 25.0;

            textBlock_fileName.Background = Brushes.Transparent;

            if (FileName.Length < 8)
            {
                textBlock_fileName.Text = FileName;
            }
            else
            {
                textBlock_fileName.Text = $"{FileName.Substring(0, 8)}...";
            }

            textBlock_fileName.FontSize = 18.0;
            textBlock_fileName.FontWeight = FontWeights.Bold;

            textBlock_fileName.HorizontalAlignment = HorizontalAlignment.Center;
            textBlock_fileName.VerticalAlignment = VerticalAlignment.Center;
            textBlock_fileName.TextAlignment = TextAlignment.Center;

            Children.Add(textBlock_fileName);
            Grid.SetRow(textBlock_fileName, 2);
        }
    }
}
