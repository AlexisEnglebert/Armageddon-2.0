using System.Runtime.CompilerServices;


namespace Armageddon
{
    public class Log
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void info(string message);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void error(string message);
    }
}
