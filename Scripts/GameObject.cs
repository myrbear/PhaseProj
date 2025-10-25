
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PhaseProj {

    public class GameObject {

        private Quad _Quad;
        private Transform2D _Transform;

        public GameObject() { 

            _Quad = new Quad(this);
            _Transform = new Transform2D();
            //_Transform._Position = new OpenTK.Mathematics.Vector2(0.1f);
            _Transform._Scale = new OpenTK.Mathematics.Vector2(0.5f);
            _Transform.UpdateTransform();
        }

        public void Update() {
            _Transform._Rotation += 1f * GameEngine.GetInstance().GetDeltaTime();
            _Transform.UpdateTransform();
        }
        public Transform2D GetTransform() {
            return _Transform;
        }
        public Quad GetQuad() {
            return _Quad;
        }
    }
}
