/* 
 *              weupnp - Trivial upnp java library 
 *
 * Copyright (C) 2008 Alessandro Bahgat Shehata, Daniele Castagna
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, FΩifth Floor, Boston, MA  02110-1301  USA
 * 
 * Alessandro Bahgat Shehata - ale dot bahgat at gmail dot com
 * Daniele Castagna - daniele dot castagna at gmail dot com
 * 
 */

/*
 * refer to miniupnpc-1.0-RC8
 */
package com.sysu.cloudgaming.node.network.upnp;

import java.net.InetAddress;


import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaming.App;

/**
 * This class contains a trivial main method that can be used to test whether
 * weupnp is able to manipulate port mappings on a IGD (Internet Gateway
 * Device) on the same network.
 *
 * @author Alessandro Bahgat Shehata
 */
public class Main {
	private static Logger logger = LoggerFactory.getLogger(Main.class);
    private static int SAMPLE_PORT = 12540;
    private static short WAIT_TIME = 10;
    
    public static void main(String[] args) throws Exception{
    

        GatewayDiscover discover = new GatewayDiscover();
        logger.info("Looking for Gateway Devices");
        discover.discover();
        GatewayDevice d = discover.getValidGateway();

        if (null != d) {
            logger.info("Gateway device found.\n{} ({})", new Object[]{d.getModelName(), d.getModelDescription()});
        } else {
            logger.info("No valid gateway device found.");
            return;
        }
        
        InetAddress localAddress = d.getLocalAddress();
        logger.info("Using local address: {}", localAddress);
        String externalIPAddress = d.getExternalIPAddress();
        logger.info("External address: {}", externalIPAddress);
        PortMappingEntry portMapping = new PortMappingEntry();

        logger.info("Attempting to map port {}", SAMPLE_PORT);
        logger.info("Querying device to see if mapping for port {} already exists", SAMPLE_PORT);

        if (!d.getSpecificPortMappingEntry(SAMPLE_PORT,"UDP",portMapping)) {
            logger.info("Sending port mapping request");

            if (d.addPortMapping(SAMPLE_PORT,SAMPLE_PORT,localAddress.getHostAddress(),"UDP","test")) {
                logger.info("Mapping succesful: waiting {} seconds before removing mapping.", WAIT_TIME);
                
                Thread.sleep(1000*WAIT_TIME);
                d.deletePortMapping(SAMPLE_PORT,"UDP");

                logger.info("Port mapping removed");
                logger.info("Test SUCCESSFUL");
            } else {
                logger.info("Port mapping removal failed");
                logger.info("Test FAILED");
            }
            
        } else {
            logger.info("Port was already mapped. Aborting test.");
        }

        logger.info("Stopping weupnp");
    }
    
}
