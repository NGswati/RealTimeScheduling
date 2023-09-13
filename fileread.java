import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class fileread {
    public static void main(String[] args) {
        Scanner sc = null;
        try {
            sc = new Scanner(new File("Set_1_Input_m_2_U_40_n_20.txt"));
        } catch (FileNotFoundException e) {
            System.out.println("Error opening file");
            System.exit(1);
        }

        while (sc.hasNextLine()) {
            String line = sc.nextLine();
            String[] values = line.split("\t");
            for (String value : values) {
                System.out.print(value + "\t");
            }
            System.out.println();
        }

        sc.close();
    }
}