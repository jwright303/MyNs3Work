/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"

// Default Network Topology
//
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |
// n5   n6   n7   n0 
//                  


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

/*
void
CourseChange (std::string context, Ptr<const MobilityModel> model)
{
  Vector position = model->GetPosition ();
  NS_LOG_UNCOND (context << " x = " << position.x << ", y = " << position.y);
}
*/

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nWifi = 3;
  uint32_t pacSize = 1024;
  bool tracing = false;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.AddValue ("pacSize", "Size of packet to send", pacSize);

  cmd.Parse (argc,argv);

  // The underlying restriction of 18 is due to the grid position
  // allocator's configuration; the grid layout will exceed the
  // bounding box if more than 18 nodes are provided.
  if (nWifi > 18)
    {
      std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
      return 1;
    }
  if (verbose)
    {
      LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
      //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211ax_5GHZ);
  wifi.SetObssPdAlgorithm ("ns3::ConstantObssPdAlgorithm",
		  	   "ObssPdLevel", DoubleValue (-72.0));

  YansWifiPhyHelper phy;
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  //channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  //channel.AddPropagationLoss ("ns3::LogDistancePropogationLossModel",
//		  	      "Exponent", DoubleValue (3.0),
//			      "ReferenceLoss", DoubleValue(40.0459));
  phy.SetChannel (channel.Create ());

  phy.Set ("Antennas", UintegerValue (2));
  phy.Set ("MaxSupportedTxSpatialStreams", UintegerValue (2));
  phy.Set ("MaxSupportedRxSpatialStreams", UintegerValue (2));

  WifiMacHelper mac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
		  		"DataMode",StringValue ("DsssRate1Mbps"),
				"ControlMode", StringValue ("DsssRate1Mbps"));
	  
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
	       "QosSupported", BooleanValue (true),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobility.Install (wifiStaNodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (staDevices);
  Ipv4InterfaceContainer apInterface = address.Assign (apDevices);

  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory", 
		     Address (InetSocketAddress (interfaces.GetAddress (0), port)));
  onoff.SetConstantRate (DataRate ("448kb/s"));
  ApplicationContainer apps = onoff.Install (wifiStaNodes.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  // Create a packet sink to receive these packets
  PacketSinkHelper sink ("ns3::UdpSocketFactory",
		  	 Address (InetSocketAddress (interfaces.GetAddress (nWifi-1), port)));
  apps = sink.Install (wifiStaNodes.Get (nWifi-1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

  if (tracing == true)
    {
      phy.EnablePcapAll ("third", apDevices.Get (0));
    }
/*
  std::ostringstream oss;
  oss << "/NodeList/" << wifiStaNodes.Get (nWifi - 1)->GetId () << "/$ns3::MobilityModel/CourseChange";

  Config::Connect (oss.str (), MakeCallback (&CourseChange));
*/
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
