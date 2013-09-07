package my.lsd.jee.access.web;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Logger;

import javax.ejb.ConcurrencyManagement;
import javax.ejb.ConcurrencyManagementType;
import javax.ejb.LocalBean;
import javax.ejb.Singleton;
import javax.ejb.Startup;

@Singleton
@ConcurrencyManagement(ConcurrencyManagementType.BEAN)
@Startup
@LocalBean
public class DeferredResultRegistry
{
	private static final Logger logger = Logger.getLogger(DeferredResultRegistry.class.getName());
	
	private ConcurrentHashMap<Long, DeferredResult> resultMap = new ConcurrentHashMap<Long, DeferredResult>();
	private AtomicInteger resultCount = new AtomicInteger(0);

	public DeferredResult put(Long key, DeferredResult deferredResult) {
		logger.finest("put (key = " + key + ")");
		DeferredResult pre = resultMap.put(key, deferredResult);
		if (pre == null) resultCount.incrementAndGet();
		return pre;
	}

	public DeferredResult remove(Long key) {
		logger.finest("remove (key = " + key + ")");
		DeferredResult pre = resultMap.remove(key);
		if (pre != null) resultCount.decrementAndGet();
		return pre;
	}
	
	public int getResultCount() {
		return resultCount.get();
	}
}
