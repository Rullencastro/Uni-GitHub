package tiefighter;

import agents.LARVAFirstAgent;
import jade.core.AID;
import jade.lang.acl.ACLMessage;
import static java.lang.Math.*;
import swing.LARVADash;
import java.util.ArrayList;
import geometry.Point;
import jade.lang.acl.MessageTemplate;

public class Fighter extends LARVAFirstAgent{
    enum Status {
        CHECKIN, CHECKOUT,WAIT,OPENPROBLEM,COMISSIONING,JOINSESSION, SOLVEPROBLEM, CLOSEPROBLEM, EXIT
    }
    Status mystatus;
    String service = "PManager", 
           problem = "Fondor",
           password = "105Wing",
           problemManager = "", 
           content, 
           sessionKey, 
           sessionManager,
           storeManager,
           sensorKeys,
           nextAction;
    // map Variable
    int width, height, maxFlight, maxTerrain = 0, altitude;
    
    // Sensors Simulated and Real
    Square gps = new Square();
    int myCompass = 0,myAngular = 0;
    double energy = 3500;
    Orientation myOrientation;
    String[] contentTokens,coords,mySensors = new String[]{
        "THERMALHQ",
    };
    
    // Jedis and goals
    Jedi newJedi = new Jedi();
    ArrayList <Jedi> jedisLocations = new ArrayList<>();
    Square goal = new Square();
    int posAngularAct = 0;
    
    // Comunication variable
    ACLMessage open, session, destroyer;
    AID idSession, idDestroyer;
    MessageTemplate tempSession,tempDest;
    
    // Flags
    boolean step=true, updateThermal = true,goRecharge = false, isInGoal = false, alive = true;
    int nMove = 0;
    
    @Override
    public void setup() {
        super.setup();
        logger.onOverwrite();
        logger.setLoggerFileName("mylog.json");
        //this.enableDeepLARVAMonitoring();
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
        this.DFSetMyServices(new String[]{"FIGHTER " + password});
        return Status.WAIT;
    }
    
    
    /**
     * This method wait the agent to receive destroyer messages
     * @author Raul Castro
     */
    public Status MyWait() {
        
        this.destroyer = this.LARVAblockingReceive();
        sessionKey = destroyer.getConversationId();
        
        if(destroyer.getPerformative() == ACLMessage.CFP){
            idDestroyer = destroyer.getSender();
            destroyer = destroyer.createReply();
            destroyer.setPerformative(ACLMessage.AGREE);
            destroyer.setOntology("COMMITMENT");
            //ENCUENTRAMIENTO MODIFICAR IMPORTANTE
            destroyer.setInReplyTo("RECRUITMENT FIGHTER");
            destroyer.setContent("");
            this.LARVAsend(destroyer);
        }
        
        tempDest = MessageTemplate.MatchSender(idDestroyer);
        
        destroyer = this.LARVAblockingReceive(tempDest);
        
        if(destroyer.getPerformative() == ACLMessage.ACCEPT_PROPOSAL){
            coords = destroyer.getContent().split(" ");
            gps.setXY(Integer.parseInt(coords[0]), Integer.parseInt(coords[1]));
            myCompass = 0;
            altitude = 0;
            return Status.COMISSIONING;
        }else if(destroyer.getPerformative() == ACLMessage.REJECT_PROPOSAL)
            return Status.CHECKOUT;
        
        return Status.CHECKOUT;
    }

    public Status MyCheckout() {
        this.doLARVACheckout();
        return Status.EXIT;
    }
    
