package fileReader;
import controller.ControllerNative;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import static java.lang.System.exit;

public class CatoptricFileReader {
    private final ControllerNative controller;

    public CatoptricFileReader() {
        controller = new ControllerNative();
    }

    public void sendFileToController(String filename) throws IOException {
        BufferedReader bufferedReader = null;
        try {
            FileReader fileReader = new FileReader(filename);
            bufferedReader = new BufferedReader(fileReader);
            String line;
            //Assume the file is in the correct format
            //Anton and I discussed this, and we decided the format as follows:
            //row col motor direction steps
            while ((line = bufferedReader.readLine()) != null) {
                String[] tokens = line.trim().split("\\s+");
                int row = Integer.parseInt(tokens[0]);
                int col = Integer.parseInt(tokens[1]);
                int motor = Integer.parseInt(tokens[2]);
                int direction = Integer.parseInt(tokens[3]);
                int steps = Integer.parseInt(tokens[4]);
                controller.moveMirror(row, col, motor, direction, steps);
            }
        } catch (IndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
            System.out.println("Entering command line mode");
            invokeConsole();
        }finally {
            if(bufferedReader != null)bufferedReader.close();
        }
    }

    public void invokeConsole() {
         try{
            controller.console();
         }catch(Exception e){
            System.out.println("unknown exception caught");
            exit(0);
         }  
    }

    public static void main(String[] args) {
        CatoptricFileReader fileReader = new CatoptricFileReader();
        //There are four cases:
        //case 1: args.length == 0, then invoke console
        //case 2: args.length == 2 and the second arg is -f, then send the file to controller
        //case 3: args.length == 2 and the second arg is not -f, then exit with error message
        //case 4: anything else, then exit with error message
        if (args.length == 0){
            fileReader.invokeConsole();
        }
        else if (args.length == 2){
            if (!args[1].equals("-f")){
            System.out.println("Invalid command line arguments");
            exit(1);
            }
            else{
                try {
                    fileReader.sendFileToController(args[0]);
                }catch (IOException e){
                    System.out.println(e.getMessage());
                    System.out.print("Enter command line mode....");
                    fileReader.invokeConsole();
                }finally {
                    exit(0);
                }
            }
        }
        else{
            System.out.println("Invalid command line arguments");
            exit(1);
        }
    }
}
