include "ns3/core-module.h"
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
	NodeContainer first, second, third, fourth, fifth,
                f1_s0;
  //creacion de los 10 nodos
  first.Create (2);
  second.Create(2);
  third.Create(2);
  fourth.Create(2);
  fifth.Create(2);

  f1_s0.Add(first.Get(1));
  f1_s0.Add(second.Get(0));

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer deviceOne, deviceTwo, deviceThree, deviceFour, deviceFive,
                      devicef1_s0;
  deviceOne = pointToPoint.Install(first);
  deviceTwo = pointToPoint.Install(second);
  deviceThree = pointToPoint.Install(third);
  deviceFour = pointToPoint.Install(fourth);
  deviceFive = pointToPoint.Install(fifth);
  devicef1_s0 = pointToPoint.Install(f1_s0);

  //instala a nodos TCP/UDP/IP/etc.
  InternetStackHelper stack;
  stack.Install (first);
  stack.Install (second);
  stack.Install (third);
  stack.Install (fourth);
  stack.Install (fifth);


  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0"); //les da IP a los nodos

  //hacer las asociaciones IP adress-nodo
  Ipv4InterfaceContainer interfaceOne, interfaceTwo, interfaceThree, interfaceFour, interfaceFive, interfacef1_s0;
  interfaceOne = address.Assign(deviceOne);
  interfaceTwo = address.Assign(deviceTwo);
  interfacef1_s0 = address.Assign(devicef1_s0);
  interfaceThree = address.Assign(deviceThree);
  interfaceFour = address.Assign(deviceFour);
  interfaceFive = address.Assign(deviceFive);
  
  //Despues viene el trafico, parte 2, levantar la red
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverAppOne = echoServer.Install(second.Get(0));
  serverAppOne.Start(Seconds (1.0));
  serverAppOne.Stop(Seconds (10.0));

  UdpEchoClientHelper echoClient(interfaceTwo.GetAddress(0), 9);
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
