using OpenTK.Graphics.OpenGL4;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace PhaseProj {
    public class Shader {
        public int _Handle;

        public Shader() {
            // loading source code of vertex and fragment shaders as strings

            Assembly assembly = Assembly.GetEntryAssembly()!;
            string contentPath = assembly.Location + "\\..\\Content\\";
            string fragPath = contentPath + "Shaders\\quad.frag";
            string vertPath = contentPath + "Shaders\\quad.vert";
            string vertexShaderSource = "";
            string fragmentShaderSource = "";

            using (StreamReader reader = new StreamReader(fragPath)) {
                fragmentShaderSource = reader.ReadToEnd();
            }

            using (StreamReader reader = new StreamReader(vertPath)) {
                vertexShaderSource = reader.ReadToEnd();
            }

            // create vertex and fragment shaders for positions and colors

            int vertexShader = GL.CreateShader(ShaderType.VertexShader);
            int fragmentShader = GL.CreateShader(ShaderType.FragmentShader);

            // load the text of the shader files into the shaders

            GL.ShaderSource(vertexShader, vertexShaderSource);
            GL.ShaderSource(fragmentShader, fragmentShaderSource);

            // compile shaders

            GL.CompileShader(vertexShader);
            GL.CompileShader(fragmentShader);

            // check for errors

            GL.GetShader(vertexShader, ShaderParameter.CompileStatus, out int vertexSuccess);
            GL.GetShader(fragmentShader, ShaderParameter.CompileStatus, out int fragmentSuccess);

            // create shader program

            _Handle = GL.CreateProgram();

            // attach shaders to shader program

            GL.AttachShader(_Handle, vertexShader);
            GL.AttachShader(_Handle, fragmentShader);

            // compiled data is copied to shader program when program is linked to OpenGL object, making individual shaders useless now
            
            GL.LinkProgram(_Handle);

            // individual shaders are not needed after the shader program has been created
            // saves memory

            GL.DetachShader(_Handle, vertexShader);
            GL.DetachShader(_Handle, fragmentShader);

            GL.DeleteShader(vertexShader);
            GL.DeleteShader(fragmentShader);


        }
    }
}
