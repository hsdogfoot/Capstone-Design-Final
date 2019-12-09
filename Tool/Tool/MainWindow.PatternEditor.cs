using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Tool.PatternEditor;

namespace Tool
{
    public partial class MainWindow : Window
    {
        private PatternData mLoadedPatternData = null;
        private Window_StatementList mWindow_Popup = new Window_StatementList();

        private EAdderMode mAdderMode = EAdderMode.CreateNew;
        private int mTargetNodeIndex = 0;

        // 행동 규칙 편집기를 초기화합니다.
        private void initialize_PatternEditor()
        {
            StatementManager.Initialize();
            mWindow_Popup.ListBox_StatementList.MouseDoubleClick += onMouseDoubleClick_ListBox_StatementList;

            double iconSize = 0.75;

            // "새로 만들기" 버튼 생성
            Image image_addFile = new Image();
            image_addFile.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\AddFile.png"));
            image_addFile.Width = Border_AddPatternFile.Width * iconSize;
            image_addFile.Height = Border_AddPatternFile.Height * iconSize;

            Border_AddPatternFile.Child = image_addFile;

            // "저장하기" 버튼 생성
            Image image_saveFile = new Image();
            image_saveFile.Source = new BitmapImage(new Uri($"{Environment.CurrentDirectory}\\Resources\\Icons\\SaveFile.png"));
            image_saveFile.Width = Border_SavePatternFile.Width * iconSize;
            image_saveFile.Height = Border_SavePatternFile.Height * iconSize;

            Border_SavePatternFile.Child = image_saveFile;
        }

        // 행동 규칙 목록을 갱신합니다.
        private void refresh_WrapPanel_PatternEditor_PatternList()
        {
            WrapPanel_PatternEditor_PatternList.Children.Clear();

            string directory = "Datas\\Patterns\\";
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
                return;
            }

