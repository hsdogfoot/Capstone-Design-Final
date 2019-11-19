using System;
using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace Tool.SceneEditor
{
    class GamePreviewHwndHost : HwndHost
    {
        public int WindowWidth { get; set; }
        public int WindowHeight { get; set; }

        public GamePreviewHwndHost(int windowWidth, int windowHeight)
        {
            WindowWidth = windowWidth;
            WindowHeight = windowHeight;
        }

        public void RunGame()
        {
            UpdateGame();
            RenderGame();
        }

        public void Destroy()
        {
            DestroyGame();
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            IntPtr hInstance = Marshal.GetHINSTANCE(System.Reflection.Assembly.GetExecutingAssembly().GetModules()[0]);
            if (!CreateGame(hInstance, hwndParent.Handle, WindowWidth, WindowHeight))
            {
                System.Windows.Application.Current.Shutdown();
            }

            IntPtr childHwnd = GetWindowHandle();

            return new HandleRef(this, childHwnd);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            DestroyGame();
        }

        [DllImport("GameFramework.dll", EntryPoint = "CreateGame", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool CreateGame(IntPtr hInsatnce, IntPtr parentHandle, int width, int height);

        [DllImport("GameFramework.dll", EntryPoint = "GetWindowHandle", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr GetWindowHandle();

        [DllImport("GameFramework.dll", EntryPoint = "UpdateGame", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UpdateGame();

        [DllImport("GameFramework.dll", EntryPoint = "RenderGame", CallingConvention = CallingConvention.Cdecl)]
        private static extern void RenderGame();

        [DllImport("GameFramework.dll", EntryPoint = "DestroyGame", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DestroyGame();
    }
}
