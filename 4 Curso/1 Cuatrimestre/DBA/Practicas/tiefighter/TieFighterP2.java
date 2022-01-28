package tiefighter;

import agents.LARVAFirstAgent;
import jade.core.AID;
import jade.lang.acl.ACLMessage;
import static java.lang.Math.*;
import swing.LARVADash;
import java.util.ArrayList;

public class TieFighterP2 extends LARVAFirstAgent{
    enum Status {
        CHECKIN, CHECKOUT, OPENPROBLEM,COMISSIONING,JOINSESSION, SOLVEPROBLEM, CLOSEPROBLEM, EXIT
    }
    Status mystatus;
    String service = "PManager", problem = "Hoth",
            problemManager = "", content, sessionKey, sessionManager,storeManager,sensorKeys,nextAction;
    int width, height, maxFlight;
    ACLMessage open, session;
    String[] contentTokens,mySensors = new String[]{
        "ALIVE",
        "GPS",
        "COMPASS",
        "LIDAR",
        "ENERGY",
        "DISTANCE",
        "ANGULAR", 
    };
    boolean step = true,goRecharge = true,resuelto = false;
    
    Orientation nextSquare;
    
    ArrayList<Square> memory = new ArrayList<Square>();

    @Override
    public void setup() {
        super.setup();
        logger.onOverwrite();
        logger.setLoggerFileName("mylog.json");
        this.enableDeepLARVAMonitoring();
        Info("Setup and configure agent");
        mystatus = Status.CHECKIN;
        exit = false;
        this.myDashboard = new LARVADash(this);
        //this.doActivateLARVADash();
    }

    @Override
    public void Execute() {
        Info("Status: " + mystatus.name());
        if (step) {
            step = this.Confirm("The next status will be " + mystatus.name() + "\n\nWould you like to continue step by step?");
        }
        switch (mystatus) {
            case CHECKIN:
                mystatus = MyCheckin();
                break;
            case OPENPROBLEM:
                mystatus = MyOpenProblem();
                break;
            case COMISSIONING:
                mystatus = MyComissioning();
                break;
            case JOINSESSION:
                mystatus = MyJoinSession();
                break;    
            case SOLVEPROBLEM:
                mystatus = MySolveProblem();
                break;
            case CLOSEPROBLEM:
                mystatus = MyCloseProblem();
                break;
            case CHECKOUT:
                mystatus = MyCheckout();
                break;
            case EXIT:
            default:
                exit = true;
                break;
        }
    }

    @Override
    public void takeDown() {
        Info("Taking down and deleting agent");
        this.saveSequenceDiagram("./" + this.problem + ".seqd");
        super.takeDown();
    }

    public Status MyCheckin() {
        Info("Loading passport and checking-in to LARVA");
        if (!loadMyPassport("MyPassport.passport")) {
            Error("Unable to load passport file");
            return Status.EXIT;
        }
        if (!doLARVACheckin()) {
            Error("Unable to checkin");
            return Status.EXIT;
        }
        return Status.OPENPROBLEM;
    }

    public Status MyOpenProblem() {
        if (this.DFGetAllProvidersOf(service).isEmpty()) {
            Error("Service PMANAGER is down");
            return Status.CHECKOUT;
        }
        problemManager = this.DFGetAllProvidersOf(service).get(0);
        Info("Found problem manager " + problemManager);
        this.outbox = new ACLMessage();
        outbox.setSender(getAID());
        outbox.addReceiver(new AID(problemManager, AID.ISLOCALNAME));
        outbox.setContent("Request open " + problem);
        this.LARVAsend(outbox);
        Info("Request opening problem " + problem + " to " + problemManager);
        open = LARVAblockingReceive();
        Info(problemManager + " says: " + open.getContent());
        content = open.getContent();
        contentTokens = content.split(" ");
        if (contentTokens[0].toUpperCase().equals("AGREE")) {
            sessionKey = contentTokens[4];
            session = LARVAblockingReceive();
            sessionManager = session.getSender().getLocalName();
            Info(sessionManager + " says: " + session.getContent());
            return Status.COMISSIONING;
        } else {
            Error(content);
            return Status.CHECKOUT;
        }
    }

    public Status MyCloseProblem() {
        outbox = open.createReply();
        outbox.setContent("Cancel session " + sessionKey);
        Info("Closing problem, session " + sessionKey);
        this.LARVAsend(outbox);
        inbox = LARVAblockingReceive();
        Info(problemManager + " says: " + inbox.getContent());
        return Status.CHECKOUT;
    }

