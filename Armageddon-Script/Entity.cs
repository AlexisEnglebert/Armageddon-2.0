using System;
using System.Runtime.CompilerServices;
namespace Armageddon
{
    public abstract class Component
    {
        public Entity Ent { get; set; }
    }

    public class TagComponent : Component
    {
        public string Tag
        {
            get => GetTag(Ent.ID);
            set => SetTag(Ent.ID);
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string GetTag(ulong EntityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string SetTag(ulong EntityID);
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get { GetTranslation(Ent.ID, out float translationX, out float translationY, out float translationZ); return new Vector3(translationX, translationY, translationZ); }
            set { Log.error("Une valeur a ete set"); SetTranslation(Ent.ID, ref value.X, ref value.Y, ref value.Z); }
        }
        public Vector3 Rotation
        {
            get { GetRotation(Ent.ID, out float RotX, out float RotY, out float RotZ); return new Vector3(RotX, RotY, RotZ); }
            set { SetRotation(Ent.ID, ref value.X, ref value.Y, ref value.Z); }
        }
        public Vector3 Scale
        {
            get { GetScale(Ent.ID, out float ScaleX, out float ScaleY, out float ScaleZ); return new Vector3(ScaleX, ScaleY, ScaleZ); }
            set { SetScale(Ent.ID, ref value.X, ref value.Y, ref value.Z); }
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]

        private extern static void GetTranslation(ulong EntityID, out float translationX, out float translationY, out float translationZ);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetTranslation(ulong EntityID, ref float valueX, ref float valueY, ref float valueZ);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Vector3 GetRotation(ulong EntityID, out float RotX, out float RotY, out float RotZ);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Vector3 SetRotation(ulong EntityID, ref float valueX, ref float valueY, ref float valueZ);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]

        private extern static Vector3 GetScale(ulong EntityID, out float ScaleX, out float ScaleY, out float ScaleZ);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Vector3 SetScale(ulong EntityID, ref float valueX, ref float valueY, ref float valueZ);

        

    }

    public class Entity
    {
        protected Entity() { ID = 0; }
        public readonly ulong ID;
        internal Entity(int id)
        {
            ID = (ulong)id;
        }
        public bool HasComponent<T>() where T : Component, new()
        {
            return HasComponent_Internal(ID,typeof(T));
        }
        public T GetComponent<T>() where T : Component,new()
        {
            if(HasComponent<T>())
            {
                T component = new T();
                component.Ent = this;
                return component;
            }
            return null;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Internal(ulong EntityID, Type type);
    }
}
