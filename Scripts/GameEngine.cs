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
        private Quad quad;

        public GameEngine() : base(GameWindowSettings.Default, NativeWindowSettings.Default) {
            Title = "PHASE ver-1.0.0";
            CursorState = CursorState.Normal;
            WindowBorder = WindowBorder.Fixed;
            WindowState = WindowState.Normal;

            CenterWindow();

            quad = new Quad();
        }
        protected override void OnFramebufferResize(FramebufferResizeEventArgs e) {
            base.OnFramebufferResize(e);
            GL.Viewport(0, 0, 512, 288);
        }
        protected override void OnUpdateFrame(FrameEventArgs args) {
            base.OnUpdateFrame(args);
            GL.ClearColor(new Color4(0.4f, 0.4f, 0.4f, 1f));
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            quad.Draw();
            Context.SwapBuffers();
        }
    }
}
