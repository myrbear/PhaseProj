
using OpenTK.Windowing.Desktop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Windowing.Common;
using OpenTK.Mathematics;

using System;
using System.Runtime.InteropServices;

namespace PhaseProj {

    public class GameEngine : GameWindow {

        private const int MAX_VERTS = 10;

        [StructLayout(LayoutKind.Sequential)]
        public class MyStruct {
            public float x0;
            public float x1;
            public float x2;

            public float y0;
            public float y1;
            public float y2;

            public float xp;
            public float yp;
        }
        private const string dllPath = @"C:\Users\mzlaf\Documents\proj\phase_proj\comms\x64\Debug\comms2.dll";

        [DllImport(dllPath)]
        private static extern int CppFunction(MyStruct A, MyStruct B);

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

            _GOs[0].GetTransform()._Position = new Vector2(0.2f, 0.2f);
            _GOs[1].GetTransform()._Position = new Vector2(-0.45f, -0f);

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
        private float Lerp(float start, float end, float time) {
            float offset = end - start;
            return start + offset * time;
        }
        private float _Elapsed;
        private bool _Trigger;
        protected override void OnUpdateFrame(FrameEventArgs args) {

            _DeltaTime = (float)args.Time;
            _Elapsed += _DeltaTime;

            base.OnUpdateFrame(args);
            GL.ClearColor(new Color4(0.4f, 0.4f, 0.4f, 1f));
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            _GOs[0].GetTransform()._Position.X = Lerp(0.2f, -0.2f, _Elapsed / 4f);

            if (_Elapsed >= 4) {
                _Elapsed = 0;
                _Trigger = true;
            }

            for (int i = 0; i < MAX_GAME_OBJECTS - 1; i++) {
            
                var go = _GOs[i];
                var go1 = _GOs[i + 1];

                if (go != null) {
                    go.Update();
                }

                if (go != null && go1 != null) {
                    MyStruct A = new MyStruct();
                    var verts0 = go.GetQuad().GetVertsLocal();

                    A.x0 = verts0[0].X;
                    A.x1 = verts0[1].X;
                    A.x2 = verts0[2].X;

                    A.y0 = verts0[0].Y;
                    A.y1 = verts0[1].Y;
                    A.y2 = verts0[2].Y;

                    A.xp = go.GetTransform()._Position.X;
                    A.yp = go.GetTransform()._Position.Y;

                    //A.len = verts0.Length;
                    //A.ys = new float[A.len];

                    //for (int j = 0; j < verts0.Length; j++) {
                    //    A.xs[j] = verts0[j].X;
                    //    A.ys[j] = verts0[j].Y;
                    //}

                    MyStruct B = new MyStruct();
                    var verts1 = go1.GetQuad().GetVertsLocal();

                    //B.len = verts1.Length;
                    //B.xs = new float[B.len];
                    //B.ys = new float[B.len];

                    //for (int j = 0; j < verts1.Length; j++) {
                    //    B.xs[j] = verts1[j].X;
                    //    B.ys[j] = verts1[j].Y;
                    //}
                    B.x0 = verts1[0].X;
                    B.x1 = verts1[1].X;
                    B.x2 = verts1[2].X;

                    B.y0 = verts1[0].Y;
                    B.y1 = verts1[1].Y;
                    B.y2 = verts1[2].Y;

                    B.xp = go1.GetTransform()._Position.X;
                    B.yp = go1.GetTransform()._Position.Y;

                    int valueFromCpp = CppFunction(A, B);

                        if (valueFromCpp == 1) {
                            go.GetQuad()._Color = new Vector3(0.5f, 0.5f, 0.5f);
                        }
                        else {
                            go.GetQuad()._Color = new Vector3(1f, 1f, 1f);
                        }

                }

                if (go != null) {
                    go.GetQuad().Draw();
                }
            }

            Context.SwapBuffers();
        }
    }
}
