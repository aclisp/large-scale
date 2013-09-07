package my.lsd.jee.ext;

import java.util.Date;
import java.util.Random;
import java.util.logging.Logger;

import javax.ejb.EJB;
import javax.ejb.MessageDriven;
import javax.ejb.ActivationConfigProperty;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.MessageListener;
import javax.jms.TextMessage;

import my.lsd.jee.util.Util;

@MessageDriven(name = "LoopbackResponder", activationConfig =  {
        @ActivationConfigProperty(propertyName = "acknowledgeMode", propertyValue = "Auto-acknowledge"),
        @ActivationConfigProperty(propertyName = "destinationType", propertyValue = "javax.jms.Queue"),
        @ActivationConfigProperty(propertyName = "destination", propertyValue = "queue/ASYNCSERVLETQueueOUTBOUND")
})
public class LoopbackResponder implements MessageListener 
{
	private static final Logger logger = Logger.getLogger(LoopbackResponder.class.getName());
	
	@EJB
	private LoopbackSender sender;
	
	@EJB
	private TimerSvc timerSvc;
	
	@EJB
	private StateMachine stateMachine;
	
	@Override
	public void onMessage(Message message) {
		logger.finest(message.toString());
		
		TextMessage msg = null;
		try {
            if (message instanceof TextMessage) {
                msg = (TextMessage) message;
                logger.info("LoopbackResponder: Received text message: " + msg.getText());
                
                Long reqIdLong = Util.getReqId(msg.getText());
                String reqId;
                if (reqIdLong == null) {
                	reqId = "<unk>";
                } else {
                	reqId = reqIdLong.toString();
                }
                
                String threadId = Util.getThreadId();
                String msgBody = msg.getText().substring(msg.getText().indexOf(']') + 2);
                String result = longRunningProcess(reqId, threadId, msgBody);
                
                if (reqIdLong != null && result != null) {
                	sender.send("MSG[" + reqId + "] " + result + " @" + System.currentTimeMillis() + " Reqmsg=" + msg.getText());
                }
                
            } else {
                logger.warning("LoopbackResponder: Received message of wrong type: " + message.getClass().getName());
            }
        } catch (JMSException e) {
            throw new RuntimeException(e);
        }
	}
	

    private String longRunningProcess(final String reqId, String threadId, String msgBody) {
    	String result = null;
        Random randomGenerator = new Random();
        int randomInt = randomGenerator.nextInt(6000);
        logger.info("AsyncRequestProcessor: Begin long run process (" +
                    randomInt + " ms) for request: " +
                    reqId + ". on thread: " + threadId +
                    "[" + new Date() + "]");
 
//        try {
//            Thread.sleep(randomInt);
//        } catch (InterruptedException ie) {
//            logger.log(Level.WARNING, ie.getMessage(), ie);
//        }

        /*
         * Maintains a state machine as 
         * +------+---------------
         * |reqId | state
         * +------+---------------
         * +    1 | BEGIN
         * +    2 | BEGIN
         * 
         * State transfer is
         * BEGIN --> WAIT_1 --> END
         *              |
         *              +--> TIMEOUT
         */ 
        
        // 1. get or create task state
        StateMachine.State state = stateMachine.get(new Long(reqId));
        if (state == null) {
        	stateMachine.put(new Long(reqId), StateMachine.State.BEGIN);
        	state = StateMachine.State.BEGIN;
        }
        
        // 2. check current task state, and process
        switch (state) {
        case BEGIN:
            timerSvc.setTimer(randomInt, Long.parseLong(reqId));
            // 3. send an out-going message
            // 4. change state
            state = StateMachine.State.WAIT_1;
            break;
        case WAIT_1:
        	if (msgBody.startsWith("TIMEOUT")) 
        		state = StateMachine.State.TIMEOUT;
        	else
        		logger.warning("Received unexpected message in WAIT_1 for request: " + reqId);
        	break;
        case END:
        	break;
        case TIMEOUT:
        	break;
        default:
        	logger.warning("Unknown state " + state + " for request: " + reqId);
        	break;
        }
        
        // after processing, write back state if necessary
        if (state == StateMachine.State.END || state == StateMachine.State.TIMEOUT) {
        	stateMachine.remove(new Long(reqId));
        	result = new Date().toString();
        } else {
        	stateMachine.put(new Long(reqId), state); 
        	result = null;
        	logger.info("AsyncRequestProcessor: NOOP. state = " + state + " for request: " + reqId);
        }
 
        logger.info("AsyncRequestProcessor: Done processing request: " +
                    reqId + ". on thread: " +
                    threadId + "[" + new Date() + "]");
        return result;
    }
}
