package testing;
public class JNI {
    static {
        System.loadLibrary("testing"); // Load native library libcatoptric_cpp.so (Unixes)
                                     // This library contains a native method called test()
     }
    private native void test();
    public static void main(String[] args) {
        new JNI().test();  // Invoke native method
     }
}