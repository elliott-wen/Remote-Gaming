package com.sysu.cloudgaminghub.hub.nodenetwork;

import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.ProtocolCodecFactory;
import org.apache.mina.filter.codec.ProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolEncoder;


public class NodeNetworkCodecFactory implements ProtocolCodecFactory {
	
	private ProtocolEncoder encoder;
    private ProtocolDecoder decoder;
    public NodeNetworkCodecFactory()
    {
    	encoder = new HubMessageEncoder();
        decoder = new NodeMessageDecoder();

    }
	public ProtocolDecoder getDecoder(IoSession arg0) throws Exception {
		
		return decoder;
	}
	public ProtocolEncoder getEncoder(IoSession arg0) throws Exception {
		
		return encoder;
	}
}
