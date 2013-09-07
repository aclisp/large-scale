package my.lsd.jee.ext;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Logger;

import javax.ejb.ConcurrencyManagement;
import javax.ejb.ConcurrencyManagementType;
import javax.ejb.Singleton;
import javax.ejb.LocalBean;
import javax.ejb.Startup;

@Singleton
@ConcurrencyManagement(ConcurrencyManagementType.BEAN)
@Startup
@LocalBean
public class StateMachine
{
	private static final Logger logger = Logger.getLogger(StateMachine.class.getName());
	
	public enum State {
		BEGIN, WAIT_1, END, TIMEOUT
	}
	
	private ConcurrentHashMap<Long, State> dataMap = new ConcurrentHashMap<Long, State>();
	private AtomicInteger stateCount = new AtomicInteger(0);

	public State put(Long key, State state) {
		logger.finest("put (key = " + key + ")");
		State pre = dataMap.put(key, state);
		if (pre == null) stateCount.incrementAndGet();
		return pre;
	}

	public State remove(Long key) {
		logger.finest("remove (key = " + key + ")");
		State pre = dataMap.remove(key);
		if (pre != null) stateCount.decrementAndGet();
		return pre;
	}	
	
	public State get(Long key) {
		return dataMap.get(key);
	}
	
	public int getStateCount() {
		return stateCount.get();
	}
}
