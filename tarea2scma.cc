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
	NodeContainer base, first, second, third, fourth, fifth,
                f1_s0;
  //creacion de los 10 nodos
  base.Create(10);

  first.Add(base.Get(0));
  first.Add(base.Get(1));

  second.Add(base.Get(2));
  second.Add(base.Get(3));

  third.Add(base.Get(4));
  third.Add(base.Get(5));

  fourth.Add(base.Get(6));
  fourth.Add(base.Get(7));

  fifth.Add(base.Get(8));
  fifth.Add(base.Get(9));


  f1_s0.Add(first.Get(1));
  f1_s0.Add(second.Get(0));

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer device;
  device = csma.Install(base);

  //instala a nodos TCP/UDP/IP/etc.
  InternetStackHelper stack;
  stack.Install(base);


  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0"); //les da IP a los nodos

  //hacer las asociaciones IP adress-nodo
  Ipv4InterfaceContainer interface;
  interface = address.Assign(device);

  //Despues viene el trafico, parte 2, levantar la red
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverAppOne = echoServer.Install(second.Get(1));
  serverAppOne.Start(Seconds (1.0));
  serverAppOne.Stop(Seconds (10.0));

  UdpEchoClientHelper echoClient(interface.GetAddress(3), 9);
  echoClient.SetAttribute("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute("Interval", TimeValue (Seconds (2.0)));
  echoClient.SetAttribute("PacketSize", UintegerValue (1024));

  ApplicationContainer clientAppOne = echoClient.Install(first.Get(0));
  clientAppOne.Start(Seconds (2.0));
  clientAppOne.Stop(Seconds (10.0));

  Simulator::Run();
  Simulator::Destroy();


  return 0;
}
