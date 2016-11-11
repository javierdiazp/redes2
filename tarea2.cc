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
                      s2_5, s5_6, s6_7, s7_8, s8_9,
                      s9_10, s5_11, s11_12, s12_13,
                      s13_14, s14_15, s15_16, s16_17,
                      s17_18, s18_19, s7_19, s12_19;
  //Creacion de los nodos
  base.Create(20);

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
  s5_6 = NodeContainer(base.Get(5), base.Get(6));
  s6_7 = NodeContainer(base.Get(6), base.Get(7));
  s7_8 = NodeContainer(base.Get(7), base.Get(8));
  s8_9 = NodeContainer(base.Get(8), base.Get(9));
  s9_10 = NodeContainer(base.Get(9), base.Get(10));
  s5_11 = NodeContainer(base.Get(5), base.Get(11));
  s11_12 = NodeContainer(base.Get(11), base.Get(12));
  s12_13 = NodeContainer(base.Get(12), base.Get(13));
  s13_14 = NodeContainer(base.Get(13), base.Get(14));
  s14_15 = NodeContainer(base.Get(14), base.Get(15));
  s15_16 = NodeContainer(base.Get(15), base.Get(16));
  s16_17 = NodeContainer(base.Get(16), base.Get(17));
  s17_18 = NodeContainer(base.Get(17), base.Get(18));
  s18_19 = NodeContainer(base.Get(18), base.Get(19));
  s7_19 = NodeContainer(base.Get(7), base.Get(19));
  s12_19 = NodeContainer(base.Get(12), base.Get(19));


  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer dev0_1, dev1_3, dev3_4, dev2_4, dev0_2,
                     dev0_3, dev0_4, dev1_4, dev2_3, dev1_2,
                     dev2_5, dev5_6, dev6_7, dev7_8, dev8_9,
                     dev9_10, dev5_11, dev11_12, dev12_13,
                     dev13_14, dev14_15, dev15_16, dev16_17,
                     dev17_18, dev18_19, dev7_19, dev12_19;

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
  dev5_6 = p2p.Install(s5_6);
  dev6_7 = p2p.Install(s6_7);
  dev7_8 = p2p.Install(s7_8);
  dev8_9 = p2p.Install(s8_9);
  dev9_10 = p2p.Install(s9_10);
  dev5_11 = p2p.Install(s5_11);
  dev11_12 = p2p.Install(s11_12);
  dev12_13 = p2p.Install(s12_13);
  dev13_14 = p2p.Install(s13_14);
  dev14_15 = p2p.Install(s14_15);
  dev15_16 = p2p.Install(s15_16);
  dev16_17 = p2p.Install(s16_17);
  dev17_18 = p2p.Install(s17_18);
  dev18_19 = p2p.Install(s18_19);
  dev7_19 = p2p.Install(s7_19);
  dev12_19 = p2p.Install(s12_19);

  //Instala a nodos TCP/UDP/IP/etc.
  InternetStackHelper stack;
  stack.Install(base);


  //Hacer las asociaciones IP adress-nodo
  Ipv4AddressHelper address;
  Ipv4InterfaceContainer I0_1, I1_3, I3_4, I2_4, I0_2,
                         I0_3, I0_4, I1_4, I2_3, I1_2,
                         I2_5, I5_6, I6_7, I7_8, I8_9,
                         I9_10, I5_11, I11_12, I12_13,
                         I13_14, I14_15, I15_16, I16_17,
                         I17_18, I18_19, I7_19, I12_19;

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

  address.SetBase ("10.1.12.0", "255.255.255.0");
  I5_6 = address.Assign(dev5_6);

  address.SetBase ("10.1.13.0", "255.255.255.0");
  I6_7 = address.Assign(dev6_7);
  
  address.SetBase ("10.1.14.0", "255.255.255.0");
  I7_8 = address.Assign(dev7_8);
  
  address.SetBase ("10.1.15.0", "255.255.255.0");
  I8_9 = address.Assign(dev8_9);
  
  address.SetBase ("10.1.16.0", "255.255.255.0");
  I9_10 = address.Assign(dev9_10);

  address.SetBase ("10.1.17.0", "255.255.255.0");
  I5_11 = address.Assign(dev5_11);

  address.SetBase ("10.1.18.0", "255.255.255.0");
  I11_12 = address.Assign(dev11_12);

  address.SetBase ("10.1.19.0", "255.255.255.0");
  I12_13 = address.Assign(dev12_13);

  address.SetBase ("10.1.20.0", "255.255.255.0");
  I13_14 = address.Assign(dev13_14);

  address.SetBase ("10.1.21.0", "255.255.255.0");
  I14_15 = address.Assign(dev14_15);

  address.SetBase ("10.1.22.0", "255.255.255.0");
  I15_16 = address.Assign(dev15_16);

  address.SetBase ("10.1.23.0", "255.255.255.0");
  I16_17 = address.Assign(dev16_17);

  address.SetBase ("10.1.24.0", "255.255.255.0");
  I17_18 = address.Assign(dev17_18);

  address.SetBase ("10.1.25.0", "255.255.255.0");
  I18_19 = address.Assign(dev18_19);

  address.SetBase ("10.1.26.0", "255.255.255.0");
  I7_19 = address.Assign(dev7_19);

  address.SetBase ("10.1.27.0", "255.255.255.0");
  I12_19 = address.Assign(dev12_19);

  //Encender routing global para que nodos se puedan comunicar entre si
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //Despues viene el trafico, parte 2, levantar la red
  UdpEchoServerHelper echoServer (9);

  //Dos servers para probar la red
  ApplicationContainer serverAppOne = echoServer.Install(base.Get(10));
  serverAppOne.Start(Seconds (1.0));
  serverAppOne.Stop(Seconds (10.0));

  ApplicationContainer serverAppTwo = echoServer.Install(base.Get(16));
  serverAppTwo.Start(Seconds (1.0));
  serverAppTwo.Stop(Seconds (10.0));

  //Luego, dos clientes
  UdpEchoClientHelper echoClientOne(I9_10.GetAddress(1), 9);
  echoClientOne.SetAttribute("MaxPackets", UintegerValue (1));
  echoClientOne.SetAttribute("Interval", TimeValue (Seconds (1.0)));
  echoClientOne.SetAttribute("PacketSize", UintegerValue (1024));

  ApplicationContainer clientAppOne = echoClientOne.Install(base.Get(1));
  clientAppOne.Start(Seconds (2.0));
  clientAppOne.Stop(Seconds (10.0));

  UdpEchoClientHelper echoClientTwo(I16_17.GetAddress(0), 9);
  echoClientTwo.SetAttribute("MaxPackets", UintegerValue (1));
  echoClientTwo.SetAttribute("Interval", TimeValue (Seconds (1.0)));
  echoClientTwo.SetAttribute("PacketSize", UintegerValue (1024));

  ApplicationContainer clientAppTwo = echoClientTwo.Install(base.Get(3));
  clientAppTwo.Start(Seconds (2.5));
  clientAppTwo.Stop(Seconds (10.0));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