    public Status MyCheckout() {
        this.doLARVACheckout();
        return Status.EXIT;
    }
    
    public Status MyComissioning() {
        String localService="STORE "+sessionKey;
        if (this.DFGetAllProvidersOf(localService).isEmpty()){
            Error("Service STORE is down");
            return Status.CLOSEPROBLEM;
        }
        storeManager = this.DFGetAllProvidersOf(localService).get(0);
        Info("Found store manager " + storeManager);
        sensorKeys="";
        for (String s: mySensors){
            outbox = new ACLMessage();
            outbox.setSender(this.getAID());
            outbox.addReceiver(new AID(storeManager, AID.ISLOCALNAME));
            outbox.setContent("Request product "+s+" session "+sessionKey);
            this.LARVAsend(outbox);
            inbox = this.LARVAblockingReceive();
            if (inbox.getContent().startsWith("Confirm")){
                Info("Bought sensor "+s);
                sensorKeys += inbox.getContent().split(" ")[2]+" ";
            }else{
                this.Alert("Sensor "+s+" could not be obtained");
                return Status.CLOSEPROBLEM;
            }
        }
        Info("Bought all sensor keys "+sensorKeys);
        return Status.JOINSESSION;
    }
    
    public Status MyJoinSession() { 
        session = session.createReply();
        session.setContent("Request join session "+sessionKey+" attach sensors "+sensorKeys);
        this.LARVAsend(session);
        session = this.LARVAblockingReceive();
        String parse[] = session.getContent().split(" ");
        if(parse[0].equals("Confirm")){
            width = Integer.parseInt(parse[8]);
            height = Integer.parseInt(parse[10]);
            maxFlight = Integer.parseInt(parse[14]);
            return Status.SOLVEPROBLEM;
        }
        else{
            Alert("Error joining session: "+session.getContent());
            return Status.CLOSEPROBLEM;
        } 
    }
    
    public boolean updateSensors(){
        session = session.createReply();
        session.setContent("Query sensors session "+sessionKey);
        this.LARVAsend(session);
        session = this.LARVAblockingReceive();
            if (session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure")){
                Alert("Reading of sensors failed due to "+session.getContent());
                return false;
            }
        return true;
    }
    
    /**
     *
     * @author Raúl , Alejandro e Ignacio
     */
    public int[] posicionObjetivo(){
        double distance = myDashboard.getDistance();
        double angular = Math.toRadians(myDashboard.getAngular());
        double gps[] = myDashboard.getGPS();
        double X_inc = distance * cos(angular);
        double Y_inc = distance * sin(angular);
        double X = gps[0]+X_inc;
        double Y =(-gps[1])+Y_inc;        
        
        return new int [] {(int)X,(int)-Y};
    }
    
 /**
 *
 * @author Raúl 
 */
    public void considerRecharge(){
        double energy = myDashboard.getEnergy();
        double altimeter = myDashboard.getLidar()[5][5];
        double movsToRecharge = altimeter;
        double energyToRecharge = movsToRecharge*mySensors.length;
        if(energy <= energyToRecharge+(mySensors.length*2)){
            goRecharge = true;
        }
    }
    
    /**
 *
 * @author Raúl 
 */
    public void goToRecharge(){
        int altimeter = (int)myDashboard.getLidar()[5][5];
        int downTimes = altimeter / 5;
            for(int i = 0; i < downTimes; i++)
                this.sendAction("DOWN"); 
        this.sendAction("RECHARGE");
        goRecharge = false;
    }
/**
 *
 * @author Raúl 
 */
    public void chooseAction(){
        int posObjetivo[] = posicionObjetivo();
        double gps[] = myDashboard.getGPS();
        int altimeter = (int)myDashboard.getLidar()[5][5];
        
        int lidar[][] = myDashboard.getLidar();
        String message = "Reading of sensor Thermal;\n";
        for(int y = 0; y < lidar.length;y++){
            for(int x = 0; x < lidar[0].length;x++){
                message += String.format("%03d ", lidar[x][y]);
            }
            message += "\n";
        }
        Info(message);
        
        Info("Target: x="+posObjetivo[0]+" , y="+posObjetivo[1]);
        Info("Actual: x="+gps[0]+" , y="+gps[1]);
        if(posObjetivo[0] == gps[0] && posObjetivo[1] == gps[1]){
            int downTimes = altimeter / 5;
            for(int i = 0; i < downTimes; i++)
                this.sendAction("DOWN");
            this.sendAction("CAPTURE");
            resuelto = true;
        }
        else{
            this.heuristic();
        }
           
    }
    
