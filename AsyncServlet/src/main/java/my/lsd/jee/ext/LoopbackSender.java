package my.lsd.jee.ext;

import java.util.logging.Logger;

import javax.annotation.Resource;
import javax.ejb.Stateless;
import javax.ejb.LocalBean;
import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.MessageProducer;
import javax.jms.Queue;
import javax.jms.Session;
import javax.jms.TextMessage;

@Stateless
@LocalBean
public class LoopbackSender
{
	private static final Logger logger = Logger.getLogger(LoopbackSender.class.getName());

    @Resource(mappedName = "java:/ConnectionFactory")
    private ConnectionFactory connectionFactory;
    
    @Resource(mappedName = "java:/queue/ASYNCSERVLETQueueINBOUND")
    private Queue queue;
    
    public void send(String msg) throws JMSException {
    	Connection connection = null;
        try {
        	Destination destination = queue;
        	connection = connectionFactory.createConnection();
        	Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
        	MessageProducer messageProducer = session.createProducer(destination);
        	connection.start();
        	TextMessage message = session.createTextMessage();
        	message.setText(msg);
        	messageProducer.send(message);
        	logger.info("LoopbackSender: Sent a message '" + message.getText() + "'");
        } finally {
            if (connection != null) {
                try {
                    connection.close();
                } catch (JMSException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