            // Patterns 디렉토리에서 .pat 파일들을 읽어들입니다.
            string[] filePaths = Directory.GetFiles(directory);
            foreach (string path in filePaths)
            {
                if (!Path.GetExtension(path).Equals(".pat"))
                {
                    continue;
                }

                string fileName = Path.GetFileNameWithoutExtension(path);

                using (StreamReader reader = new StreamReader(new FileStream(path, FileMode.Open, FileAccess.Read)))
                {
                    int conditionCount = 0;
                    string line = reader.ReadLine();
                    if (line == null)
                    {
                        goto WrongFormat;
                    }

                    if (!int.TryParse(line, out conditionCount))
                    {
                        goto WrongFormat;
                    }

                    int id;
                    LinkedList<KeyValuePair<int, string>> conditions = new LinkedList<KeyValuePair<int, string>>();
                    KeyValuePair<int, string>? behaviour = null;
                    for (int i = 0; i <= conditionCount; ++i)
                    {
                        line = reader.ReadLine();
                        if (line == null)
                        {
                            goto WrongFormat;
                        }

                        string[] pair = line.Split(',');
                        if (pair.Length != 2)
                        {
                            goto WrongFormat;
                        }

                        if (!int.TryParse(pair[0], out id))
                        {
                            goto WrongFormat;
                        }

                        if (i != conditionCount)
                        {
                            conditions.AddLast(new KeyValuePair<int, string>(id, pair[1]));
                        }
                        else
                        {
                            behaviour = new KeyValuePair<int, string>(id, pair[1]);
                        }
                    }

                    // 수집한 정보들을 토대로 PatternData 클래스를 만듭니다.
                    PatternData data = new PatternData(fileName, path, conditions, behaviour.GetValueOrDefault());

                    // 이를 이용해 Grid 객체를 생성하고 목록에 추가합니다.
                    Grid_PatternFile patternFile = new Grid_PatternFile(data);
                    patternFile.MouseLeftButtonDown += onMouseLeftButtonDown_PatternEditor_Grid_PatternFile;
                    patternFile.Grid_XButton.MouseLeftButtonUp += onMouseLeftButtonUp_Grid_XButton_PatternFile;
                    WrapPanel_PatternEditor_PatternList.Children.Add(patternFile);
                }

                continue;

            WrongFormat:
                MessageBox.Show($"{path}\nPattern File Format Wrong", "Error Message", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        // 작업중인 행동 규칙 화면을 갱신합니다. (블록 코딩 부분)
        private void refresh_StackPanel_PatternEditor()
        {
            Debug.Assert(mLoadedPatternData != null);

            StackPanel_PatternEditor.Children.Clear();
            TextBox_PatternName.Text = mLoadedPatternData.Name;

            // 로드할 패턴의 조건 블록들을 차례로 생성해 목록에 추가합니다. (Linked List 로 되어있음)
            int nodeCount = 0;
            Grid_StatementBlock block;
            LinkedListNode<KeyValuePair<int, string>> node = mLoadedPatternData.Conditions.First;
            while (node != null)
            {
                block = new Grid_StatementBlock(EBlockType.Condition, node.Value.Key, node.Value.Value, nodeCount);
                block.Path_Adder.MouseLeftButtonUp += onMouseLeftButtonUp_Path_Adder;
                block.Grid_XButton.MouseLeftButtonUp += onMouseLeftButtonUp_Grid_XButton_Condition;

                StackPanel_PatternEditor.Children.Add(block);
                ++nodeCount;
                node = node.Next;
            }

            // 로드할 패턴의 동작 블록을 생성해 목록에 추가합니다.
            block = new Grid_StatementBlock(EBlockType.Behaviour, mLoadedPatternData.Behaviour.Key, mLoadedPatternData.Behaviour.Value, nodeCount);
            block.Path_Adder.MouseLeftButtonUp += onMouseLeftButtonUp_Path_Adder;
            block.Grid_XButton.MouseLeftButtonUp += onMouseLeftButtonUp_Grid_XButton_Behaviour;

            StackPanel_PatternEditor.Children.Add(block);
        }

        // 작성한 내용을 .pat 형식으로 저장합니다.
        private void save_PatternFile()
        {
            if (mLoadedPatternData == null)
            {
                MessageBox.Show("작성된 행동 규칙이 존재하지 않습니다.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            if (TextBox_PatternName.Text.Equals(""))
            {
                MessageBox.Show("행동 규칙의 이름을 먼저 설정해주세요.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            using (StreamWriter writer = new StreamWriter(new FileStream($"Datas\\Patterns\\{TextBox_PatternName.Text}.pat", FileMode.OpenOrCreate, FileAccess.Write)))
            {
                writer.WriteLine(StackPanel_PatternEditor.Children.Count - 1);

                foreach (Grid_StatementBlock block in StackPanel_PatternEditor.Children)
                {
                    writer.WriteLine($"{block.ID},{block.Param}");
                }
            }

            refresh_WrapPanel_PatternEditor_PatternList();
            MessageBox.Show("성공적으로 행동 규칙 파일을 저장했습니다.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void onLoaded_WrapPanel_PatternEditor_PatternList(object sender, RoutedEventArgs e)
        {
            refresh_WrapPanel_PatternEditor_PatternList();
        }

        private void onMouseEnter_Border_AddPatternFile(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                Border_AddPatternFile.Background = Brushes.LightSkyBlue;
            }
            else
            {
                Border_AddPatternFile.Background = Brushes.Azure;
            }
        }

        private void onMouseLeave_Border_AddPatternFile(object sender, MouseEventArgs e)
        {
            Border_AddPatternFile.Background = Brushes.White;
        }

        private void onMouseLeftButtonDown_Border_AddPatternFile(object sender, MouseButtonEventArgs e)
        {
            Border_AddPatternFile.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeftButtonUp_Border_AddPatternFile(object sender, MouseEventArgs e)
        {
            Border_AddPatternFile.Background = Brushes.White;

            if (mLoadedPatternData != null)
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
                        save_PatternFile();
                        break;
                    case MessageBoxResult.No:
                        break;
                }
            }

            mAdderMode = EAdderMode.CreateNew;
            mTargetNodeIndex = 0;

            mWindow_Popup.Title = "동작 목록";
            mWindow_Popup.ListBox_StatementList.Background = Brushes.LightPink;
            mWindow_Popup.ListBox_StatementList.Items.Clear();

            foreach (string statement in StatementManager.Behaviours)
            {
                mWindow_Popup.ListBox_StatementList.Items.Add(statement);
            }

            mWindow_Popup.Owner = this;
            mWindow_Popup.ShowDialog();
        }

        private void onMouseEnter_Border_SavePatternFile(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                Border_SavePatternFile.Background = Brushes.LightSkyBlue;
            }
            else
            {
                Border_SavePatternFile.Background = Brushes.Azure;
            }
        }

        private void onMouseLeave_Border_SavePatternFile(object sender, MouseEventArgs e)
        {
            Border_SavePatternFile.Background = Brushes.White;
        }

        private void onMouseLeftButtonDown_Border_SavePatternFile(object sender, MouseButtonEventArgs e)
        {
            Border_SavePatternFile.Background = Brushes.LightSkyBlue;
        }

        private void onMouseLeftButtonUp_Border_SavePatternFile(object sender, MouseEventArgs e)
        {
            Border_SavePatternFile.Background = Brushes.White;

            save_PatternFile();
        }

        private void onMouseLeftButtonDown_PatternEditor_Grid_PatternFile(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount != 2)
            {
                return;
            }

            if (mLoadedPatternData != null)
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
                        save_PatternFile();
                        break;
                    case MessageBoxResult.No:
                        break;
                }
            }

            Grid_PatternFile file = (Grid_PatternFile)sender;

            mLoadedPatternData = file.Data;
            refresh_StackPanel_PatternEditor();
        }

        private void onMouseDoubleClick_ListBox_StatementList(object sender, MouseButtonEventArgs e)
        {
            switch (mAdderMode)
            {
                case EAdderMode.CreateNew:
                    mLoadedPatternData = new PatternData("", "", new LinkedList<KeyValuePair<int, string>>(), new KeyValuePair<int, string>(mWindow_Popup.ListBox_StatementList.SelectedIndex, ""));
                    break;
                case EAdderMode.AddCondition:
                    if (mTargetNodeIndex == 0)
                    {
                        mLoadedPatternData.Conditions.AddFirst(new KeyValuePair<int, string>(mWindow_Popup.ListBox_StatementList.SelectedIndex, "None"));
                        break;
                    }

                    int nodeCount = 1;
                    LinkedListNode<KeyValuePair<int, string>> node = mLoadedPatternData.Conditions.First;
                    while (node != null)
                    {
                        if (nodeCount == mTargetNodeIndex)
                        {
                            break;
                        }

                        ++nodeCount;
                        node = node.Next;
                    }

                    mLoadedPatternData.Conditions.AddAfter(node, new KeyValuePair<int, string>(mWindow_Popup.ListBox_StatementList.SelectedIndex, "None"));
                    break;
            }

            refresh_StackPanel_PatternEditor();
            mWindow_Popup.Close();
        }

        private void onMouseLeftButtonUp_Path_Adder(object sender, MouseButtonEventArgs e)
        {
            System.Windows.Shapes.Path path_adder = (System.Windows.Shapes.Path)sender;
            Grid_StatementBlock block = (Grid_StatementBlock)path_adder.Parent;

            mAdderMode = EAdderMode.AddCondition;
            mTargetNodeIndex = block.NodeNumber;

            mWindow_Popup.Title = "조건 목록";
            mWindow_Popup.ListBox_StatementList.Background = Brushes.LightCyan;
            mWindow_Popup.ListBox_StatementList.Items.Clear();

            foreach (string statement in StatementManager.Conditions)
            {
                mWindow_Popup.ListBox_StatementList.Items.Add(statement);
            }

            mWindow_Popup.ShowDialog();
        }

        private void onMouseLeftButtonUp_Grid_XButton_PatternFile(object sender, MouseButtonEventArgs e)
        {
            File.Delete(((((sender as Grid).Parent as Grid).Parent as Border).Parent as Grid_PatternFile).Data.FilePath);
            MessageBox.Show("성공적으로 행동 규칙 파일을 삭제했습니다.", "알림", MessageBoxButton.OK, MessageBoxImage.Information);

            refresh_WrapPanel_PatternEditor_PatternList();
        }

        private void onMouseLeftButtonUp_Grid_XButton_Condition(object sender, MouseButtonEventArgs e)
        {
            Grid block = (Grid)sender;

            int nodeCount = 0;
            LinkedListNode<KeyValuePair<int, string>> node = mLoadedPatternData.Conditions.First;
            while (node != null)
            {
                if (nodeCount == (((block.Parent as Grid).Parent as Border).Parent as Grid_StatementBlock).NodeNumber)
                {
                    mLoadedPatternData.Conditions.Remove(node);
                    refresh_StackPanel_PatternEditor();
                    break;
                }

                ++nodeCount;
                node = node.Next;
            }
        }

        private void onMouseLeftButtonUp_Grid_XButton_Behaviour(object sender, MouseButtonEventArgs e)
        {
            mLoadedPatternData = null;
            StackPanel_PatternEditor.Children.Clear();
            TextBox_PatternName.Text = "";
        }
    }
}