    /**
 *
 * @author Raúl 
 */
    //DEVUELVE EL ANGULO QUE EQUIVALE A LA ORIENTACION
    public int getAngle(Orientation or){
        int angulo = 0;
        switch (or) {
            case E:
                angulo = 0;
                break;
            case N:
                angulo = 90;
                break;
            case S:
                angulo = 270;
                break;
            case W:
                angulo = 180;
                break;    
            case NE:
                angulo = 45;
                break;
            case NW:
                angulo = 135;
                break;
            case SE:
                angulo = 315;
                break;
            case SW:
                angulo = 225;
                break;
            default:
                break;
        }
        return angulo;
    }
    
    /**
 *
 * @author Raúl 
 */
    //DEVUELVE LA ALTURA DEL TERRENO EN BASE A NUESTRA ALTURA Y AL LIDAR
    public int getHeightOfLand(int height){
        int altTF = (int)myDashboard.getGPS()[2];
        
        return altTF - height;
    }
    
    /**
 *
 * @author Raúl 
 */
    //DEVUELVE EL VALOR LIDAR DEPENDIENDO DE LA ORIENTACION QUE LE DIGAMOS
    public int getLidar(Orientation Orientation){
        int lidar[][] = myDashboard.getLidar();
        int lor = 0;
        switch (Orientation) {
            case E:
                lor = lidar[5][6];
                break;
            case N:
                lor = lidar[4][5];
                break;
            case S:
                lor = lidar[6][5];
                break;
            case W:
                lor = lidar[5][4];
                break;    
            case NE:
                lor = lidar[4][6];
                break;
            case NW:
                lor = lidar[4][4];
                break;
            case SE:
                lor = lidar[6][6];
                break;
            case SW:
                lor = lidar[6][4];
                break;
            default:
                break;
        }
        return lor;
    }
    
    /**
 *
 * @author Raúl 
 */
    //DEVUELVE UN ARRAY CON LAS ALTURAS DEL TERRENO CIRCUNDANTES AL TIEFIGHTER ORDENADAS POR INTERES SEGUN EL ANGULAR
    public ArrayList<Integer> getNearHeights(){
        ArrayList<Integer> heights = new ArrayList<Integer>();
        double angular = myDashboard.getAngular();
        if(angular <= 22.5 || angular > 337.5 && myDashboard.getLidar()[6][5] > -this.maxFlight){
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
        }else if(angular <= 67.5 && angular > 22.5 && myDashboard.getLidar()[6][5] > -this.maxFlight){
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
        }else if(angular <= 112.5 && angular > 67.5){
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
        }else if(angular <= 157.5 && angular > 112.5){
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
        }else if(angular <= 202.5 && angular > 157.5){
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
        }else if(angular <= 247.5 && angular > 202.5){
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
        }else if(angular <= 292.5 && angular > 247.5){
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
        }else if(angular <= 337.5 && angular > 292.5){
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
        }
        return heights;
    }
    
