using System.Collections.Generic;

namespace Tool.PatternEditor
{
    public static class StatementManager
    {
        public static List<string> Conditions { get; private set; } = new List<string>();
        public static List<string> Behaviours { get; private set; } = new List<string>();

        public static void Initialize()
        {
            Conditions.Add("__ 키를 눌렀다면");
            Conditions.Add("__ 키를 누르고 있다면");
            Conditions.Add("__ 키를 뗐다면");
            Conditions.Add("__ 와 충돌했다면");
            Conditions.Add("죽었다면");

            Behaviours.Add("__ 의 속도로 수평 이동한다.");
            Behaviours.Add("__ 의 속도로 수직 이동한다.");
            Behaviours.Add("__ 의 힘으로 점프한다.");
            Behaviours.Add("죽는다.");
        }
    }
}
