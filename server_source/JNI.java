package testing;
public class JNI {
    static {
        System.loadLibrary("catoptric_cpp");
    }
    private native void test();
    public static void main(String[] args) {
        new JNI().test();  // Invoke native method
     }
}