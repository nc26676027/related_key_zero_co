import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;

public class Find4 {

    public static void main(String[] args){

            try {
                     int ALLFILE=256;
                     int counter=0;
                     String str="";
         String[] command_run={"/usr/local/bin/stp","","--cryptominisat","--threads","8"};
         Process process=null;
         InputStreamReader isr=null;
         LineNumberReader lnr=null;
                     String result="";


                             for(int fileno=0;fileno<16;fileno++)
                            {
                                str=("/home/niuchao/new_mode_search20200203/skinny_n-2n/key_state"+args[0]+"/head_state4/skinny"+fileno+".cvc");
                                command_run[1]=str;
                                process=Runtime.getRuntime().exec(command_run);
                                isr=new InputStreamReader(process.getInputStream());
                                lnr=new LineNumberReader(isr);
                                counter=0;
                                while((result=lnr.readLine())!=null){
                                        counter++;
                                        if(counter>1){
                                               break;
                                        }
                                }
                                if(counter==1)
                                {
                                   System.out.println("Valid : "+fileno+"\n");
                                }
                                isr.close();
                                lnr.close();
                             }

            } catch (IOException ioe) {
                    System.out.println("Wrong!");
                    ioe.printStackTrace();
            }
    }
}
