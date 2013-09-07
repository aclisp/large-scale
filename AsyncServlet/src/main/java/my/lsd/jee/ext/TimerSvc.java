package my.lsd.jee.ext;

import java.util.Iterator;
import java.util.PriorityQueue;
import java.util.concurrent.Semaphore;
import java.util.logging.Logger;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import javax.ejb.ConcurrencyManagement;
import javax.ejb.ConcurrencyManagementType;
import javax.ejb.DependsOn;
import javax.ejb.EJB;
import javax.ejb.Singleton;
import javax.ejb.LocalBean;
import javax.ejb.Startup;
import javax.jms.JMSException;

import my.lsd.jee.access.web.DeferredResultRegistry;

@DependsOn({"DeferredResultRegistry", "StateMachine"})
@Singleton
@ConcurrencyManagement(ConcurrencyManagementType.BEAN)
@Startup
@LocalBean
public class TimerSvc
{
	private static final Logger logger = Logger.getLogger(TimerSvc.class.getName());
	
	private static final boolean DISPLAY_TICK = true;
	private static final long TIMER_RESOLUTION = 500;
	private Thread timerIsr;
	private Thread timer;
	private Semaphore timerIsrSem;
	private volatile boolean shutdownRequested;
	private volatile boolean initializeComleted;
	
	@PostConstruct
	private void init() {
		logger.info("TimerSvc initialize");
		
		shutdownRequested = false;
		initializeComleted = false;
		timerIsrSem = new Semaphore(0);
		timerHashTable = new TimerPriorityQueue[TIMER_HASH_SIZE];
		for (int i = 0; i < TIMER_HASH_SIZE; i++) {
			timerHashTable[i] = new TimerPriorityQueue();
		}
		timerCount = 0;

		timerIsr = new Thread(new Runnable() {
			public void run() {
				while (!shutdownRequested) {
					timerIsrSem.release();
					try { Thread.sleep(TIMER_RESOLUTION); } catch (InterruptedException e) { }
				}
			}
		}, "REALTIME_TIMER_ISR");
		timerIsr.start();
		
		timer = new Thread(new Runnable() {
			public void run() {
				while (!shutdownRequested) {
					try { timerIsrSem.acquire(); } catch (InterruptedException e) { }
					if (initializeComleted) timerExpireHandler();
				}
			}
		}, "REALTIME_TIMER");
		timer.start();
		
		initializeComleted = true;
	}
	
	@PreDestroy
	private void fini() {
		logger.info("TimerSvc finish");
		
		shutdownRequested = true;
		timerIsr.interrupt();
		timer.interrupt();
				
	}
	
	private long beforeTick = 0;
	private long currentTick = 0;
	private long timerIdSequence = 0;
	private static final int TIMER_HASH_SIZE = 4111; // prime number to minimize hash collision
	private TimerPriorityQueue[] timerHashTable;
	private int timerCount;
	
	// for debugging purpose only
	@EJB private DeferredResultRegistry deferredResultRegistry;
	@EJB private StateMachine stateMachine;	
	
	private void timerExpireHandler() {
		long startMsec = 0;
		beforeTick = currentTick;
		currentTick = System.currentTimeMillis();
		
		if (DISPLAY_TICK) {
			if (currentTick/1000 != beforeTick/1000) {
				//logger.info("tick: " + currentTick + " timer count: " + timerCount + " pending result count: " + deferredResultRegistry.getResultCount() + " pending state count: " + stateMachine.getStateCount());
				startMsec = System.currentTimeMillis();
			}
		}
		
		checkExpiredTimer();
		
		if (DISPLAY_TICK) {
			if (currentTick/1000 != beforeTick/1000) {
				logger.info("proc_time: " + (System.currentTimeMillis() - startMsec) + " timer count: " + timerCount + " pending result count: " + deferredResultRegistry.getResultCount() + " pending state count: " + stateMachine.getStateCount());
			}
		}
	}
	
	private synchronized long getTimerId() {
		timerIdSequence++;
		if (timerIdSequence == Long.MAX_VALUE) timerIdSequence = 0;
		return timerIdSequence;
	}
	
	private static int getTimerHashIndex(long timerid) {
		return (int) (timerid % TIMER_HASH_SIZE);
	}	

	private class TimerPriorityQueue {
		private final PriorityQueue<Timer> priorityQueue = new PriorityQueue<Timer>();
	}
	
	private class Timer implements Comparable<Timer> {
		private long timerid = getTimerId();
		private long exptick = 0;
		private long reqid = 0;
		public Timer(long timeout, long reqid) {
			currentTick = System.currentTimeMillis();
			this.exptick = currentTick + timeout;
			this.reqid = reqid;
			if (this.exptick <= currentTick) throw new IllegalArgumentException();
		}
		// Note: this class has a natural ordering that is inconsistent with equals.
		public int compareTo(Timer o) {
			long delta = this.exptick - o.exptick;
			if (delta == 0) return 0;
			else if (delta > 0) return 1;
			else return -1;
		}
	}
	
	public long setTimer(long timeout, long reqid) {
		Timer timer = new Timer(timeout, reqid);
		long timerid = timer.timerid;
		int hashIndex = getTimerHashIndex(timerid);
		synchronized (timerHashTable[hashIndex]) {
			timerHashTable[hashIndex].priorityQueue.add(timer);
			timerCount++;
		}
		return timerid;
	}
	
	public void cancelTimer(long timerid) {
		int hashIndex = getTimerHashIndex(timerid);
		boolean exist = false;
		synchronized (timerHashTable[hashIndex]) {
			Iterator<Timer> iter = timerHashTable[hashIndex].priorityQueue.iterator();
			while (iter.hasNext()) {				
				if (iter.next().timerid == timerid) {
					iter.remove();
					exist = true;
					if (timerCount != 0) timerCount--;
					break;
				}
			}
		}
		if (exist == false) {
			logger.warning("Canceling a non-existing timer id=(" + timerid + ") ");
		}
	}
	
	@EJB
	private SelfSender selfSender;
	
	private void checkExpiredTimer() {
		Timer timer = null;
		for (int hashIndex = 0; hashIndex < TIMER_HASH_SIZE; hashIndex++) {
			synchronized (timerHashTable[hashIndex]) {
				while ((timer = timerHashTable[hashIndex].priorityQueue.peek()) != null) {
					if (currentTick >= timer.exptick) {

						try {
							selfSender.send("MSG[" + timer.reqid + "] TIMEOUT");
						} catch (JMSException e) {
							e.printStackTrace();
						}
						
						timerHashTable[hashIndex].priorityQueue.poll();
						if (timerCount != 0) timerCount--;
					} else {
						break;
					}
				}
			}
		}
	}
}
