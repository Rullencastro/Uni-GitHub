package tiefighter;

import agents.LARVAFirstAgent;
import appboot.LARVABoot;
import jade.core.AID;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import java.util.ArrayList;
import swing.LARVADash;

/**
 * @author Pablo Rodriguez
 * @author Alejandro Cuenca
 */
public class Corellian extends LARVAFirstAgent{
    enum Status {
        CHECKIN, CHECKOUT,COMISSIONING,JOINSESSION, SOLVEPROBLEM, WAIT, EXIT
    }
    Status mystatus;
    String service = "PManager", 
           problem = "Ando",
           password = "105Wing",
           problemManager = "",
           mymap,
           content, 
           sessionKey, 
           sessionManager,
           storeManager,
           sensorKeys,
           nextAction;
    
    int width, height, maxFlight, maxTerrain = 0, minTerrain = 214748364, altitude;
    
    // Sensors Simulated and Real
    Square gps = new Square();
    int myCompass = 0,myAngular = 0;
    double energy = 3500;
    Orientation myOrientation;
    
    String[] contentTokens,coords,mySensors = new String[]{
  
    };
    
    
    Square goal = new Square();
    int posAngularAct = 0;
    
        // Comunication variable
    ACLMessage open, session, destroyer;
    AID idSession, idDestroyer;
    MessageTemplate tempSession,tempDest;
    
    // Flags
    boolean step=true,goRecharge = false, isInGoal = false, alive = true;
    int nMove = 0;
    
    /**
     * @author Pablo Rodriguez
    */
    @Override
    public void setup() {
        super.setup();
        logger.onOverwrite();
        logger.setLoggerFileName("mylog.json");
        Info("Setup and configure agent");
        mystatus = Status.CHECKIN;
        exit = false;
        this.myDashboard = new LARVADash(this);
    }
    
