package fileReader;

import controller.ControllerNative;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.File;

import static java.lang.System.exit;

public class CatoptricFileReader {
    private final ControllerNative controller;

    public CatoptricFileReader() {
        controller = new ControllerNative();
    }

    //filename is arg[1], pretty sure this gets passed in by start.sh I'll figure this out later :)
    public void sendFileToController(String filename) throws IOException {
        BufferedReader bufferedReader = null;
        try {
            String filePath = filename;
            // For some reason cwd is resource, unsure why
            // I'll just have my python script write the file to here from now on
            FileReader fileReader = new FileReader(filePath);
            bufferedReader = new BufferedReader(fileReader);
            String line;
            // Assume the file is in the correct format
            // Anton and I discussed this, and we decided the format as follows:
            // row col motor direction steps
            while ((line = bufferedReader.readLine()) != null) {
                String[] tokens = line.trim().split("\\s+");
                int row = Integer.parseInt(tokens[0]);
                int col = Integer.parseInt(tokens[1]); // c++ counts colunm starting at 0 // Anton: removed -1 here
                int motor = Integer.parseInt(tokens[2]);
                int direction = Integer.parseInt(tokens[3]);
                int steps = Integer.parseInt(tokens[4]);
                System.out.println("here");
                controller.moveMirror(row, col, motor, direction, steps);
            }
        } catch (IndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
            System.out.println("Entering command line mode");
            invokeConsole();
        } finally {
            if (bufferedReader != null)
                bufferedReader.close();
        }
    }

    public void invokeConsole() {
        try {
            controller.console();
        } catch (Exception e) {
            System.out.println("unknown exception caught");
            exit(0);
        }
    }

    public static void main(String[] args) {
        CatoptricFileReader fileReader = new CatoptricFileReader();
        // There are four cases:
        // case 1: args.length == 0, then invoke console
        // case 2: args.length == 2 and the second arg is -t, then send the
        // file to controller. After that is done, swith to console mode
        // case 3: args.length == 2 and the second arg is not -t, then exit with error
        // message
        // case 4: anything else, then exit with error message
        if (args.length == 0) {
            fileReader.invokeConsole();
        } else if (args.length == 2) {
            if (!((args[0]).equals("-t "))) {
                System.out.println("Invalid command line arguments");
                exit(1);
            } else {
                try {
                    fileReader.sendFileToController(args[1]);
                } catch (IOException e) {
                    System.out.println(e.getMessage());
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                    System.out.println("Unknown Error Caught");
                } finally {
                    System.out.print("Enter command line mode....");
                    fileReader.invokeConsole();
                    exit(0);
                }
            }
        } else {
            System.out.println("Invalid command line arguments");
            System.out.println(args[0] + args[1]);
            exit(1);
        }
    }
}
