package my.lsd.jee.access.web;

import java.util.logging.Logger;

import javax.ejb.ActivationConfigProperty;
import javax.ejb.EJB;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.MessageListener;
import javax.jms.TextMessage;
import javax.ejb.MessageDriven;

import my.lsd.jee.util.Util;

@MessageDriven(name = "AsyncInboundResponder", activationConfig = { 
		@ActivationConfigProperty(propertyName = "acknowledgeMode", propertyValue = "Auto-acknowledge"),
		@ActivationConfigProperty(propertyName = "destinationType", propertyValue = "javax.jms.Queue"), 
		@ActivationConfigProperty(propertyName = "destination", propertyValue = "queue/ASYNCSERVLETQueueINBOUND") })
public class AsyncInboundResponder implements MessageListener
{
	private static final Logger logger = Logger.getLogger(AsyncInboundResponder.class.getName());

    @EJB
    private DeferredResultRegistry deferredResultRegistry;
	
	@Override
	public void onMessage(Message message)
	{
		logger.finest(message.toString());
		
		TextMessage msg = null;
		try {
			if (message instanceof TextMessage) {
				msg = (TextMessage) message;
	            logger.info("AsyncInboundResponder: Received text message: " + msg.getText());
	            
				Long reqIdLong = Util.getReqId(msg.getText());
				
				// 6 
				DeferredResult deferredResult = deferredResultRegistry.remove(reqIdLong);
				if (deferredResult != null && 
						deferredResult.setResult(msg.getText())) {
					// SUCCESS
					logger.info("SUCCESS for request: " + reqIdLong);
				} else {
					// TIMEOUT
					logger.info("TIMEOUT for request: " + reqIdLong);
				}
			} else {
				logger.warning("AsyncInboundResponder: Received message of wrong type: " + message.getClass().getName());
			}
		} catch (JMSException e) {
            throw new RuntimeException(e);
        }
	}
}
