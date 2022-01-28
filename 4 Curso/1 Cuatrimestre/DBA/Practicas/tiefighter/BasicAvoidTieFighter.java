package tiefighter;

import agents.LARVAFirstAgent;
import jade.core.AID;
import jade.lang.acl.ACLMessage;
import swing.LARVADash;

public class BasicAvoidTieFighter extends LARVAFirstAgent{

    enum Status {
        CHECKIN, CHECKOUT, OPENPROBLEM, COMISSIONING, JOINSESSION, SOLVEPROBLEM, CLOSEPROBLEM, EXIT
    }
    Status mystatus;
    
    String service = "PManager", problem = "Dagobah",
            problemManager = "", 
            content, sessionKey, sessionManager, storeManager, sensorKeys;
    
    int width, height, maxFlight;
    
    ACLMessage open, session;
    
    String[] contentTokens, 
            mySensors = new String[] {
                "ALIVE",
                "GPS",
                "COMPASS",
                "LIDAR",
                "ENERGY",
                "DISTANCE",
                "ANGULAR"
            };
    
    boolean step = true, caught = false, noError = true;

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
        this.doActivateLARVADash();
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
        if (!loadMyPassport("passport/MyPassport.passport")) {
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
    
    public Status MyComissioning() {
        String localService = "STORE " + sessionKey;
        
        if(this.DFGetAllProvidersOf(localService).isEmpty()) {
            Error("Service STORE is down");
            return Status.CLOSEPROBLEM;
        }
        
        this.storeManager = this.DFGetAllProvidersOf(localService).get(0);
        Info("Found store Manager "+ this.storeManager);
        this.sensorKeys = "";
        for(String s : this.mySensors) {
            outbox = new ACLMessage();
            outbox.setSender(this.getAID());
            outbox.addReceiver(new AID(this.storeManager, AID.ISLOCALNAME));
            outbox.setContent("Request product "+s+" session "+this.sessionKey);
            this.LARVAsend(outbox);
            
            inbox = this.LARVAblockingReceive();
            if(inbox.getContent().startsWith("Confirm")) {
                Info("Bougth sensor "+s);
                this.sensorKeys += inbox.getContent().split(" ")[2] + " ";
            }
            else {
                this.Alert("Sensor "+s+" could not be obtained");
                return Status.CLOSEPROBLEM;
            }
        }
        Info("Bougth all sensor keys "+sensorKeys);
        return Status.JOINSESSION;
    }
    
    public Status MyJoinSession() {
        this.session = this.session.createReply();
        this.session.setContent("Request join session "+this.sessionKey+" attach sensors "+this.sensorKeys);
        this.LARVAsend(this.session);
        
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

    public Status MySolveProblem() {
        while(!caught) {
            double energy = myDashboard.getEnergy();
            int lidar[][] = myDashboard.getLidar();
            double gps[] = myDashboard.getGPS();
            int heigth = lidar[(int) gps[0]][(int) gps[1]];
            
            if(energy + 5 <= heigth)
            {
                for(int i = heigth; i > 0; i-=5)
                    this.sendAction("DOWN");
                this.sendAction("RECHARGE");
            }
            
            noError = readSensors();
            if(!noError)
                return Status.CLOSEPROBLEM;

            
           /* double angular = myDashboard.getAngular();
            double distance = myDashboard.getDistance();*/
            caught = myDashboard.getOnTarget();
            Info("Reading of Sensor OnTarget: "+caught);
            
            if(!caught) {
                double next_cells[][] = new double[8][2];
            
                //Filling the adjacent cells with priority right and top
                next_cells[0] = new double[]{gps[0]+1,gps[1]};
                next_cells[1] = new double[]{gps[0]+1,gps[1]-1};
                next_cells[2] = new double[]{gps[0]+1,gps[1]-1};
                next_cells[3] = new double[]{gps[0],gps[1]-1};
                next_cells[4] = new double[]{gps[0],gps[1]+1};
                next_cells[5] = new double[]{gps[0]-1,gps[1]-1};
                next_cells[6] = new double[]{gps[0]-1,gps[1]};
                next_cells[7] = new double[]{gps[0]-1,gps[1]+1};

                // Choose best option from surrounding cells
                int bestOption = 0;
                double bestValue = Double.MAX_VALUE;
                for(int i=0;i<8;i++) {
                    if(this.Heurisctic() < bestValue && this.Heurisctic() > 0)
                        bestOption = i;
                }
                
                // MOVER TIEFIGHTER
                
            } else {
                Info("Target found!");
                noError = sendAction("CAPTURE");
            }
            
        }
        
        return Status.CLOSEPROBLEM;
    }

    public Status MyCloseProblem() {
        outbox = open.createReply();
        outbox.setContent("Cancel session " + sessionKey);
        Info("Closing problem Helloworld, session " + sessionKey);
        this.LARVAsend(outbox);
        inbox = LARVAblockingReceive();
        Info(problemManager + " says: " + inbox.getContent());
        return Status.CHECKOUT;
    }

    public Status MyCheckout() {
        this.doLARVACheckout();
        return Status.EXIT;
    }
    
    private double Heurisctic(){
        return 1;
    }
    
    private boolean sendAction(String action) {
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
    
    private boolean readSensors() {
        // Sensors
        session = session.createReply();
        session.setContent("Query sensors session "+sessionKey);
        this.LARVAsend(session);

        session = this.LARVAblockingReceive();
        if(session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure")) {
            Alert("Reading of sensor failure due to "+session.getContent());
            return false;
        }
        
        return true;
    }
    
    private void Move(double angleTarget, double actualAngle){
        double angleMove = angleTarget - actualAngle;
        
        if(angleMove > 180)
            angleMove -= 360;
        if(angleMove < -180)
            angleMove += 360;
        
        if(angleMove < 0){
            //A la derecha
            while(actualAngle != angleTarget) {
                this.sendAction("RIGHT"); // REVISAR
                actualAngle -= 45.0;
                if( actualAngle < 0)
                    actualAngle += 360;
            }
        }else if(angleMove > 0){
            //A la izquierda
            this.sendAction("LEFT"); // REVISAR
                actualAngle += 45.0;
                if( actualAngle < 0)
                    actualAngle += 360;
        } 
        
        this.sendAction("MOVE");
    }
    

}
