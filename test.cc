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

	NodeContainer testOne, testTwo;
  
  testOne.Create(2);
  
  testTwo.Add(testOne.Get(1));
  testTwo.Create(1);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer deviceOne, deviceTwo, deviceThree;
  deviceOne = pointToPoint.Install(testOne);
  deviceTwo = pointToPoint.Install(testTwo);

  InternetStackHelper stack;
  stack.Install(testOne);
  stack.Install(testTwo.Get(1));

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0"); //les da IP a los nodos

  Ipv4InterfaceContainer interfaceOne, interfaceTwo, interfaceThree;
  interfaceTwo = address.Assign(deviceOne);
  interfaceTwo = address.Assign(deviceTwo);
  
  UdpEchoServerHelper echoServer(9);

  ApplicationContainer serverAppOne = echoServer.Install(testTwo.Get(1));
  serverAppOne.Start(Seconds (1.0));
  serverAppOne.Stop(Seconds (10.0));

  UdpEchoClientHelper echoClient(interfaceTwo.GetAddress(1), 9);
  echoClient.SetAttribute("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute("Interval", TimeValue (Seconds (1)));
  echoClient.SetAttribute("PacketSize", UintegerValue (1024));

  ApplicationContainer clientAppOne = echoClient.Install(testOne.Get(0));
  clientAppOne.Start(Seconds (2.0));
  clientAppOne.Stop(Seconds (10.0));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
