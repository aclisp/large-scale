package my.lsd.jee.access.web;

import java.io.IOException;
import java.util.Date;
import java.util.logging.Logger;

import javax.annotation.Resource;
import javax.ejb.EJB;
import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.MessageProducer;
import javax.jms.Queue;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.servlet.AsyncContext;
import javax.servlet.AsyncEvent;
import javax.servlet.AsyncListener;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class AsyncServlet
 */
@WebServlet(urlPatterns = "/async", asyncSupported = true)
public class AsyncServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	private static final Logger logger = Logger.getLogger(AsyncServlet.class.getName());
	
	public static final String REQ_ID = "id";
	public static final String REQ_TIMEOUT = "timeout";
	public static final String REQ_DISPATCH = "dispatch";
	public static final String REQ_ATTR_RESULT = "result";
	public static final String REQ_ATTR_RECEIVED_AT = "receivedAt";
	public static final String REQ_ATTR_RESULT_AT = "resultAt";
	
    @Resource(mappedName = "java:/ConnectionFactory")
    private ConnectionFactory connectionFactory;
    
    @Resource(mappedName = "java:/queue/ASYNCSERVLETQueueOUTBOUND")
    private Queue queue;
    
    @EJB
    private DeferredResultRegistry deferredResultRegistry;
	
    public AsyncServlet() {
        super();
    }

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		logger.info("AsyncServlet: Processing request: " +
                request.getParameter(AsyncServlet.REQ_ID) + ". on thread: " +
                Thread.currentThread().getId() + ":" +
                Thread.currentThread().getName() + "[" +
                new Date() + "]");
		request.setAttribute(REQ_ATTR_RECEIVED_AT, new Date());
		
		boolean dispatch = false;
        String dispatchParam = request.getParameter(AsyncServlet.REQ_DISPATCH);
        if ("true".equalsIgnoreCase(dispatchParam)) dispatch = true;
        
        boolean timeout = false;
        String timeoutParam = request.getParameter(AsyncServlet.REQ_TIMEOUT);
        if ("true".equalsIgnoreCase(timeoutParam)) timeout = true;
        
        AsyncContext asyncCtx = request.startAsync();
        
        asyncCtx.addListener(new AsyncListener() {

			@Override
			public void onComplete(AsyncEvent ae) throws IOException {
				logger.finest("AsyncListener: onComplete for request: " +
			            ae.getAsyncContext().getRequest().getParameter(AsyncServlet.REQ_ID));
			}

			@Override
			public void onError(AsyncEvent ae) throws IOException {
				logger.finest("AsyncListener: onError for request: " +
			            ae.getAsyncContext().getRequest().getParameter(AsyncServlet.REQ_ID));
				deferredResultRegistry.remove(new Long(ae.getAsyncContext().getRequest().getParameter(AsyncServlet.REQ_ID)));
			}

			@Override
			public void onStartAsync(AsyncEvent ae) throws IOException {
				logger.finest("AsyncListener: onStartAsync for request: " +
						ae.getAsyncContext().getRequest().getParameter(AsyncServlet.REQ_ID));
			}

			@Override
			public void onTimeout(AsyncEvent ae) throws IOException {
				logger.info("AsyncListener: TIMEOUT for request: " +
			            ae.getAsyncContext().getRequest().getParameter(AsyncServlet.REQ_ID));
				
				// 5 timeout may have race condition with other thread
				DeferredResult deferredResult = deferredResultRegistry.remove(new Long(ae.getAsyncContext().getRequest().getParameter(AsyncServlet.REQ_ID)));
				if (deferredResult != null) {
					deferredResult.setResult("TIMEOUT");
				}
			}
        	
        });
        
        if (timeout) {
            asyncCtx.setTimeout(5000);
        }
        
        // 1. Construct deferredResult
        DeferredResult deferredResult = new DeferredResult(asyncCtx, dispatch);
        
        // 2. Save the deferredResult in in-memory queue
        DeferredResult oldResult = deferredResultRegistry.put(new Long(request.getParameter(AsyncServlet.REQ_ID)), deferredResult);
        if (oldResult != null) {
        	logger.warning("AsyncServlet: Replaced an outstanding request: " + request.getParameter(AsyncServlet.REQ_ID) + " previous request will be ignored");
        	// 2.1 Do nothing, just let old async request timeout
        	// But there is no way to removeListener
        	// 2.2 Make old async request complete immediately
        	oldResult.setResultNoDispatch("AGAIN");
        }
        
        // 3. About to send message
        Connection connection = null;
        JMSException error = null;
        try {
        	Destination destination = queue;
        	connection = connectionFactory.createConnection();
        	Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
        	MessageProducer messageProducer = session.createProducer(destination);
        	connection.start();
        	TextMessage message = session.createTextMessage();
        	message.setText("MSG[" + request.getParameter(AsyncServlet.REQ_ID) + "] Hello World! @" + System.currentTimeMillis());
        	messageProducer.send(message);
        	logger.info("AsyncServlet: Sent a message '" + message.getText() +"'");
        } catch (JMSException e) {
            e.printStackTrace();
            error = e;
        } finally {
            if (connection != null) {
                try {
                    connection.close();
                } catch (JMSException e) {
                    e.printStackTrace();
                }
            }
        }
        
        // 4. Message sent, other thread may switch in, there should be no code afterward.
        logger.info("AsyncServlet: Returning after request: " +
                request.getParameter(AsyncServlet.REQ_ID) + ". on thread: " +
                Thread.currentThread().getId() + ":" +
                Thread.currentThread().getName()+ "[" +
                new Date() + "]");
        
        if (error != null) {
        	// 4.1 Sent error, no other thread
        	logger.warning("AsyncServlet: Message sent with error: " + error);
        	deferredResult = deferredResultRegistry.remove(new Long(request.getParameter(AsyncServlet.REQ_ID)));
        	deferredResult.setResult(error);
        } 
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}
