
using OpenTK.Windowing.Desktop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Windowing.Common;
using OpenTK.Mathematics;

namespace PhaseProj {

    public class GameEngine : GameWindow {

        private GameObject quad;
        private float _DeltaTime;
        private static GameEngine _Instance;

        public GameEngine() : base(GameWindowSettings.Default, NativeWindowSettings.Default) {
            
            Title = "PHASE ver-1.0.0";
            CursorState = CursorState.Normal;
            WindowBorder = WindowBorder.Fixed;
            WindowState = WindowState.Normal;

            GL.Enable(EnableCap.DepthTest);
            GL.DepthFunc(DepthFunction.Less);
            CenterWindow();

            quad = new GameObject();

            _Instance = this;
        }

        public static GameEngine GetInstance() {

            return _Instance;
        }
        public float GetDeltaTime() {

            return _DeltaTime;
        }
        protected override void OnFramebufferResize(FramebufferResizeEventArgs e) {
            
            base.OnFramebufferResize(e);
            GL.Viewport(0, 0, 512, 288);
        }
        protected override void OnUpdateFrame(FrameEventArgs args) {

            _DeltaTime = (float)args.Time;

            base.OnUpdateFrame(args);
            GL.ClearColor(new Color4(0.4f, 0.4f, 0.4f, 1f));
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            quad.Update();
            quad.GetQuad().Draw();
            Context.SwapBuffers();
        }
    }
}
