package controller;

import java.util.Scanner;
import static java.lang.System.exit;
public class ControllerNative {
     static {
          System.loadLibrary("catoptric_cpp");
     }

     public boolean Block_mode = false;

     public native void moveMirror(int row, int col, int motor, int direction, int steps);

     public native void reset();

     public native void test();

     public native void quit();

     public void console() throws Exception {
          Block_mode = true;
          Scanner userInput = null;
          try {
               System.out.println("Welcome to use the local console to the Catoptric Array!");
               System.out.println(
                         "Please Enter your command: 'move' to move a single mirror, 'reset' to reset all mirrrors, 'test' to test all mirrors");
               System.out.println("type 'quit' to close the console");
               userInput = new Scanner(System.in);
               do {
                    String command = userInput.nextLine();
                    switch (command) {
                         case "quit":
                              exit(0);
                         case "reset":
                              reset();
                              break;
                         case "test":
                              test();
                              break;
                         case "move":
                              System.out.println("Enter row: ");
                              int row = userInput.nextInt();
                              System.out.println("Enter col: ");
                              int col = userInput.nextInt() -1;
                              System.out.println("Enter motor: ");
                              int motor = userInput.nextInt();
                              System.out.println("Enter direction: ");
                              int direction = userInput.nextInt();
                              System.out.println("Enter steps: ");
                              int steps = userInput.nextInt();
                              System.out.println("Hello Java!");
                              moveMirror(row, col, motor, direction, steps);
                              break;
                         default:
                              break;
                    }
               } while (true);
          } catch (IndexOutOfBoundsException e) {
               System.out.println(e.getMessage());
               System.out.println("Try again");
               console();
          } catch (Exception e) {
               e.printStackTrace();
          } finally {
               userInput.close();
          }
     }

}
