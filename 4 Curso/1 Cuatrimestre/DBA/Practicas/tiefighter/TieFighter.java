
package tiefighter;

import appboot.LARVABoot;
import java.util.ArrayList;


public class TieFighter {
    public static void main(String[] args) {
        LARVABoot connection = new LARVABoot();
        String code = "105Wing1";
        
        connection.Boot("isg2.ugr.es", 1099);
        connection.launchAgent("DESTROYER-"+code, Destroyer.class);
        connection.launchAgent("FIGHTER1-"+code, Fighter.class);
        connection.launchAgent("FIGHTER2-"+code, Fighter.class);
        connection.launchAgent("FIGHTER3-"+code, Fighter.class);
        connection.launchAgent("FIGHTER4-"+code, Fighter.class);
        connection.launchAgent("CORELLIAN1-"+code, Corellian.class);
        connection.launchAgent("CORELLIAN2-"+code, Corellian.class);
        connection.launchAgent("CORELLIAN3-"+code, Corellian.class);
        connection.launchAgent("CORELLIAN4-"+code, Corellian.class);
        connection.WaitToShutDown();
        
         
    }
    
}
