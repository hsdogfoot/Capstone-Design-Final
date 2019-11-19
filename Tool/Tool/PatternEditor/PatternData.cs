using System.Collections.Generic;

namespace Tool.PatternEditor
{
    public class PatternData
    {
        public string Name { get; private set; }
        public string FilePath { get; private set; }

        public LinkedList<KeyValuePair<int, string>> Conditions { get; set; }
        public KeyValuePair<int, string> Behaviour { get; set; }

        public PatternData(string name, string filePath, LinkedList<KeyValuePair<int, string>> conditions, KeyValuePair<int, string> behaviour)
        {
            Name = name;
            FilePath = filePath;

            Conditions = conditions;
            Behaviour = behaviour;
        }
    }
}
