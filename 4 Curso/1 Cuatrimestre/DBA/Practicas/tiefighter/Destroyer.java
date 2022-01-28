package tiefighter;

import agents.LARVAFirstAgent;
import jade.core.AID;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;
import static java.lang.Math.*;
import swing.LARVADash;
import java.util.ArrayList;

public class Destroyer extends LARVAFirstAgent{
    enum Status {
        CHECKIN, CHECKOUT, OPENPROBLEM,COMISSIONING,JOINSESSION, SOLVEPROBLEM, CLOSEPROBLEM, EXIT
    }
    Status mystatus;
    String service = "PManager", 
           problem = "Bogano",
           password = "105Wing",
           problemManager = "",
           mymap,
           content, 
           sessionKey, 
           sessionManager,
           storeManager,
           sensorKeys,
           nextAction;
    
    int width, height, maxFlight, rechargesAvailable = 5;
    int contTF = 0, contCOR=0, height_TF = 0,height_COR = 0,stepsTF1 = 0, stepsTF2 = 0;
    int indexCOR1 = 0, indexCOR2 = 0;
    
    ACLMessage open, session,TF1,TF2,COR1,COR2;
    String[] contentTokens,mySensors = new String[]{
        "GPS",
    };
    boolean step = true,goRecharge = true,resuelto = false,endTF1 = false,endTF2 = false,endCOR1 = false,endCOR2 = false;
    
