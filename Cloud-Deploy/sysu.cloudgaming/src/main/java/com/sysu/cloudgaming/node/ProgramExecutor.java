package com.sysu.cloudgaming.node;



import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DefaultExecutor;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ProgramExecutor {
	private static Logger logger = LoggerFactory.getLogger(ProgramExecutor.class);
	public  boolean executorProgram(String pathname)
	{
		CommandLine cmdLine = CommandLine.parse(pathname);
		DefaultExecutor executor = new DefaultExecutor();
		//executor.setExitValue(1);
		try {
			int exitValue = executor.execute(cmdLine);
			logger.debug("return {}",exitValue);
		} 
		catch (Exception e)
		{
			logger.warn(e.getMessage(),e);
			return false;
		}
		return false;
	}
}
