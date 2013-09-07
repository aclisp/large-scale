package my.lsd.jee.util;

import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Util {
	private static final Logger logger = Logger.getLogger(Util.class.getName());
	
	public static Long getReqId(String msgText) {
		Pattern pattern = Pattern.compile("^MSG\\[(\\d*)\\]");
		Matcher matcher = pattern.matcher(msgText);
		boolean match = matcher.lookingAt();
		if (match) {
			return new Long(matcher.group(1));
		} else {
			logger.warning("Can not get ReqId from msg '" + msgText + "'");
			return null;
		}
	}
	
	public static String getThreadId() {
		long id = Thread.currentThread().getId();
        String threadName = Thread.currentThread().getName();
        if (null == threadName || threadName.length() == 0)
            threadName = "";
        String threadId = id + ":" + threadName;
        return threadId;
	}
}