    AID idTF1,idTF2,idCOR1,idCOR2,PM;
    ArrayList<Jedi> jedisEncounteredCOR1 = new ArrayList<Jedi>();
    ArrayList<Jedi> jedisEncounteredCOR2 = new ArrayList<Jedi>();
    Square posTF1 = new Square();
    Square posTF2 = new Square();
    
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
        this.DFSetMyServices(new String[]{"DESTROYER " + password});
        return Status.OPENPROBLEM;
    }

    public Status MyOpenProblem() {
        if (this.DFGetAllProvidersOf(service).isEmpty()) {
            Error("Service PMANAGER is down");
            return Status.CHECKOUT;
        }
        problemManager = this.DFGetAllProvidersOf(service).get(0);
        Info("Found problem manager " + problemManager);
        
        int nAgents;
        do {
            nAgents = 0;
            nAgents += DFGetAllProvidersOf("FIGHTER " + password).size();
            nAgents += DFGetAllProvidersOf("CORELLIAN " + password).size();
            //nAgents += DFGetAllProvidersOf("RAZOR " + password).size();
        } while (nAgents<8);
        
        this.outbox = new ACLMessage();
        outbox.setSender(getAID());
        outbox.setPerformative(ACLMessage.REQUEST);
        outbox.addReceiver(new AID(problemManager, AID.ISLOCALNAME));
        outbox.setContent("Request open " + problem);
        this.LARVAsend(outbox);
        Info("Request opening problem " + problem + " to " + problemManager);
        open = this.LARVAblockingReceive();
        PM = open.getSender();
        
        Info(problemManager + " says: " + open.getContent());
        content = open.getContent();
        contentTokens = content.split(" ");
        if (contentTokens[0].toUpperCase().equals("AGREE")) {
            sessionKey = contentTokens[4];
            session = this.LARVAblockingReceive();
            sessionManager = session.getSender().getLocalName();
            Info(sessionManager + " says: " + session.getContent());
            return Status.COMISSIONING;
        } else {
            Error(content);
            return Status.CHECKOUT;
        }
    }

    public Status MyCloseProblem() {
        
        int nAgents;
        do {
            nAgents = 0;
            nAgents += DFGetAllProvidersOf("FIGHTER " + password).size();
            nAgents += DFGetAllProvidersOf("CORELLIAN " + password).size();
            //nAgents += DFGetAllProvidersOf("RAZOR " + password).size();
        } while (nAgents>0);
        
        outbox = open.createReply();
        outbox.setContent("Cancel session " + sessionKey);
        outbox.setPerformative(ACLMessage.CANCEL);
        Info("Closing problem, session " + sessionKey);
        this.LARVAsend(outbox);
        inbox = this.LARVAblockingReceive(MessageTemplate.MatchSender(PM));
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
        session.setContent("REQUEST JOIN SESSION "+sessionKey+" AS DESTROYER AT 0 0 ATTACH SENSORS "+sensorKeys);
        session.setPerformative(ACLMessage.REQUEST);
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
    
    
    /**
     * Solveproblem for destroyer
     * @author Raul Castro
     */
    public Status MySolveProblem() {
        if(this.updateSensors()){
            this.getMap();
        }
        
        this.obtainHeightsTFCOR();
        
        this.recruitment();
        this.acceptFighters();
        
        boolean exitTF1= false,exitTF2=false;
        while(!endTF1 || !endTF2 || !exitTF1 || !exitTF2){
            this.inbox = this.LARVAblockingReceive();
            // PRIMER FIGHTER (CUADRANTE IZQUIERDO)
            if(this.inbox.getSender().equals(idTF1)){
                if(this.inbox.getPerformative() == ACLMessage.INFORM){
                    if(endTF1){
                        this.endFighters(this.inbox,1);
                        exitTF1 = true;
                    }
                    else{    
                        this.recorridoFighter(this.inbox,stepsTF1,1);
                    }
                }
                else if(this.inbox.getPerformative() == ACLMessage.QUERY_IF){
                    this.recharge(this.inbox);
                }
                else if(this.inbox.getPerformative() == ACLMessage.INFORM_REF){
                    this.saveJedis(this.inbox);
                }
            }
            // SEGUNDO FIGHTER (CUADRANTE DERECHO)
            else if(this.inbox.getSender().equals(idTF2)){
                if(this.inbox.getPerformative() == ACLMessage.INFORM){
                    if(endTF2){
                        this.endFighters(this.inbox,2);
                        exitTF2 = true;
                    }
                    else{    
                        this.recorridoFighter(this.inbox,stepsTF2,2);
                    }
                }
                else if(this.inbox.getPerformative() == ACLMessage.QUERY_IF){
                    this.recharge(this.inbox);
                }
                else if(this.inbox.getPerformative() == ACLMessage.INFORM_REF){
                    this.saveJedis(this.inbox);
                }
            }
        }
        
        this.jedisEncounteredCOR1.sort((j1,j2) -> j1.y > j2.y ? 1 : -1);
        this.jedisEncounteredCOR2.sort((j1,j2) -> j1.y > j2.y ? 1 : -1);
        
        
        this.acceptCorellians();
        
        boolean exitCOR1= false,exitCOR2=false;
        while(!endCOR1 || !endCOR2 || !exitCOR1 || !exitCOR2){
            this.inbox = this.LARVAblockingReceive();
            if(this.inbox.getInReplyTo().equals("TAKEOFF")){
                Info("CORELLIAN HA LLEGADO AL MUNDO");
            }
            // PRIMER CORELLIAN (CUADRANTE IZQUIERDO)
            else if(this.inbox.getSender().equals(idCOR1)){
                if(this.inbox.getPerformative() == ACLMessage.INFORM){
                    if(endCOR1){
                        this.endCorellians(this.inbox);
                        exitCOR1 = true;
                    }
                    else{    
                        this.sendJedisToCorellians(this.inbox,1);
                    }
                }
                else if(this.inbox.getPerformative() == ACLMessage.QUERY_IF){
                    this.recharge(this.inbox);
                }
            // SEGUNDO CORELLIAN (CUADRANTE DERECHO)
            }else if(this.inbox.getSender().equals(idCOR2)){
                if(this.inbox.getPerformative() == ACLMessage.INFORM){
                    if(endCOR2){
                        this.endCorellians(this.inbox);
                        exitCOR2 = true;
                    }
                    else{    
                        this.sendJedisToCorellians(this.inbox,2);
                    }
                }
                else if(this.inbox.getPerformative() == ACLMessage.QUERY_IF){
                    this.recharge(this.inbox);
                }
            }
        }
        
        return Status.CLOSEPROBLEM; 
    }
    
    
    /**
     * Recruit the 2 fighters and 2 corellians and reject the others
     * @author Raul Castro
     */
    public void recruitment(){
        
        this.outbox = new ACLMessage();
        outbox.setConversationId(sessionKey);
        outbox.setSender(this.getAID());
        
        for(int i = 0; i < this.DFGetAllProvidersOf("FIGHTER "+password).size(); i++)
            outbox.addReceiver(new AID(this.DFGetAllProvidersOf("FIGHTER "+password).get(i), AID.ISLOCALNAME));
        
        outbox.setPerformative(ACLMessage.CFP);
        outbox.setOntology("COMMITMENT");
        outbox.setReplyWith("RECRUITMENT FIGHTER");
        outbox.setContent(mymap);
        this.LARVAsend(outbox);
        
        this.outbox = new ACLMessage();
        outbox.setConversationId(sessionKey);
        outbox.setSender(this.getAID());
        for(int i = 0; i < this.DFGetAllProvidersOf("CORELLIAN "+password).size(); i++)
            outbox.addReceiver(new AID(this.DFGetAllProvidersOf("CORELLIAN "+password).get(i), AID.ISLOCALNAME));
        outbox.setPerformative(ACLMessage.CFP);
        outbox.setOntology("COMMITMENT");
        outbox.setReplyWith("RECRUITMENT CORELLIAN");
        outbox.setContent(mymap);
        this.LARVAsend(outbox);
        
        int numTfCor = this.DFGetAllProvidersOf("CORELLIAN "+password).size() + this.DFGetAllProvidersOf("FIGHTER "+password).size();
        
        for(int i = 0; i < numTfCor ; i++){
            this.inbox = this.LARVAblockingReceive();
            if(inbox.getInReplyTo().equals("RECRUITMENT FIGHTER") && inbox.getPerformative() == ACLMessage.AGREE && contTF < 2){
                if(contTF == 0){
                    TF1 = inbox;
                    idTF1 = inbox.getSender();
                    contTF++;
                }
                else if(contTF == 1){
                    TF2 = inbox;
                    idTF2 = inbox.getSender();
                    contTF++;
                }
            }
            else if(inbox.getInReplyTo().equals("RECRUITMENT CORELLIAN") && inbox.getPerformative() == ACLMessage.AGREE && contCOR < 2){
                if(contCOR == 0){
                    COR1 = inbox;
                    idCOR1 = inbox.getSender();
                    contCOR++;
                }
                else if (contCOR == 1){
                    COR2 = inbox;
                    idCOR2 = inbox.getSender();
                    contCOR++;
                }
            }
            else{
                outbox = inbox.createReply();
                outbox.setPerformative(ACLMessage.REJECT_PROPOSAL);
                outbox.setOntology("COMMITMENT");
                outbox.setContent("Cancel crew");
                this.LARVAsend(outbox);
            }
            
        } 
    }
    
    /**
     * Accept the fighters to enter the world with the coordinates
     * @author Raul Castro
     */
    public void acceptFighters(){
        TF1 = TF1.createReply();
        TF1.setPerformative(ACLMessage.ACCEPT_PROPOSAL);
        TF1.setOntology("COMMITMENT");
        TF1.setReplyWith("TAKEOFF");
        TF1.setContent(10+" "+10);
        this.LARVAsend(TF1);
        
        TF2 = TF2.createReply();
        TF2.setPerformative(ACLMessage.ACCEPT_PROPOSAL);
        TF2.setOntology("COMMITMENT");
        TF2.setReplyWith("TAKEOFF");
        TF2.setContent((width/2+10)+" "+10);
        this.LARVAsend(TF2);
    }
    
    
    /**
      * Accept the corellian to enter the world in the location of the first jedi
      * @author Alejandro Cuenca
      * @author Raúl Castro
    */
    public void acceptCorellians(){
        COR1 = COR1.createReply();
        COR1.setPerformative(ACLMessage.ACCEPT_PROPOSAL);
        COR1.setOntology("COMMITMENT");
        COR1.setReplyWith("TAKEOFF");
        COR1.setContent((this.jedisEncounteredCOR1.get(indexCOR1).x)+" "+(this.jedisEncounteredCOR1.get(indexCOR1).y));
        this.LARVAsend(COR1);
        
        COR2 = COR2.createReply();
        COR2.setPerformative(ACLMessage.ACCEPT_PROPOSAL);
        COR2.setOntology("COMMITMENT");
        COR2.setReplyWith("TAKEOFF");
        COR2.setContent((this.jedisEncounteredCOR2.get(indexCOR1).x)+" "+(this.jedisEncounteredCOR2.get(indexCOR1).y));
        this.LARVAsend(COR2);
    }
    
    /**
     * Send jedis position for each corellian
     * @author Raul Castro Moreno
     */
    public void sendJedisToCorellians(ACLMessage msg,int COR){
        msg = msg.createReply();
        msg.setPerformative(ACLMessage.REQUEST);
        msg.setOntology("COMMITMENT");
        if(COR == 1){
            this.jedisEncounteredCOR1.get(indexCOR1).captured();
            indexCOR1++;
            if(indexCOR1 < this.jedisEncounteredCOR1.size()){
                Jedi aux = new Jedi();
                aux.setPos(this.jedisEncounteredCOR1.get(indexCOR1).x, this.jedisEncounteredCOR1.get(indexCOR1).y);
                msg.setContent("CAPTURE "+aux.x+" "+aux.y);
                msg.setReplyWith("CAPTURE "+aux.x+" "+aux.y);
                if(indexCOR1 == this.jedisEncounteredCOR1.size()-1)
                    endCOR1 = true;
            }
            this.LARVAsend(msg);
        }else if(COR == 2){
            this.jedisEncounteredCOR2.get(indexCOR2).captured();
            indexCOR2++;
            if(indexCOR2 < this.jedisEncounteredCOR2.size()){
                Jedi aux = new Jedi();
                aux.setPos(this.jedisEncounteredCOR2.get(indexCOR2).x, this.jedisEncounteredCOR2.get(indexCOR2).y);
                msg.setContent("CAPTURE "+aux.x+" "+aux.y);
                msg.setReplyWith("CAPTURE "+aux.x+" "+aux.y);
                if(indexCOR2 == this.jedisEncounteredCOR2.size()-1)
                    endCOR2 = true;
            }
            this.LARVAsend(msg);
        }
    }
    
    
    /**
     * Cancel the fighters when they finished the route
     * @author Raul Castro
     */
    public void endFighters(ACLMessage msg,int TF){
        String parse[] = msg.getContent().split(" ");
        msg = msg.createReply();
        if(TF == 1)
            posTF1.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2])); 
        else if(TF == 2)
            posTF2.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
            
        msg.setPerformative(ACLMessage.CANCEL);
        msg.setOntology("COMMITMENT");
        msg.setContent("Cancel crew");
        this.LARVAsend(msg);
    }
    
    /**
     * Cancel the corellians when each capture all the jedis
     * @author Raul Castro
     */
    public void endCorellians(ACLMessage msg){
        msg = msg.createReply();
        msg.setPerformative(ACLMessage.CANCEL);
        msg.setOntology("COMMITMENT");
        msg.setContent("Cancel crew");
        this.LARVAsend(msg);
    }
    
    /**
     * Calculate the route and send the route points to the fighter
     * @author Raul Castro
     */
    public void recorridoFighter(ACLMessage msg,int steps,int TF){
        int timesHorizontal = ((width/2)-20)/20;
        if(((width/2)-20)%20 > 0)
            timesHorizontal++;
        
        int totalSteps = timesHorizontal*2;
        
        String parse[];
      
        if(steps == 0){
            parse = msg.getContent().split(" ");
            msg = msg.createReply();
            if(TF == 1){
                    posTF1.setXY(Integer.parseInt(parse[0]), Integer.parseInt(parse[1]));
                    msg.setContent("MOVE "+posTF1.x+" "+(height-10)+" "+height_TF);
                }
                else if(TF == 2){
                    posTF2.setXY(Integer.parseInt(parse[0]), Integer.parseInt(parse[1]));
                    msg.setContent("MOVE "+posTF2.x+" "+(height-10)+" "+height_TF);
                }
            
            msg.setPerformative(ACLMessage.REQUEST);
            this.LARVAsend(msg);
            
            if(TF == 1)
                stepsTF1++;
            else if(TF == 2)
                stepsTF2++;
            
        }else if(steps > 0){
            if(steps % 2 == 1){
            //HORIZONTAL
                parse = msg.getContent().split(" ");
                msg = msg.createReply();
                if(TF == 1){
                    posTF1.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
                    if(steps == totalSteps - 1)
                        msg.setContent("MOVE "+((width/2)-10)+" "+posTF1.y+" "+height_TF);
                    else
                        msg.setContent("MOVE "+(posTF1.x+20)+" "+posTF1.y+" "+height_TF);
                }
                else if(TF == 2){
                    posTF2.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
                    if(steps == totalSteps - 1)
                        msg.setContent("MOVE "+(width-10)+" "+posTF2.y+" "+height_TF);
                    else
                        msg.setContent("MOVE "+(posTF2.x+20)+" "+posTF2.y+" "+height_TF);
                }
                
                msg.setPerformative(ACLMessage.REQUEST);
                
                this.LARVAsend(msg);
                if(TF == 1)
                    stepsTF1++;
                else if(TF == 2)
                    stepsTF2++;
            }
            else if(steps % 2 == 0){
            //VERTICAL
                parse = msg.getContent().split(" ");
                msg = msg.createReply();
                if(TF == 1){
                    posTF1.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
                    if(posTF1.y == 10)
                        msg.setContent("MOVE "+posTF1.x+" "+(height-10)+" "+height_TF);
                    else
                        msg.setContent("MOVE "+posTF1.x+" "+10+" "+height_TF);
                }
                else if(TF == 2){
                    posTF2.setXY(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
                    if(posTF2.y == 10)
                        msg.setContent("MOVE "+posTF2.x+" "+(height-10)+" "+height_TF);
                    else
                        msg.setContent("MOVE "+posTF2.x+" "+10+" "+height_TF);
                }
                    
                msg.setPerformative(ACLMessage.REQUEST);
                this.LARVAsend(msg);
                if(TF == 1){
                    if(stepsTF1 == totalSteps)
                        endTF1 = true;
                    else
                        stepsTF1++;
                }
                else if(TF == 2){
                    if(stepsTF2 == totalSteps)
                        endTF2 = true;
                    else
                        stepsTF2++;
                }
            }
        }
 
    }
    
    /**
     * Manages the recharges for the fighters and for the corellians
     * @author Raul Castro
     */
    public void recharge(ACLMessage msg){
        
        msg = msg.createReply();
        msg.setContent("RECHARGE");
        if(this.rechargesAvailable > 0){
            msg.setPerformative(ACLMessage.CONFIRM);
            this.rechargesAvailable--;
        }else{
            msg.setPerformative(ACLMessage.DISCONFIRM);
        }
        this.LARVAsend(msg);
    }
    
    /**
     * Save the jedis locations sended by the fighters
     * @author Raul Castro
     */
    public void saveJedis(ACLMessage msg){
        Jedi aux = new Jedi();
        String parse[] = msg.getContent().split(" ");
        aux.setPos(Integer.parseInt(parse[1]), Integer.parseInt(parse[2]));
        boolean exists = false;
        if(aux.x < width/2){
            for(Jedi j : this.jedisEncounteredCOR1){
                if(j.x == aux.x && j.y == aux.y)
                    exists = true;
            }
            if(!exists)
                this.jedisEncounteredCOR1.add(aux);
        }
        else{
            for(Jedi j : this.jedisEncounteredCOR2){
                if(j.x == aux.x && j.y == aux.y)
                    exists = true;
            }
            if(!exists)
                this.jedisEncounteredCOR2.add(aux);
        }
    }
    
    /**
     * Go to the maximum map height and query the map
     * @author Raul Castro
     */
    public void getMap(){
        int altitude = (int)myDashboard.getGPS()[2];
        int timesUp = (maxFlight-altitude)/5;
        Info("ALT "+altitude);
        Info("MAX "+maxFlight);
        Info("UP "+timesUp);
        for(int i = 0 ; i < timesUp;i++){
            this.sendAction("UP");
            
        }
        this.updateSensors();
        session = session.createReply();
        session.setPerformative(ACLMessage.QUERY_REF);
        session.setContent("QUERY MAP SESSION "+sessionKey);
        this.LARVAsend(session);
        session = this.LARVAblockingReceive();
        mymap = session.getContent();
    }
    
    /**
     * Obtain the heights efficient to fly for the corellians and fighters
     * @author Raul Castro
     */
    public void obtainHeightsTFCOR(){
        int max = 0;
        for(int i = 0; i < width;i++){
            for(int j = 0; j < height;j++){
                if( myDashboard.getMapLevel(i, j) > max)
                  max = myDashboard.getMapLevel(i, j);
            }
        }
        this.height_COR = max;
        this.height_TF = max + 5;
    }
    
    public boolean sendAction(String action) {
        session = session.createReply();
        session.setContent("Request execute "+ action +" session "+sessionKey);
        session.setPerformative(ACLMessage.REQUEST);
        this.LARVAsend(session);
        
        session = this.LARVAblockingReceive();
        if(session.getContent().startsWith("Refuse") || session.getContent().startsWith("Failure")) {
            Alert("Execute action failure due to "+session.getContent());
            return false;
        }
        return true;
    }
    
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
    
}
