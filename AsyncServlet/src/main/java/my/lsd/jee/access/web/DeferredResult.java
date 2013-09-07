package my.lsd.jee.access.web;

import java.util.Date;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.logging.Logger;

import javax.servlet.AsyncContext;
import javax.servlet.ServletRequest;

public class DeferredResult {
	private static final Logger logger = Logger.getLogger(DeferredResult.class.getName());
	
	private AsyncContext asyncContext;
	private boolean dispatch;

	private AtomicBoolean completed = new AtomicBoolean(false);
	
    public DeferredResult(AsyncContext asyncContext) {
    	this(asyncContext, true);
    }
    
    public DeferredResult(AsyncContext asyncContext, boolean dispatch) {
    	this.asyncContext = asyncContext;
        this.dispatch = dispatch;
    	logger.finest("Created deferred result id = " + 
    			this.asyncContext.getRequest().getParameter(AsyncServlet.REQ_ID) + 
    			" with dispatch = " + dispatch);
    }
    
    public boolean setResult(Object result) {
    	if (completed.compareAndSet(false, true)) {
	    	logger.finest("setResult id = " + 
	    			asyncContext.getRequest().getParameter(AsyncServlet.REQ_ID) + 
	    			", result = " + result + ", dispatch = " + dispatch);
	    	
	    	ServletRequest req = asyncContext.getRequest();
			req.setAttribute(AsyncServlet.REQ_ATTR_RESULT, result);
			req.setAttribute(AsyncServlet.REQ_ATTR_RESULT_AT, new Date());

			if (dispatch) {
	            asyncContext.dispatch("/deferredresult.jsp");
	        } else {
	        	asyncContext.complete();
	        }
			
	        return true;
    	} else {
    		logger.warning("setResult id = " +
    				asyncContext.getRequest().getParameter(AsyncServlet.REQ_ID) +
    				" returned FALSE because it has already been set");
            return false;
    	}
    }
    
    public boolean setResultNoDispatch(Object result) {
    	if (completed.compareAndSet(false, true)) {
	    	logger.finest("setResult id = " + 
	    			asyncContext.getRequest().getParameter(AsyncServlet.REQ_ID) + 
	    			", result = " + result + ", dispatch = false");
	    	
	    	ServletRequest req = asyncContext.getRequest();
			req.setAttribute(AsyncServlet.REQ_ATTR_RESULT, result);
			req.setAttribute(AsyncServlet.REQ_ATTR_RESULT_AT, new Date());

        	asyncContext.complete();
			
	        return true;
    	} else {
    		logger.warning("setResult id = " +
    				asyncContext.getRequest().getParameter(AsyncServlet.REQ_ID) +
    				" returned FALSE because it has already been set");
            return false;
    	}
    }
}
