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
            get {  GetTranslation(Ent.ID,out Vector3 translation); return translation; }
            set => SetTranslation(Ent.ID);
        }
        public Vector3 Rotation
        {
            get => GetRotation(Ent.ID);
            set => SetRotation(Ent.ID);
        }
        public Vector3 Scale
        {
            get => GetScale(Ent.ID);
            set => SetScale(Ent.ID);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]

        private extern static Vector3 GetTranslation(ulong EntityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Vector3  SetTranslation(ulong EntityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]

        private extern static Vector3 GetRotation(ulong EntityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Vector3 SetRotation(ulong EntityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]

        private extern static Vector3 GetScale(ulong EntityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Vector3 SetScale(ulong EntityID);

    }

    public class Entity
    {
        public ulong ID { get; set; }
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
