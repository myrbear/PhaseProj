using OpenTK.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PhaseProj {
    public class Rigidbody {

        private GameObject _Parent;
        private Vector2 _Velocity;

        public Rigidbody(GameObject parent) {

            _Parent = parent;
        }

        public void ApplyGravity() {

            _Velocity.Y -= 0.981f * GameEngine.GetInstance().GetDeltaTime();
        }
    }
}
