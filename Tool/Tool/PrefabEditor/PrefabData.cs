using System.Collections.Generic;
using System.Windows;

namespace Tool.PrefabEditor
{
    public class PrefabData
    {
        public string Name { get; private set; }
        public string FilePath { get; private set; }

        public string ImagePath { get; private set; }
        public Vector Scale { get; private set; }
        public double Rotation { get; private set; }

        public List<string> Patterns { get; private set; } = new List<string>();

        public PrefabData(string name, string filePath, string imagePath, Vector scale, double rotation, List<string> patterns)
        {
            Name = name;
            FilePath = filePath;

            ImagePath = imagePath;
            Scale = scale;
            Rotation = rotation;

            foreach (string data in patterns)
            {
                Patterns.Add(data);
            }
        }
    }
}
