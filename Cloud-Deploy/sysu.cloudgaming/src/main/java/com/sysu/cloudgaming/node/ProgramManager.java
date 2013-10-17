package com.sysu.cloudgaming.node;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.input.SAXBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaming.config.Config;

public class ProgramManager {
	private static Logger logger = LoggerFactory.getLogger(ProgramManager.class);
	List<ProgramBean> list=new ArrayList<ProgramBean>();
	public  List<ProgramBean> searchLocalProgram()
	{
		list.clear();
		try
		{
			File programPath=new File(Config.LOCALPROGRAMPATH);
			if(!programPath.isDirectory())
			{
				logger.warn("Program Path Not Found!");
				return list;
			}
			File insideProgramPath[]=programPath.listFiles();
			for(int i=0;i<insideProgramPath.length;i++)
			{
				if(insideProgramPath[i].isDirectory())
				{
					File gamePath=insideProgramPath[i];
					File infoFile=new File(gamePath.getAbsolutePath()+"/"+Config.LOCALPROGRAMXMLNAME);
					if(infoFile.exists())
					{
						ProgramBean b=parseProgramXmlFile(infoFile);
						if(b!=null)
						{
							list.add(b);
						}
					}
					else
					{
						logger.warn("Miss info file in {}",gamePath.getAbsolutePath());
					}
				}
			}
			return list;
		}
		
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
			return list;
		}
	}
	public  ProgramBean parseProgramXmlFile(File f)
	{
		SAXBuilder builder=new SAXBuilder();
		ProgramBean b=new ProgramBean();
		try
		{
			Document doc=builder.build(f);
			Element info=doc.getRootElement();
			b.setProgramName(info.getChild("name").getText());
			b.setProgramID(info.getChild("id").getText());
			b.setProgramVersion(info.getChild("ver").getText());
			b.setProgramPath(f.getParent()+"/"+info.getChild("path").getText());
			logger.debug("Path:{}",b.getProgramPath());
			return b;
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
			return null;
		}
		
	}
}
