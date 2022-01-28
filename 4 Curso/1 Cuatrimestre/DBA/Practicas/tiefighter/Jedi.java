package tiefighter;

/**
 *
 * @author raul
 */
public class Jedi {
    public int x;
    public int y;
    public boolean captured;
    
    
    public void setPos(int x,int y){
        this.x = x;
        this.y = y;
    }
    
    public void captured(){
        this.captured = true;
    }
}