    /**
 *
 * @author Raúl 
 */
    //DEVUELVE UN ARRAY QUE COMPLEMENTA AL ANTERIOR, INDICANDO EN QUE ORIENTACIONES SON ESOS VALORES
    public ArrayList<Orientation> getOrientations(){
        ArrayList<Orientation> orientations = new ArrayList<Orientation>();
        int angular = (int)myDashboard.getAngular();
        
        if( (angular <= 22.5 && angular >= 0)  || (angular > 337.5 && angular <= 360)){
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.SE); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.W); 
        }else if(angular <= 67.5 && angular > 22.5){
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.SE); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SW); 
        }else if(angular <= 112.5 && angular > 67.5){
                orientations.add(Orientation.N); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.E); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.SE); 
                orientations.add(Orientation.S); 
        }else if(angular <= 157.5 && angular > 112.5){
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.E); 
                orientations.add(Orientation.SE); 
        }else if(angular <= 202.5 && angular > 157.5){
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.SE); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.E); 
        }else if(angular <= 247.5 && angular > 202.5){
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SE); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.E); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.NE); 
        }else if(angular <= 292.5 && angular > 247.5){
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.E); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.N); 
        }else if(angular <= 337.5 && angular > 292.5){
                orientations.add(Orientation.SE); 
                orientations.add(Orientation.E); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.NW); 
        }
        return orientations;
    }
     /**
 *
 * @author Raúl 
 */     
    //INDICA SI ES POSIBLE PASAR POR AHI, SI LA ALTURA DEL TERRENO SUPERA LA MAXIMA DE VUELO
    public boolean isPossibleWay(int alt){
        boolean sePuede = true;
        
        if(alt > maxFlight){
            sePuede = false;
        }
        return sePuede;
    }
    
    /**
 *
 * @author Raúl 
 */
    public boolean outOfBoundaries(Square c){
        Info("Casilla x: "+c.x+" y: "+c.y);
        Info("Ancho: "+width+"Alto: "+height);
        if(c.x >= width || c.y >= height || c.x < 0 || c.y < 0){
            return true;
        }
        
        return false;
    }
    
    /**
 *
 * @author Raúl 
 */
    public Square getSquare(Orientation or){
        double [] gps = myDashboard.getGPS();
        Square c = new Square();
        switch (or) {
            case E:
                c.x = (int)gps[0] + 1;
                c.y = (int)gps[1];
                break;
            case N:
                c.x = (int)gps[0];
                c.y = (int)gps[1] - 1;
                break;
            case S:
                c.x = (int)gps[0];
                c.y = (int)gps[1] + 1;
                break;
            case W:
                c.x = (int)gps[0] - 1;
                c.y = (int)gps[1];
                break;    
            case NE:
                c.x = (int)gps[0] + 1;
                c.y = (int)gps[1] - 1;
                break;
            case NW:
                c.x = (int)gps[0] - 1;
                c.y = (int)gps[1] - 1;
                break;
            case SE:
                c.x = (int)gps[0] + 1;
                c.y = (int)gps[1] + 1;
                break;
            case SW:
                c.x = (int)gps[0] - 1;
                c.y = (int)gps[1] + 1;
                break;
            default:
                break;
        }
        return c;
    }
    
    /**
 *
 * @author Raúl 
 */
    public boolean isSquareInMemory(Square c){
        for(int i = 0; i < memory.size();i++){
            //Info("casilla memoria: x= "+memoria.get(i).x+" y="+memoria.get(i).y);
            if(c.x == memory.get(i).x && c.y == memory.get(i).y){
                return true;
            }
        }
        
        return false;
    }
    
    /**
 *
 * @author Raúl 
 */
    //ELEGIMOS LA PROXIMA CASILLA POR PREFERENCIA QUE SEA POSIBLE PASAR 
    public void chooseNextSquare(){
        ArrayList<Integer> cp = getNearHeightsLeftWall();
        ArrayList<Orientation> op = getOrientationsLeftWall();
        
        int indice = 0;
        boolean enc = false;
        Info("MaxFlight: "+this.maxFlight);
        Info("Altura TieF: "+this.myDashboard.getGPS()[2]);
        for(int i = 0; i < cp.size() && !enc; i++){
            Info("Altura: "+cp.get(i));
            Info("Posible: "+isPossibleWay( cp.get(i) ));
            if( isPossibleWay( cp.get(i) ) && !this.outOfBoundaries(getSquare(op.get(i)))){
                if(!isSquareInMemory(getSquare(op.get(i)))) {
                    indice = i;
                    enc = true;
                }
                else{
                    indice = i;
                }
            }
        }
        nextSquare = op.get(indice);
    }
    
    /**
 *
 * @author Raúl 
 */
    public void addSquareToMemory(){
        Square c = new Square();
        c.x = (int)myDashboard.getGPS()[0];
        c.y = (int)myDashboard.getGPS()[1];
        memory.add(c);
    }
    
    /**
 *
 * @author Raúl 
 */
    //ALINEAMOS AL TIE FIGHTER CON LA PROXIMA CASILLA
    public void heuristic(){
        int compass = (int)myDashboard.getCompass();
        
        this.chooseNextSquare();
        Info("Angulo para ir a la siguiente casilla: "+getAngle(nextSquare));
        Info("A donde apunta el tie Fighter:"+compass);
        if(compass != getAngle(nextSquare))
            rotate(getAngle(nextSquare),compass);
        Info("lidar prox casilla: "+ getLidar(nextSquare));
        if(getLidar(nextSquare) < 0 && (int)this.myDashboard.getGPS()[2] < maxFlight){
            int upTimes = -(getLidar(nextSquare)/5); 
            for(int i = 0; i < upTimes;i++)
                this.sendAction("UP");
        }
        
        this.addSquareToMemory();
        
        this.sendAction("MOVE");
    }
    /**
 *
 * @author Pablo e Ignacio
 */
    //POSIBLE RETOQUE
    public void rotate(int angleTarget, int actualAngle){
        if(angleTarget > 180)
            angleTarget -= 360;
        else if(angleTarget < -180)
            angleTarget += 360;
        if(angleTarget < 0 || (angleTarget == 0 && actualAngle <= 180)){
            //A la derecha
            while(actualAngle != angleTarget) {
                Info("Target = "+angleTarget);
                Info("Actual = "+actualAngle);
                this.sendAction("RIGHT"); // REVISAR
                actualAngle -= 45.0;
            }
        }else if(angleTarget > 0 || (angleTarget == 0 && actualAngle > 180)){
            //A la izquierda
            while(actualAngle != angleTarget){
                Info("Target = "+angleTarget);
                Info("Actual = "+actualAngle);
                this.sendAction("LEFT"); // REVISAR
                actualAngle = (int) (actualAngle + 45.0) % 360;
            }
        }
    }
    
    /**
 *
 * @author Pablo e Ignacio 
 */
    public boolean sendAction(String action) {
        session = session.createReply();
        session.setContent("Request execute "+ action +" session "+sessionKey);
        this.LARVAsend(session);
        
        session = this.LARVAblockingReceive();
        if(session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure")) {
            Alert("Execute action failure due to "+session.getContent());
            return false;
        }
        return true;
    }
    
    
    
    /**
 *
 * @author Raúl e Ignacio
 */
    public Status MySolveProblem() {
        
        if(!this.updateSensors())
            return Status.CLOSEPROBLEM;
        
        while(!resuelto && myDashboard.getEnergy() > 0 && myDashboard.getAlive()){
            if(this.updateSensors()){
                considerRecharge();
                if(!goRecharge){
                    chooseAction();
                }else{
                    goToRecharge();
                }
            }else
                return Status.CLOSEPROBLEM;
        }
        this.updateSensors();
        return Status.CLOSEPROBLEM;
        
    }
    
    /**
 *
 * @author Raúl 
 */
    public ArrayList<Integer> getNearHeightsLeftWall(){
        ArrayList<Integer> heights = new ArrayList<Integer>();
        double angular = myDashboard.getAngular();
        if(angular <= 22.5 || angular > 337.5 && myDashboard.getLidar()[6][5] > -this.maxFlight){
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
        }else if(angular <= 67.5 && angular > 22.5 && myDashboard.getLidar()[6][5] > -this.maxFlight){
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
        }else if(angular <= 112.5 && angular > 67.5){
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
        }else if(angular <= 157.5 && angular > 112.5){
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE)));  
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
        }else if(angular <= 202.5 && angular > 157.5){
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE)));  
                heights.add(getHeightOfLand(getLidar(Orientation.N)));  
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
        }else if(angular <= 247.5 && angular > 202.5){
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
        }else if(angular <= 292.5 && angular > 247.5){
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
        }else if(angular <= 337.5 && angular > 292.5){
                heights.add(getHeightOfLand(getLidar(Orientation.SE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.E))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NE))); 
                heights.add(getHeightOfLand(getLidar(Orientation.N))); 
                heights.add(getHeightOfLand(getLidar(Orientation.NW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.W))); 
                heights.add(getHeightOfLand(getLidar(Orientation.SW))); 
                heights.add(getHeightOfLand(getLidar(Orientation.S))); 
        }
        return heights;
    }
    
    /**
 *
 * @author Raúl 
 */
    public ArrayList<Orientation> getOrientationsLeftWall(){
        ArrayList<Orientation> orientations = new ArrayList<Orientation>();
        int angular = (int)myDashboard.getAngular();
        
        if( (angular <= 22.5 && angular >= 0)  || (angular > 337.5 && angular <= 360)){
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE); 
        }else if(angular <= 67.5 && angular > 22.5){
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE);
                orientations.add(Orientation.E); 
        }else if(angular <= 112.5 && angular > 67.5){
                orientations.add(Orientation.N); 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE);
                orientations.add(Orientation.E);
                orientations.add(Orientation.NE); 
        }else if(angular <= 157.5 && angular > 112.5){ 
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE);
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N);
        }else if(angular <= 202.5 && angular > 157.5){ 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE);
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N);
                orientations.add(Orientation.NW);
        }else if(angular <= 247.5 && angular > 202.5){
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE);
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N);
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
        }else if(angular <= 292.5 && angular > 247.5){
                orientations.add(Orientation.S); 
                orientations.add(Orientation.SE);
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N);
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW);
        }else if(angular <= 337.5 && angular > 292.5){ 
                orientations.add(Orientation.SE);
                orientations.add(Orientation.E); 
                orientations.add(Orientation.NE); 
                orientations.add(Orientation.N);
                orientations.add(Orientation.NW); 
                orientations.add(Orientation.W); 
                orientations.add(Orientation.SW); 
                orientations.add(Orientation.S);
        }
        return orientations;
    }
}
