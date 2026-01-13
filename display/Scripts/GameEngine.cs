
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

        private const int MAX_GAME_OBJECTS = 20;

        private GameObject[] _GOs;
        private float _DeltaTime;
        private static GameEngine _Instance;

        public GameEngine() : base(GameWindowSettings.Default, NativeWindowSettings.Default) {

            _GOs = new GameObject[MAX_GAME_OBJECTS];

            Title = "PHASE ver-1.0.0";
            CursorState = CursorState.Normal;
            WindowBorder = WindowBorder.Fixed;
            WindowState = WindowState.Normal;

            GL.Enable(EnableCap.DepthTest);
            GL.DepthFunc(DepthFunction.Less);
            CenterWindow();

            _GOs[0] = new GameObject();
            _GOs[1] = new GameObject();

            //ground._Transform._Position = new Vector2(0, 0.1f);

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

            for (int i = 0; i < MAX_GAME_OBJECTS; i++) {
            
                var go = _GOs[i];

                if (go != null) {

                    go.Update();
                    go.GetQuad().Draw();
                }
            }

            Context.SwapBuffers();
        }
    }
}
