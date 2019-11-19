using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Tool.PatternEditor
{
    public class Grid_StatementBlock : Grid
    {
        public EBlockType Type { get; private set; }
        public int NodeNumber { get; private set; }

        public Path Path_Adder { get; private set; } = new Path();
        public Grid Grid_XButton { get; private set; } = new Grid();

        public int ID { get; private set; }
        public string Param { get; private set; } = "None";

        private Rectangle mRectangle_XButton = new Rectangle();

        public Grid_StatementBlock(EBlockType type, int id, string arg, int nodeNumber)
        {
            Debug.Assert(id >= 0);

            Type = type;
            NodeNumber = nodeNumber;

            ID = id;

            ColumnDefinitions.Add(new ColumnDefinition());
            ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(15.0, GridUnitType.Pixel) });
            ColumnDefinitions.Add(new ColumnDefinition());

            Margin = new Thickness(0.0, 0.0, 25.0, 0.0);

            HorizontalAlignment = HorizontalAlignment.Center;
            VerticalAlignment = VerticalAlignment.Center;

            Initialize_Adder();
            Initialize_Block(arg);
        }

        private void Initialize_Adder()
        {
            double width = 40.0;
            double height = 50.0;

            PathFigure pathFigure_triangle = new PathFigure();
            pathFigure_triangle.StartPoint = new Point(0.0, 0.0);
            pathFigure_triangle.Segments.Add(new LineSegment() { Point = new Point(0.0, height) });
            pathFigure_triangle.Segments.Add(new LineSegment() { Point = new Point(width, height * 0.5) });
            pathFigure_triangle.IsClosed = true;

            PathFigureCollection pathFigures_adder = new PathFigureCollection();
            pathFigures_adder.Add(pathFigure_triangle);

            Path_Adder.Data = new PathGeometry() { Figures = pathFigures_adder };
            Path_Adder.Margin = new Thickness(0.0);

            Path_Adder.Fill = Brushes.Red;
            Path_Adder.Stroke = Brushes.Black;
            Path_Adder.StrokeThickness = 4.0;
            Path_Adder.StrokeLineJoin = PenLineJoin.Round;

            Path_Adder.HorizontalAlignment = HorizontalAlignment.Center;
            Path_Adder.VerticalAlignment = VerticalAlignment.Center;

            Path_Adder.Cursor = Cursors.Hand;
            Path_Adder.MouseEnter += onMouseEnter_Path_Adder;
            Path_Adder.MouseLeave += onMouseLeave_Path_Adder;
            Path_Adder.MouseLeftButtonDown += onMouseLeftButtonDown_Path_Adder;
            Path_Adder.MouseLeftButtonUp += onMouseLeftButtonUp_Path_Adder;

            Children.Add(Path_Adder);
            Grid.SetColumn(Path_Adder, 0);
        }

        private void Initialize_Block(string arg)
        {
            Border border_block = new Border();
            border_block.Width = 320.0;
            border_block.Height = 140.0;

            switch (Type)
            {
                case EBlockType.Condition:
                    border_block.Background = Brushes.LightCyan;
                    break;
                case EBlockType.Behaviour:
                    border_block.Background = Brushes.LightPink;
                    break;
                default:
                    Debug.Assert(false);
                    break;
            }

            border_block.BorderBrush = Brushes.Black;
            border_block.BorderThickness = new Thickness(4.0);
            border_block.CornerRadius = new CornerRadius(5.0);

            border_block.HorizontalAlignment = HorizontalAlignment.Center;
            border_block.VerticalAlignment = VerticalAlignment.Center;

            Children.Add(border_block);
            Grid.SetColumn(border_block, 2);

            // Grid
            Grid grid = new Grid();
            grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(30.0, GridUnitType.Pixel) });
            grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(2.0, GridUnitType.Star) });
            grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(2.0, GridUnitType.Star) });
            grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1.0, GridUnitType.Star) });
            grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(30.0, GridUnitType.Pixel) });

            border_block.Child = grid;

            // X Button
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
            mRectangle_XButton.Fill = Background;

            Grid_XButton.Children.Add(mRectangle_XButton);

            double xMarkSize = 12.0;

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
            path_xButton.Margin = new Thickness(5.0, 5.0, 3.0, 3.0);

            path_xButton.Fill = Brushes.Gray;
            path_xButton.Stroke = Brushes.Gray;
            path_xButton.StrokeThickness = 3.0;
            path_xButton.StrokeStartLineCap = PenLineCap.Round;
            path_xButton.StrokeEndLineCap = PenLineCap.Round;

            path_xButton.HorizontalAlignment = HorizontalAlignment.Center;
            path_xButton.VerticalAlignment = VerticalAlignment.Center;

            Grid_XButton.Children.Add(path_xButton);

            // Statement
            TextBlock textBlock_statement = new TextBlock();
            textBlock_statement.Margin = new Thickness(40.0, 10.0, 40.0, 0.0);
            textBlock_statement.Background = Brushes.Transparent;

            switch (Type)
            {
                case EBlockType.Condition:
                    textBlock_statement.Text = StatementManager.Conditions[ID];
                    break;
                case EBlockType.Behaviour:
                    textBlock_statement.Text = StatementManager.Behaviours[ID];
                    break;
                default:
                    Debug.Assert(false);
                    break;
            }

            textBlock_statement.FontSize = 14.0;
            textBlock_statement.FontWeight = FontWeights.SemiBold;
            textBlock_statement.LineHeight = 25.0;
            textBlock_statement.TextWrapping = TextWrapping.Wrap;

            textBlock_statement.HorizontalAlignment = HorizontalAlignment.Center;
            textBlock_statement.VerticalAlignment = VerticalAlignment.Center;
            textBlock_statement.TextAlignment = TextAlignment.Center;

            grid.Children.Add(textBlock_statement);
            Grid.SetRow(textBlock_statement, 0);
            Grid.SetRowSpan(textBlock_statement, 2);
            Grid.SetColumn(textBlock_statement, 0);
            Grid.SetColumnSpan(textBlock_statement, 2);

            // Params
            switch (Type)
            {
                case EBlockType.Condition:
                    switch ((ECondition)ID)
                    {
                        case ECondition.KeyDown:
                        case ECondition.KeyPressed:
                        case ECondition.KeyUp:
                            Initialize_Param_ComboBox(grid, arg);
                            break;
                        case ECondition.Collision:
                            Initialize_Param_TextBox(grid, arg);
                            break;
                        case ECondition.Dead:
                            return;
                        default:
                            Debug.Assert(false);
                            break;
                    }

                    break;
                case EBlockType.Behaviour:
                    switch ((EBehaviour)ID)
                    {
                        case EBehaviour.MoveHorizontal:
                            Initialize_Param_TextBox(grid, arg);
                            break;
                        case EBehaviour.MoveVertical:
                            Initialize_Param_TextBox(grid, arg);
                            break;
                        case EBehaviour.Jump:
                            Initialize_Param_TextBox(grid, arg);
                            break;
                        case EBehaviour.Dead:
                            break;
                        default:
                            Debug.Assert(false);
                            break;
                    }

                    break;
                default:
                    Debug.Assert(false);
                    break;
            }
        }

        private void Initialize_Param_ComboBox(Grid grid, string parameter)
        {
            Border border_params = new Border();
            border_params.Width = 210.0;
            border_params.Height = 30.0;

            border_params.Background = Brushes.White;
            border_params.BorderBrush = Brushes.Black;
            border_params.BorderThickness = new Thickness(3.0);
            border_params.CornerRadius = new CornerRadius(3.0);

            border_params.HorizontalAlignment = HorizontalAlignment.Center;
            border_params.VerticalAlignment = VerticalAlignment.Top;

            grid.Children.Add(border_params);
            Grid.SetRow(border_params, 2);
            Grid.SetColumn(border_params, 0);
            Grid.SetColumnSpan(border_params, 2);

            ComboBox comboBox_params = new ComboBox();
            comboBox_params.Width = 210.0;
            comboBox_params.Margin = new Thickness(0.0, 0.0, 0.0, 0.0);
            
            comboBox_params.Background = Brushes.White;
            comboBox_params.BorderThickness = new Thickness(0.0);

            comboBox_params.FontSize = 14.0;
            comboBox_params.FontWeight = FontWeights.Normal;

            comboBox_params.HorizontalAlignment = HorizontalAlignment.Center;
            comboBox_params.VerticalAlignment = VerticalAlignment.Center;

            comboBox_params.ItemsSource = Enum.GetNames(typeof(EKeyCode));

            comboBox_params.SelectionChanged += onSelectionChanged_ComboBox_Params;

            int parameterCode;
            if (int.TryParse(parameter, out parameterCode))
            {
                if (parameterCode >= 0 && parameterCode < comboBox_params.Items.Count)
                {
                    comboBox_params.SelectedIndex = parameterCode;
                }
            }

            border_params.Child = comboBox_params;
        }

        private void Initialize_Param_TextBox(Grid grid, string parameter)
        {
            Border border_params = new Border();
            border_params.Width = 210.0;
            border_params.Height = 30.0;

            border_params.Background = Brushes.White;
            border_params.BorderBrush = Brushes.Black;
            border_params.BorderThickness = new Thickness(3.0);
            border_params.CornerRadius = new CornerRadius(3.0);

            border_params.HorizontalAlignment = HorizontalAlignment.Center;
            border_params.VerticalAlignment = VerticalAlignment.Top;

            grid.Children.Add(border_params);
            Grid.SetRow(border_params, 2);
            Grid.SetColumn(border_params, 0);
            Grid.SetColumnSpan(border_params, 2);

            TextBox textBox_params = new TextBox();
            textBox_params.TextChanged += onTextChanged_TextBox_Params;

            textBox_params.Width = 210.0;
            textBox_params.Margin = new Thickness(5.0, 0.0, 5.0, 0.0);

            textBox_params.Background = Brushes.Transparent;
            textBox_params.BorderThickness = new Thickness(0.0);

            textBox_params.Text = parameter;
            textBox_params.FontSize = 14.0;
            textBox_params.FontWeight = FontWeights.Normal;

            textBox_params.HorizontalAlignment = HorizontalAlignment.Center;
            textBox_params.VerticalAlignment = VerticalAlignment.Center;
            textBox_params.TextAlignment = TextAlignment.Center;

            border_params.Child = textBox_params;
        }

        private void onMouseEnter_Path_Adder(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                Path_Adder.Fill = Brushes.Orange;
            }
            else
            {
                Path_Adder.Fill = Brushes.Tomato;
            }
        }

        private void onMouseLeave_Path_Adder(object sender, MouseEventArgs e)
        {
            Path_Adder.Fill = Brushes.Red;
        }

        private void onMouseLeftButtonDown_Path_Adder(object sender, MouseEventArgs e)
        {
            Path_Adder.Fill = Brushes.Orange;
        }

        private void onMouseLeftButtonUp_Path_Adder(object sender, MouseEventArgs e)
        {
            Path_Adder.Fill = Brushes.Red;
        }

        private void onMouseEnter_Grid_XButton(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                switch (Type)
                {
                    case EBlockType.Condition:
                        mRectangle_XButton.Fill = Brushes.LightPink;
                        break;
                    case EBlockType.Behaviour:
                        mRectangle_XButton.Fill = Brushes.Ivory;
                        break;
                    default:
                        Debug.Assert(false);
                        break;
                }
            }
            else
            {
                mRectangle_XButton.Fill = Brushes.LightGray;
            }
        }

        private void onMouseLeave_Grid_XButton(object sender, MouseEventArgs e)
        {
            mRectangle_XButton.Fill = Background;
        }

        private void onMouseLeftButtonDown_Grid_XButton(object sender, MouseEventArgs e)
        {
            switch (Type)
            {
                case EBlockType.Condition:
                    mRectangle_XButton.Fill = Brushes.LightPink;
                    break;
                case EBlockType.Behaviour:
                    mRectangle_XButton.Fill = Brushes.Ivory;
                    break;
                default:
                    Debug.Assert(false);
                    break;
            }
        }

        private void onMouseLeftButtonUp_Grid_XButton(object sender, MouseEventArgs e)
        {
            mRectangle_XButton.Fill = Brushes.LightGray;
        }

        private void onSelectionChanged_ComboBox_Params(object sender, SelectionChangedEventArgs e)
        {
            ComboBox comboBox = (ComboBox)sender;
            Param = comboBox.SelectedIndex.ToString();
        }

        private void onTextChanged_TextBox_Params(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            Param = textBox.Text;
        }
    }
}
