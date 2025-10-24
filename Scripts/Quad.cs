using OpenTK.Graphics.OpenGL4;
using OpenTK.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PhaseProj {
    public class Quad {
        public int[] _Indices;
        public float[] _Vertices;
        public int _VAO;
        public int _VBO;
        public int _EBO;
        private Shader _Shader;

        public Quad() {
            _Indices = new int[]
                {
                    2, 1, 0,
                    //3, 2, 0
                };
            _Vertices = new float[]
               {
                    // position             texture coordinates
                    1.0f,   1.0f,   0.0f,   // top right
                    1.0f,  -1.0f,   0.0f,   // bottom right
                   -1.0f,  -1.0f,   0.0f,   // bottom left
                   -1.0f,   1.0f,   0.0f,   // top left
               };

            // VAO

            _VAO = GL.GenVertexArray();
            GL.BindVertexArray(_VAO);

            // VBO

            _VBO = GL.GenBuffer();

            GL.BindBuffer(BufferTarget.ArrayBuffer, _VBO);
            GL.BufferData(BufferTarget.ArrayBuffer, _Vertices.Length * sizeof(float), _Vertices, BufferUsageHint.StaticDraw);

            int strideCount = 3;
            int vertexStride = strideCount * sizeof(float);

            GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, vertexStride, 0);
            GL.EnableVertexAttribArray(0);
            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);

            // EBO

            _EBO = GL.GenBuffer();
            GL.BindBuffer(BufferTarget.ElementArrayBuffer, _EBO);
            GL.BufferData(BufferTarget.ElementArrayBuffer, _Indices.Length * sizeof(uint), _Indices, BufferUsageHint.StaticDraw);
            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);

            _Shader = new Shader();
        }
        ~Quad() {
            Console.WriteLine("Quad not disposed.");
        }

        public void Dispose() {
            GL.BindBuffer(BufferTarget.ElementArrayBuffer, 0);
            GL.DeleteBuffer(_EBO);

            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
            GL.DeleteBuffer(_VBO);
            
            GL.BindVertexArray(0);
            GL.DeleteVertexArray(_VAO);

            GC.SuppressFinalize(this);
        }
        public void Draw() {
            GL.BindVertexArray(_VAO);
            GL.UseProgram(_Shader._Handle);
            GL.DrawElements(OpenTK.Graphics.OpenGL4.PrimitiveType.Triangles, _Indices.Length, DrawElementsType.UnsignedInt, 0);
            GL.BindVertexArray(0);
        }
    }
}
