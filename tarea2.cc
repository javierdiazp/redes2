#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

int main(int argc, char *argv[]) {

  //logging
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	//Parte 1: Crear 10 nodos y enlazarlos, deben ser 20 enlaces
	NodeContainer base, s0_1, s1_3, s3_4, s2_4, s0_2,
                      s0_3, s0_4, s1_4, s2_3, s1_2,
                      s2_5;
  //Creacion de los 10 nodos
  base.Create(10);

  s0_1 = NodeContainer(base.Get(0), base.Get(1));
  s1_3 = NodeContainer(base.Get(1), base.Get(3));
  s3_4 = NodeContainer(base.Get(3), base.Get(4));
  s2_4 = NodeContainer(base.Get(2), base.Get(4));
  s0_2 = NodeContainer(base.Get(0), base.Get(2));
  s0_3 = NodeContainer(base.Get(0), base.Get(3));
  s0_4 = NodeContainer(base.Get(0), base.Get(4));
  s1_4 = NodeContainer(base.Get(1), base.Get(4));
  s2_3 = NodeContainer(base.Get(2), base.Get(3));
  s1_2 = NodeContainer(base.Get(1), base.Get(2));
  s2_5 = NodeContainer(base.Get(2), base.Get(5));

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer dev0_1, dev1_3, dev3_4, dev2_4, dev0_2,
                     dev0_3, dev0_4, dev1_4, dev2_3, dev1_2,
                     dev2_5;

  dev0_1 = p2p.Install(s0_1);
  dev1_3 = p2p.Install(s1_3);
  dev3_4 = p2p.Install(s3_4);
  dev2_4 = p2p.Install(s2_4);
  dev0_2 = p2p.Install(s0_2);
  dev0_3 = p2p.Install(s0_3);
  dev0_4 = p2p.Install(s0_4);
  dev1_4 = p2p.Install(s1_4);
  dev2_3 = p2p.Install(s2_3);
  dev1_2 = p2p.Install(s1_2);
  dev2_5 = p2p.Install(s2_5);

  //Instala a nodos TCP/UDP/IP/etc.
  InternetStackHelper stack;
  stack.Install(base);


  //Hacer las asociaciones IP adress-nodo
  Ipv4AddressHelper address;
  Ipv4InterfaceContainer I0_1, I1_3, I3_4, I2_4, I0_2,
                         I0_3, I0_4, I1_4, I2_3, I1_2,
                         I2_5;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  I0_1 = address.Assign(dev0_1);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  I1_3 = address.Assign(dev1_3);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  I3_4 = address.Assign(dev3_4);

  address.SetBase ("10.1.4.0", "255.255.255.0");
  I2_4 = address.Assign(dev2_4);

  address.SetBase ("10.1.5.0", "255.255.255.0");
  I0_2 = address.Assign(dev0_2);

  address.SetBase ("10.1.6.0", "255.255.255.0");
  I0_3 = address.Assign(dev0_3);

  address.SetBase ("10.1.7.0", "255.255.255.0");
  I0_4 = address.Assign(dev0_4);

  address.SetBase ("10.1.8.0", "255.255.255.0");
  I1_4 = address.Assign(dev1_4);

  address.SetBase ("10.1.9.0", "255.255.255.0");
  I2_3 = address.Assign(dev2_3);

  address.SetBase ("10.1.10.0", "255.255.255.0");
  I1_2 = address.Assign(dev1_2);

  address.SetBase ("10.1.11.0", "255.255.255.0");
  I2_5 = address.Assign(dev2_5);
  
  //Encender routing global para que nodos se puedan comunicar entre si
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //Despues viene el trafico, parte 2, levantar la red
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverAppOne = echoServer.Install(base.Get(5));
  serverAppOne.Start(Seconds (1.0));
  serverAppOne.Stop(Seconds (10.0));

  UdpEchoClientHelper echoClient(I2_5.GetAddress(1), 9);
  echoClient.SetAttribute("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute("PacketSize", UintegerValue (1024));

  ApplicationContainer clientAppOne = echoClient.Install(base.Get(1));
  clientAppOne.Start(Seconds (2.0));
  clientAppOne.Stop(Seconds (10.0));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
