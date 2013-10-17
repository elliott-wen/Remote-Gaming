package com.sysu.cloudgaming.node;

import java.io.File;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.input.SAXBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaming.config.Config;

public class ProgramUtils {
	private static Logger logger = LoggerFactory.getLogger(ProgramUtils.class);
	
	public static Map<String,ProgramBean> searchLocalProgram()
	{
		Map<String,ProgramBean> programMap=new HashMap<String,ProgramBean>();
		try
		{
			File infoFile=new File(Config.LOCALPROGRAMPATH+"/"+Config.LOCALPROGRAMXMLNAME);
			if(infoFile.exists())
			{
				SAXBuilder builder=new SAXBuilder();
				
					Document doc=builder.build(infoFile);
					Element info=doc.getRootElement();
					List<Element> games=info.getChildren("program");
					logger.info("Local disk have {} game",games.size());
					for(Element g: games)
					{
						ProgramBean b=new ProgramBean();
						b.setProgramID(g.getChildText("id"));
						b.setProgramName(g.getChildText("name"));
						b.setProgramVersion(g.getChildText("ver"));
						b.setProgramPath(Config.LOCALPROGRAMPATH+'/'+g.getChildText("path"));
						logger.info("Add Game to Map Id:{}, Name:{}",b.getProgramID(),b.getProgramName());
						programMap.put(b.getProgramID(), b);
					}
					return programMap;
				
				
			}
			else
			{
				logger.warn("Info File Not Existed!");
			}
			return programMap;
		}
		
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
			return programMap;
		}
	}

}
