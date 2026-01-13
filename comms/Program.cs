using System;
using System.Runtime.InteropServices;

namespace comms {
    internal class Program {
        [StructLayout(LayoutKind.Sequential)]
        public class MyStruct {
            public int a;
            public char b;
        }
        private const string dllPath = @"C:\Users\myrbear\Documents\proj\comms\x64\Debug\comms2.dll";

        [DllImport(dllPath)]
        private static extern int CppFunction(MyStruct myStruct);
        static void Main(string[] args) {
            MyStruct myStruct = new MyStruct();
            myStruct.a = 10;
            myStruct.b = 'a';

            int valueFromCpp = CppFunction(myStruct);

            Console.WriteLine("return: " + valueFromCpp);
        }
    }
}