    /**
     * @author Pablo Rodriguez
     * @author Alejandro Cuenca    
    */
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
            case WAIT:
                mystatus = MyWait();
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
            case CHECKOUT:
                mystatus = MyCheckout();
                break;
            case EXIT:
            default:
                exit = true;
                break;
        }
    }
    
    /**
     * @author Pablo Rodriguez
    */
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
        this.DFSetMyServices(new String[]{"CORELLIAN " + password});
        return Status.WAIT;
    }
    
    /**
     * @author Pablo Rodriguez
    */
    public Status MyWait() {
        this.destroyer = this.LARVAblockingReceive();
        sessionKey = destroyer.getConversationId();

        if(destroyer.getPerformative() == ACLMessage.CFP){
            idDestroyer = destroyer.getSender();
            destroyer = destroyer.createReply();
            destroyer.setPerformative(ACLMessage.AGREE);
            destroyer.setOntology("COMMITMENT");
            destroyer.setInReplyTo("RECRUITMENT CORELLIAN");
            destroyer.setContent("");
            this.LARVAsend(destroyer);
        }
        
        tempDest = MessageTemplate.MatchSender(idDestroyer);
        destroyer = this.LARVAblockingReceive(tempDest);
        if(destroyer.getPerformative() == ACLMessage.ACCEPT_PROPOSAL){
            coords = destroyer.getContent().split(" ");
            gps.setXY(Integer.parseInt(coords[0]), Integer.parseInt(coords[1]));
            goal.setXY(Integer.parseInt(coords[0]), Integer.parseInt(coords[1]));
            myCompass = 0;
            altitude = 0;
            return Status.COMISSIONING;
        }else if(destroyer.getPerformative() == ACLMessage.REJECT_PROPOSAL)
            return Status.CHECKOUT;
        
        return Status.CHECKOUT;
    }
    
    /**
     * @author Pablo Rodriguez
    */
     public Status MyComissioning() {
        String localService="STORE "+sessionKey;
        if (this.DFGetAllProvidersOf(localService).isEmpty()){
            Error("Service STORE is down");
            return Status.EXIT;
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
                return Status.EXIT;
            }
        }
        Info("Bought all sensor keys "+sensorKeys);
        return Status.JOINSESSION;
    }
    
    /**
     * @author Pablo Rodriguez
    */
    public Status MyJoinSession() {
        String localService="SESSION MANAGER "+sessionKey;
        if (this.DFGetAllProvidersOf(localService).isEmpty()){
            Error("Service SESSION MANAGER is down");
            return Status.CHECKOUT;
        }
        sessionManager = this.DFGetAllProvidersOf(localService).get(0);
        
        session = new ACLMessage();
        session.setContent("REQUEST JOIN SESSION "+sessionKey+" AS CORELLIAN AT "+gps.x+" "+gps.y+" ATTACH SENSORS "+sensorKeys);
        session.setPerformative(ACLMessage.REQUEST);
        session.setSender(this.getAID());
        idSession = new AID(sessionManager, AID.ISLOCALNAME);
        session.addReceiver(idSession);
        session.setConversationId(sessionKey);
        this.LARVAsend(session);
        
        this.tempSession = MessageTemplate.MatchSender(idSession);
        session = this.LARVAblockingReceive(tempSession);
        String parse[] = session.getContent().split(" ");
        if(parse[0].equals("Confirm")){
            width = Integer.parseInt(parse[8]);
            height = Integer.parseInt(parse[10]);
            maxFlight = Integer.parseInt(parse[14]);
            gps.setZ(myDashboard.getMapLevel(gps.x, gps.y));
            
            destroyer = destroyer.createReply();
            destroyer.setPerformative(ACLMessage.INFORM);
            destroyer.setContent(gps.x+" "+gps.y);
            destroyer.setInReplyTo("TAKEOFF");
            this.LARVAsend(destroyer);
            return Status.SOLVEPROBLEM;
        }
        else{
            Alert("Error joining session: "+session.getContent());
            return Status.CHECKOUT;
        } 
    }
    
    /**
     * @author Pablo Rodriguez
     * @author Alejandro Cuenca
     * @author Raul Castro Moreno
    */
    public Status MySolveProblem() {
        Info("Corellian inicia el SolveProblem");        
        this.obtainMaxTerrain();
        
        captureJedi();       
        destroyer = this.LARVAblockingReceive(tempDest);
        while(destroyer.getPerformative() != ACLMessage.CANCEL && alive){
            if(destroyer.getPerformative() == ACLMessage.REQUEST){
                String parse[] = destroyer.getContent().split(" ");
                goal.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
            }
            
            if(gps.x == goal.x && gps.y == goal.y) {
                isInGoal = true;
            }
        
            this.simulateAngular();
            if(myCompass != myAngular)
                alive = this.rotate();

            while(!isInGoal && alive){
                this.getInfo();
                      
                this.move();
                
                if( gps.x == goal.x || gps.y == goal.y) {
                    this.simulateAngular();
                    if(myCompass != myAngular)
                        alive = this.rotate();
                }
                
            }
            
            if(!alive)
                return Status.CHECKOUT;
            if(isInGoal){
            captureJedi();
            isInGoal = false;
            }
            
            destroyer = this.LARVAblockingReceive(tempDest);
            
            this.Info("Corellian captura a un jedi");
        }
        
        return Status.CHECKOUT; 
    }
    
    /**
     * @author Pablo Rodriguez
    */
    public Status MyCheckout() {
        this.doLARVACheckout();
        return Status.EXIT;
    }
    
    /**
     * @author Pablo Rodriguez
    */
    public boolean sendAction(String action) {
        session = session.createReply();
        session.setContent("Request execute "+ action +" session "+sessionKey);
        session.setPerformative(ACLMessage.REQUEST);
        this.LARVAsend(session);
        
        session = this.LARVAblockingReceive(tempSession);
        if(session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure")) {
            Alert("Execute action failure due to "+session.getContent());
            return false;
        }
        return true;
    }
 
    /**
     * @author Alejandro Cuenca
     */
    public boolean updateSensors(){
        session = session.createReply();
        session.setContent("Query sensors session "+sessionKey);
        session.setPerformative(ACLMessage.QUERY_REF);
        this.LARVAsend(session);
        session = this.LARVAblockingReceive();
            if (session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure")){
                Alert("Reading of sensors failed due to "+session.getContent());
                return false;
            }
        return true;
    }

    /**
     * @author Alejandro Cuenca
     */
    public void obtainMaxTerrain(){
        int minW = 0,maxW = width/2;
        
        if(gps.x >= width/2){
            minW = width/2;
            maxW = width;
        }
        for(int i = minW; i < maxW;i++){
            for(int j = 0; j < height;j++){
                if( myDashboard.getMapLevel(i, j) > this.maxTerrain)
                  this.maxTerrain = myDashboard.getMapLevel(i, j);
            }
        }
    }
    
    /**
     * @author Alejandro Cuenca
     */
    public void obtainMinTerrain(){
        int minW = 0,maxW = width/2;
        
        if(gps.x >= width/2){
            minW = width/2;
            maxW = width;
        }
        for(int i = minW; i < maxW;i++){
            for(int j = 0; j < height;j++){
                if( myDashboard.getMapLevel(i, j) < this.minTerrain)
                  this.minTerrain = myDashboard.getMapLevel(i, j);
            }
        }
        this.minTerrain += 1;
    }
        
    /**
     * @author Alejandro Cuenca
     * @author Raul Castro Moreno
     */
    public void simulateAngular(){
        if(gps.x == goal.x){
            if(gps.y < goal.y)
                myAngular = 270;
            else if(gps.y > goal.y)
                myAngular = 90;
        }
        else if(gps.y == goal.y){
            if(gps.x < goal.x)
                myAngular = 0;
            else if(gps.x > goal.x)
                myAngular = 180;
        }
        else{
            if(gps.x < goal.x && gps.y > goal.y)
                myAngular = 45;
            else if(gps.x > goal.x && gps.y > goal.y)
                myAngular = 135;
            else if(gps.x > goal.x && gps.y < goal.y)
                myAngular = 225;
            else if(gps.x < goal.x && gps.y < goal.y)
                myAngular = 315;
        }
    }
    
    /**
     * @author Alejandro Cuenca
     */
    public void getInfo() {
        Info("Alive: "+alive);
        Info("ANGULAR: "+myAngular);
        Info("Goal: "+isInGoal);
        Info("Energy: "+energy);
        Info("My GPS: x="+gps.x+" y="+gps.y);
        Info("My Goal: x="+goal.x+" y="+goal.y);
    }
    
    /**
     * @author Alejandro Cuenca
     * @author Raul Castro Moreno
     */
    public boolean rotate() {
        boolean isAlive = true;
        boolean left = true;
        
        int angleRotation = myAngular - myCompass;
        if(angleRotation < 0){
            angleRotation *= -1;
            left = false;
        }
        if(angleRotation > 180){
            angleRotation = 360 - angleRotation;
            left = !left;
        }
        
        Info("Rotation of: "+angleRotation+" º");
        
        if(left) {
            while(angleRotation != 0) {
                Info("Rotate to LEFT");
                isAlive = this.sendAction("LEFT");
                this.updateEnergy();
                angleRotation -= 45;
                myCompass = (myCompass + 45) % 360;
            }
        } 
        else { 
            while(angleRotation != 0) {
                Info("Rotate to RIGHT");
                isAlive = this.sendAction("RIGHT");
                this.updateEnergy();
                angleRotation -= 45;
                myCompass = myCompass - 45;
                if(myCompass < 0)
                    myCompass += 360;
            }
        }
        
        return isAlive;
    }
    
    /**
     * @author Alejandro Cuenca
     */    
    public Orientation convertOrientation(int angle) {
        Orientation result = Orientation.E;
        if( (angle == 0)  || (angle == 360)){
            result = Orientation.E;
        }else if(angle == 45){
            result = Orientation.NE; 
        }else if(angle == 90){
            result = Orientation.N;
        }else if(angle == 135){ 
            result = Orientation.NW;
        }else if(angle == 180){ 
            result = Orientation.W; 
        }else if(angle == 225){
            result = Orientation.SW; 
        }else if(angle == 270){
            result = Orientation.S; 
        }else if(angle == 315){ 
            result = Orientation.SE;
        }
        
        return result;
    }
    
    /**
     * @author Alejandro Cuenca
     * @author Pablo Rodríguez
     * @author Raul Castro Moreno
     */
    public void updateEnergy() {
        energy -= 10;
        this.obtainMaxTerrain();
        double movsToRecharge = (gps.z-this.myDashboard.getMapLevel(gps.x, gps.y))/5;
        if (energy <= (movsToRecharge*10) + 10) {
            this.recharge();
        }
    }
    
    /**
     * @author Pablo Rodriguez
     */
    public void move() { 
        myOrientation = convertOrientation(myCompass);
        // Update GPS simulation
        switch(myOrientation){
            case S:
                gps.y++;
                break;
            case N:
                gps.y--;
                break;
            case E:
                gps.x++;
                break;
            case W:
                gps.x--;
                break;
            case SE:
                gps.y++;
                gps.x++;
                break;
            case SW:
                gps.y++;
                gps.x--;
                break;
            case NE:
                gps.y--;
                gps.x++;
                break;
            case NW:
                gps.y--;
                gps.x--;
                break;
        }
        
        updateHeightIfNeeded();
        alive = this.sendAction("MOVE");
        if(!alive)
            takeDown();
        
        this.updateEnergy();
        
        if(gps.x == goal.x && gps.y == goal.y) {
            isInGoal = true;
        }     
    }
    
    /**
     * @author Pablo Rodriguez
     * @author Alejandro Cuenca
     * @author Raul Castro Moreno
     */
    public void updateHeightIfNeeded() {
        int levelNextSquare = this.myDashboard.getMapLevel(gps.x, gps.y);
        int currentLevel = gps.z;
        this.Info(currentLevel + " Current");
        this.Info(levelNextSquare + " Next");
        if(currentLevel < levelNextSquare && currentLevel < this.maxTerrain){
            int upTimes = ((levelNextSquare - currentLevel ) / 5); 
            for(int i = 0; i < upTimes;i++){
                alive = this.sendAction("UP");
                if(!alive){
                    takeDown();
                }
                this.updateEnergy();
                
            }
                
            gps.setZ(levelNextSquare);
        }
    }
    /**
     * @author Pablo Rodriguez
     * @author Raul Castro Moreno
     */
    public void recharge() {
        destroyer = destroyer.createReply();
        destroyer.setPerformative(ACLMessage.QUERY_IF);
        destroyer.setContent("RECHARGE");
        this.LARVAsend(destroyer);
        Info("Corellian esperando recarga");
        destroyer = this.LARVAblockingReceive(tempDest);
        Info("Corellian termina la espera");
            
        if(destroyer.getPerformative() == ACLMessage.DISCONFIRM) {
            alive = false;
                
        }else if(destroyer.getPerformative() == ACLMessage.CONFIRM){     
            goToFloor();
            if(this.sendAction("RECHARGE")){
                energy = 3500.0;
            }
            else{
                alive = false;
            }    
        }
        
    }
    
    /**
     * @author Pablo Rodriguez
     */
    public void captureJedi() {
        getInfo();
        goToFloor();
        this.sendAction("CAPTURE");
        destroyer = new ACLMessage();
        destroyer.setSender(this.getAID());
        destroyer.addReceiver(idDestroyer);
        destroyer.setConversationId(this.sessionKey);
        destroyer.setPerformative(ACLMessage.INFORM);
        destroyer.setOntology("COMMITMENT");
        destroyer.setContent("CAPTURE "+goal.x+" "+goal.y);
        destroyer.setInReplyTo("CAPTURE "+goal.x+" "+goal.y);
        this.LARVAsend(destroyer);
    }
    
    /**
     * @author Pablo Rodriguez
     * @author Raul Castro Moreno
     */
    public void goToFloor() {
        int floorLevel = this.myDashboard.getMapLevel(gps.x, gps.y);
        int downTimes = ((gps.z - floorLevel ) / 5); 
        for(int i = 0; i < downTimes;i++){
            this.sendAction("DOWN");
            this.energy-=10;
        }
        gps.setZ(floorLevel);
    }
    
    /**
     * @author Pablo Rodriguez
     */
    @Override
    public void takeDown() {
        if (this.AMSIsConnected(this.sessionManager) && this.isCheckedin() ) {
            this.MyCheckout();
        }
        Info("Taking down and deleting agent");
        this.saveSequenceDiagram("./" + this.problem + ".seqd");
        super.takeDown();
    }
}
