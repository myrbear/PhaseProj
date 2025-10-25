
using OpenTK.Mathematics;

namespace PhaseProj {

    public class Transform2D {

        public Vector2 _Scale;
        public float _Rotation;
        public Vector2 _Position;
        public Matrix4 _Transform;

        public Transform2D() {

            _Scale = Vector2.One;
            _Rotation = 0;
            _Position = Vector2.Zero;
        }
        public Transform2D(Vector2 scale, float rotation, Vector2 position) {

            // [-] is up
            // [+] is down

            _Scale = scale;
            _Rotation = rotation;
            _Position = position;

            UpdateTransform();
        }

        public void UpdateTransform() {

            Matrix4 scale = Matrix4.CreateScale(new Vector3(_Scale.X, _Scale.Y, 1f));

            Quaternion rotQuaternion = Quaternion.FromEulerAngles(new Vector3(0f, 0f, _Rotation));
            Matrix4 rotation = Matrix4.CreateFromQuaternion(rotQuaternion);
            Matrix4 position = Matrix4.CreateTranslation(new Vector3(_Position.X, _Position.Y, 0f));

            _Transform = scale * rotation * position;

            // a zero rotation in a transform gets extracted as NaN

            if (_Rotation == float.NaN)
            {
                _Rotation = 0;
            }
        }
    }
}
