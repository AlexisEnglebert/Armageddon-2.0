using System;
using System.Runtime.InteropServices;

namespace Armageddon
{
    [StructLayout(LayoutKind.Sequential)]

    public class Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public Vector3(float Value)
        {
            X = Value;
            Y = Value;
            Z = Value;
        }
        public Vector3(float x,float yz)
        {
            X = x;
            Y = yz;
            Z = yz;
        }
        public Vector3(float x , float y , float z )
        {
            X = x;
            Y = y;
            Z = z;
        }
        public Vector3(Vector3 vec)
        {
            X = vec.X;
            Y = vec.Y;
            Z = vec.Z;
        }

        /*Multiplication*/
        public static Vector3 operator * (Vector3 vector, float value)
        {
            return new Vector3(vector.X * value, vector.Y * value, vector.Z * value);
        }
        public static Vector3 operator * (float value,Vector3 vector)
        {
            return new Vector3(vector.X * value, vector.Y * value, vector.Z * value);
        }
        /*Division*/
        public static Vector3 operator /(Vector3 vector, float value)
        {
            return new Vector3(vector.X / value, vector.Y / value, vector.Z / value);
        }
        public static Vector3 operator /(float value, Vector3 vector)
        {
            return new Vector3(vector.X / value, vector.Y / value, vector.Z / value);
        }
        /*Addition*/
        public static Vector3 operator + (Vector3 vector, float value)
        {
            return new Vector3(vector.X + value, vector.Y + value, vector.Z + value);
        }
        public static Vector3 operator + (float value, Vector3 vector)
        {
            return new Vector3(vector.X + value, vector.Y + value, vector.Z + value);
        }

        /*Soustraction*/
        public static Vector3 operator - (Vector3 vector, float value)
        {
            return new Vector3(vector.X - value, vector.Y - value, vector.Z - value);
        }
        public static Vector3 operator - (float value, Vector3 vector)
        {
            return new Vector3(vector.X - value, vector.Y - value, vector.Z - value);
        }
    }
}
