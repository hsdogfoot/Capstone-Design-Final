using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Tool.PatternEditor
{
    public class Grid_PatternFile : Grid
    {
        public PatternData Data { get; private set; }

        public Grid Grid_XButton = new Grid();

        private Rectangle mRectangle_XButton = new Rectangle();

        public Grid_PatternFile(PatternData data)
        {
            Data = data;

            RowDefinitions.Add(new RowDefinition { Height = new GridLength(5.0, GridUnitType.Pixel) });
            RowDefinitions.Add(new RowDefinition { Height = new GridLength(100.0, GridUnitType.Pixel) });
            RowDefinitions.Add(new RowDefinition { Height = new GridLength(45.0, GridUnitType.Pixel) });

            Width = 150.0;
            Margin = new Thickness(20.0, 0.0, 0.0, 15.0);

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

            double iconSize = 0.55;

            Image image_fileIcon = new Image();
            image_fileIcon.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\Pattern.jpg"));
            image_fileIcon.Width = border_fileIcon.Width * iconSize;
            image_fileIcon.Height = border_fileIcon.Height * iconSize;

            grid.Children.Add(image_fileIcon);
            Grid.SetRow(image_fileIcon, 0);
            Grid.SetRowSpan(image_fileIcon, 2);
            Grid.SetColumn(image_fileIcon, 0);
            Grid.SetColumnSpan(image_fileIcon, 2);

            Grid_XButton.Margin = new Thickness(1.0, 3.0, 3.0, 1.0);

            Grid_XButton.Cursor = Cursors.Hand;
            Grid_XButton.MouseEnter += onMouseEnter_Grid_XButton;
            Grid_XButton.MouseLeave += onMouseLeave_Grid_XButton;
            Grid_XButton.MouseLeftButtonDown += onMouseLeftButtonDown_Grid_XButton;
            Grid_XButton.MouseLeftButtonUp += onMouseLeftButtonUp_Grid_XButton;

            grid.Children.Add(Grid_XButton);
            Grid.SetRow(Grid_XButton, 0);
            Grid.SetColumn(Grid_XButton, 1);

            mRectangle_XButton.RadiusX = 3.0;
            mRectangle_XButton.RadiusY = 3.0;
            mRectangle_XButton.Fill = Brushes.LightGray;

            Grid_XButton.Children.Add(mRectangle_XButton);

            double xMarkSize = 6.0;

            PathFigure pathFigure_leftTop = new PathFigure();
            pathFigure_leftTop.StartPoint = new Point(0.0, 0.0);
            pathFigure_leftTop.Segments.Add(new LineSegment() { Point = new Point(xMarkSize, xMarkSize) });

            PathFigure pathFigure_rightTop = new PathFigure();
            pathFigure_rightTop.StartPoint = new Point(0.0, xMarkSize);
            pathFigure_rightTop.Segments.Add(new LineSegment() { Point = new Point(xMarkSize, 0.0) });

            PathFigureCollection pathFigures_xButton = new PathFigureCollection();
            pathFigures_xButton.Add(pathFigure_leftTop);
            pathFigures_xButton.Add(pathFigure_rightTop);

            Path path_xButton = new Path();
            path_xButton.Data = new PathGeometry() { Figures = pathFigures_xButton };
            path_xButton.Margin = new Thickness(4.0, 5.0, 3.0, 3.0);

            path_xButton.Fill = Brushes.Gray;
            path_xButton.Stroke = Brushes.Gray;
            path_xButton.StrokeThickness = 2.0;
            path_xButton.StrokeStartLineCap = PenLineCap.Round;
            path_xButton.StrokeEndLineCap = PenLineCap.Round;

            path_xButton.HorizontalAlignment = HorizontalAlignment.Center;
            path_xButton.VerticalAlignment = VerticalAlignment.Center;

            Grid_XButton.Children.Add(path_xButton);
        }

        private void Initialize_FileName()
        {
            TextBlock textBlock_fileName = new TextBlock();
            textBlock_fileName.Height = 25.0;

            textBlock_fileName.Background = Brushes.Transparent;

            if (Data.Name.Length < 8)
            {
                textBlock_fileName.Text = Data.Name;
            }
            else
            {
                textBlock_fileName.Text = $"{Data.Name.Substring(0, 8)}...";
            }

            textBlock_fileName.FontSize = 18.0;
            textBlock_fileName.FontWeight = FontWeights.Bold;

            textBlock_fileName.HorizontalAlignment = HorizontalAlignment.Center;
            textBlock_fileName.VerticalAlignment = VerticalAlignment.Center;
            textBlock_fileName.TextAlignment = TextAlignment.Center;

            Children.Add(textBlock_fileName);
            Grid.SetRow(textBlock_fileName, 2);
        }

        private void onMouseEnter_Grid_XButton(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                mRectangle_XButton.Fill = Brushes.HotPink;
            }
            else
            {
                mRectangle_XButton.Fill = Brushes.LightPink;
            }
        }

        private void onMouseLeave_Grid_XButton(object sender, MouseEventArgs e)
        {
            mRectangle_XButton.Fill = Brushes.LightGray;
        }

        private void onMouseLeftButtonDown_Grid_XButton(object sender, MouseEventArgs e)
        {
            mRectangle_XButton.Fill = Brushes.HotPink;
        }

        private void onMouseLeftButtonUp_Grid_XButton(object sender, MouseEventArgs e)
        {
            mRectangle_XButton.Fill = Brushes.LightGray;
        }
    }
}