    public Status MyComissioning() {
        Info(""+sessionKey);
        String localService="STORE "+sessionKey;
        if (this.DFGetAllProvidersOf(localService).isEmpty()){
            Error("Service STORE is down");
            return Status.CHECKOUT;
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
                return Status.CHECKOUT;
            }
        }
        Info("Bought all sensor keys "+sensorKeys);
        return Status.JOINSESSION;
    }
    
    public Status MyJoinSession() { 
        
        String localService="SESSION MANAGER "+sessionKey;
        if (this.DFGetAllProvidersOf(localService).isEmpty()){
            Error("Service SESSION MANAGER is down");
            return Status.CHECKOUT;
        }
        sessionManager = this.DFGetAllProvidersOf(localService).get(0);
        
        session = new ACLMessage();
        session.setContent("REQUEST JOIN SESSION "+sessionKey+" AS FIGHTER AT "+gps.x+" "+gps.y+" ATTACH SENSORS "+sensorKeys);
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
            gps.setZ(this.myDashboard.getMapLevel(gps.x, gps.y));
            
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
     * This method solve the problem 
     * @author Hamed Ignacio Benaisa Cruz
     * @author Raul Castro Moreno
     * @return Next Status of Fighter
     */
    public Status MySolveProblem() {
        // Elevate Fighter
        this.obtainMaxTerrain();
        this.upAltitude();
        
        // Wait for Destroyer instructions
        destroyer = this.LARVAblockingReceive(tempDest);
        while(destroyer.getPerformative() != ACLMessage.CANCEL && alive){
            // Get Goal
            if(destroyer.getPerformative() == ACLMessage.REQUEST){
                String parse[] = destroyer.getContent().split(" ");
                goal.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
            }
        
            // Set angle of Fighter
            this.simulateAngular();
            if(myCompass != myAngular)
                alive = this.rotate();

            // Pathing
            while(!isInGoal && alive){
                // Show sensors; optional
                this.getInfo();
                
                // Look for Jedis
                if(updateThermal) {
                    searchForJedis();
                    updateThermal = false;
                }
                
                // Move
                this.move();
                
                // Set angle of Fighter
                if( gps.x == goal.x || gps.y == goal.y) {
                    this.simulateAngular();
                    if(myCompass != myAngular)
                        alive = this.rotate();
                }
                    
            }
            isInGoal = false;
            nMove = 0;
            searchForJedis();
            
            destroyer = destroyer.createReply();
            destroyer.setPerformative(ACLMessage.INFORM);
            destroyer.setContent("MOVE "+goal.x+" "+goal.y+" "+maxTerrain);
            this.LARVAsend(destroyer);
            destroyer = this.LARVAblockingReceive(tempDest);
        }
             
        return Status.CHECKOUT; 
    }
    
    public boolean sendAction(String action) {
        session = session.createReply();
        session.setContent("Request execute "+ action +" session "+sessionKey);
        session.setPerformative(ACLMessage.REQUEST);
        this.LARVAsend(session);
        
        session = this.LARVAblockingReceive(tempSession);
        
        if(session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure") || session.getContent().contains("Unsuccessful")) {
            Alert("Execute action failure due to "+session.getContent());
            return false;
        }
        return true;
    }
    
    public boolean updateSensors(){
        this.updateEnergy();
        session = session.createReply();
        session.setContent("Query sensors session "+sessionKey);
        session.setPerformative(ACLMessage.QUERY_REF);
        this.LARVAsend(session);
        session = this.LARVAblockingReceive(tempSession);
            if (session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure")){
                Alert("Reading of sensors failed due to "+session.getContent());
                return false;
            }
        return true;
    }
    
    /**
     * This method get the maximun altitude of terrain in the world
     * @author Raul Castro
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
        this.maxTerrain += 5;
    }
    
    /**
     * This method simulate the Angular sensor
     * @author Raul Castro
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
     * This method setup discover the world and send
     * the jedi position to Destroyer
     * @author Hamed Ignacio Benaisa Cruz
     * @author Raúl Castro Moreno
     * @return A boolean indicate if Fighter is alive
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
     * This method move TieFighter out of the thermal registered
     * @author Hamed Ignacio Benaisa Cruz
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
        alive = this.sendAction("MOVE");
        this.updateEnergy();
        nMove++;
        if(nMove == 20){
            updateThermal = true;
            nMove = 0;
        }
        
        if(gps.x == goal.x && gps.y == goal.y) {
            isInGoal = true;
            updateThermal = true;
        }
            
    }
    
    /**
     * This method convert Double to Orientation
     * @author Hamed Ignacio Benaisa Cruz
     * @param angle The double is going to convert
     * @return The Orientation that corresponds to the Double value
     */
    public Orientation convertOrientation(double angle) {
        Orientation result = Orientation.E;
        if( (angle <= 22.5 && angle >= 0)  || (angle > 337.5 && angle <= 360)){
            result = Orientation.E;
        }else if(angle <= 67.5 && angle > 22.5){
            result = Orientation.NE; 
        }else if(angle <= 112.5 && angle > 67.5){
            result = Orientation.N;
        }else if(angle <= 157.5 && angle > 112.5){ 
            result = Orientation.NW;
        }else if(angle <= 202.5 && angle > 157.5){ 
            result = Orientation.W; 
        }else if(angle <= 247.5 && angle > 202.5){
            result = Orientation.SW; 
        }else if(angle <= 292.5 && angle > 247.5){
            result = Orientation.S; 
        }else if(angle <= 337.5 && angle > 292.5){ 
            result = Orientation.SE;
        }
        
        return result;
    }
    
    /**
     * This method update the sensors and save the position of Jedis
     * @author Hamed Ignacio Benaisa Cruz
     */
    public void searchForJedis() {
        Info("Searching jedis");
        
        for(int z=0; z<8 && alive; z++) {
            alive = this.sendAction("LEFT");
            this.updateEnergy();
            myCompass = (myCompass + 45) % 360;
            myOrientation = convertOrientation(myCompass);
            this.updateSensors();
            // Looking sectors in thermal Array
            switch(myOrientation) {
                case NW:
                    for(int i=0; i<10 && alive; i++) {
                        for(int j=0; j<10; j++) {
                            if(myDashboard.getThermal()[i][j] == 0) {
                                newJedi = new Jedi();
                                newJedi.setPos(gps.x-(10-j), gps.y-(10-i));
                                this.sendJedisInfo(newJedi);
                            }
                        }
                    }
                break;
                case NE:
                    for(int i=0; i<10 && alive; i++) {
                        for(int j=10; j<20; j++) {
                            if(myDashboard.getThermal()[i][j] == 0) {
                                newJedi = new Jedi();
                                newJedi.setPos(gps.x-(10-j), gps.y-(10-i));
                                this.sendJedisInfo(newJedi);
                            }
                        }
                    }
                break;
                case SE:
                    for(int i=10; i<20 && alive; i++) {
                        for(int j=10; j<20 && alive; j++) {
                            if(myDashboard.getThermal()[i][j] == 0) {
                                newJedi = new Jedi();
                                newJedi.setPos(gps.x-(10-j), gps.y-(10-i));
                                this.sendJedisInfo(newJedi);
                            }
                        }
                    }
                break;
                case SW:
                    for(int i=10; i<20 && alive; i++) {
                        for(int j=0; j<10; j++) {
                            if(myDashboard.getThermal()[i][j] == 0) {
                                newJedi = new Jedi();
                                newJedi.setPos(gps.x-(10-j), gps.y-(10-i));
                                this.sendJedisInfo(newJedi);
                            }
                        }
                    }
                break;
            }
        }
        
    }

    /**
     * This method send to Destroyer all Jedis Position
     * @author Hamed Ignacio Benaisa Cruz
     * @param jediLocated The new Jedi Location
     */
    public void sendJedisInfo(Jedi jediLocated) {
        boolean yaLocalizado = false;
        for(Jedi j : jedisLocations) {
            if( (j.x == jediLocated.x && j.y == jediLocated.y) ) {
                yaLocalizado = true;
            }
        }
        
        if( !yaLocalizado) {
            jedisLocations.add(jediLocated);
            Info("Jedi in: x="+jediLocated.x+" y="+jediLocated.y);
            outbox = new ACLMessage();
            outbox.setSender(this.getAID());
            outbox.addReceiver(idDestroyer);
            outbox.setConversationId(this.sessionKey);
            outbox.setOntology("COMMITMENT");
            outbox.setPerformative(ACLMessage.INFORM_REF);
            outbox.setContent("FOUND "+jediLocated.x+" "+jediLocated.y);
            this.LARVAsend(outbox);
        }
        
    }
    
    /**
     * This method print all variables of the Fighter
     * @author Hamed Ignacio Benaisa Cruz
     */
    public void getInfo() {
        Info("Alive: "+alive);
        Info("Goal: "+isInGoal);
        Info("Energy: "+energy);
        Info("My GPS: x="+gps.x+" y="+gps.y);
        Info("My Goal: x="+goal.x+" y="+goal.y);
    }

    /**
     * This method send request to Destroyer for Recharge
     * @author Hamed Ignacio Benaisa Cruz
     * @author Raúl Castro Moreno
     */
    public void recharge() {
        
            destroyer = destroyer.createReply();
            destroyer.setPerformative(ACLMessage.QUERY_IF);
            destroyer.setContent("RECHARGE");
            this.LARVAsend(destroyer);
            Info("Me pongo a esperar");
            destroyer = this.LARVAblockingReceive(tempDest);
            Info("Dejo de esperar");
            
            if(destroyer.getPerformative() == ACLMessage.DISCONFIRM) {
                alive = false;
            } else if(destroyer.getPerformative() == ACLMessage.CONFIRM){
                int actualMapLevel = this.myDashboard.getMapLevel(gps.x, gps.y);
                while(gps.z != actualMapLevel){
                    this.sendAction("DOWN");
                    gps.setZ(gps.z-5);
                }
                    
                if(this.sendAction("RECHARGE")){
                    energy = 3500.0;
                    this.upAltitude();
                }else{
                    alive = false;
                }
                
                
            }
        
    }
    
    /**
     * This method update energy sensor and consider Recharge
     * @author Hamed Ignacio Benaisa Cruz
     */
    public void updateEnergy() {
        energy--;
        double movsToRecharge = (gps.z-this.myDashboard.getMapLevel(gps.x, gps.y))/5;
        if (energy <= movsToRecharge + 10) {
            this.recharge();
        }
    }
    
    /**
     * Sets the flight height according to the standard
     * @author Hamed Ignacio Benaisa Cruz
     */
    public void upAltitude() {
        int upTimes = (this.maxTerrain - this.myDashboard.getMapLevel(gps.x, gps.y))/5;
        for(int i = 0; i < upTimes && alive; i++) {
            alive = this.sendAction("UP");
            this.updateEnergy();
            gps.setZ(gps.z+5);
        }
    }
}
