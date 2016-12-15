#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-table-entry.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RipNgSimpleRouting");

void TearDownLink (Ptr<Node> nodeA, Ptr<Node> nodeB, uint32_t interfaceA, uint32_t interfaceB)
{
  nodeA->GetObject<Ipv6> ()->SetDown (interfaceA);
  nodeB->GetObject<Ipv6> ()->SetDown (interfaceB);
}

int main(int argc, char *argv[]) {
  bool printRoutingTables = true;
  std::string SplitHorizon ("PoisonReverse");

  CommandLine cmd;
  cmd.AddValue ("printRoutingTables", "Print routing tables at 30, 60, 90 and 120 seconds", printRoutingTables);
  cmd.AddValue ("splitHorizonStrategy", "Split Horizon strategy to use (NoSplitHorizon, SplitHorizon, PoisonReverse)", SplitHorizon);
  cmd.Parse (argc, argv);

  if (SplitHorizon == "NoSplitHorizon")
    {
      Config::SetDefault ("ns3::RipNg::SplitHorizon", EnumValue (RipNg::NO_SPLIT_HORIZON));
    }
  else if (SplitHorizon == "SplitHorizon")
    {
      Config::SetDefault ("ns3::RipNg::SplitHorizon", EnumValue (RipNg::SPLIT_HORIZON));
    }
  else
    {
      Config::SetDefault ("ns3::RipNg::SplitHorizon", EnumValue (RipNg::POISON_REVERSE));
    }

  //Creacion de los nodos de la topologia
  NS_LOG_INFO ("Create nodes.");
  Ptr<Node> src = CreateObject<Node> ();
  Names::Add ("SrcNode", src);
  Ptr<Node> dst = CreateObject<Node> ();
  Names::Add ("DstNode", dst);
  Ptr<Node> t = CreateObject<Node> ();
  Names::Add ("RouterT", t);
  Ptr<Node> u = CreateObject<Node> ();
  Names::Add ("RouterU", u);
  Ptr<Node> v = CreateObject<Node> ();
  Names::Add ("RouterV", v);
  Ptr<Node> w = CreateObject<Node> ();
  Names::Add ("RouterW", w);
  Ptr<Node> x = CreateObject<Node> ();
  Names::Add ("RouterX", x);
  Ptr<Node> y = CreateObject<Node> ();
  Names::Add ("RouterY", y);
  Ptr<Node> z = CreateObject<Node> ();
  Names::Add ("RouterZ", z);

  //Containers para nodos que van a ir enlazados
  NodeContainer net1 (src, z);
  NodeContainer net2 (z, x);
  NodeContainer net3 (z, y);
  NodeContainer net4 (y, x);
  NodeContainer net5 (y, v);
  NodeContainer net6 (y, t);
  NodeContainer net7 (x, v);
  NodeContainer net8 (x, w);
  NodeContainer net9 (v, t);
  NodeContainer net10 (v, w);
  NodeContainer net11 (v, u);
  NodeContainer net12 (w, u);
  NodeContainer net13 (t, u);
  NodeContainer net14 (u, dst);

  //Containers de los routers y los hosts
  NodeContainer routers (t, u, v, w, x);
  routers.Add(y);
  routers.Add(z);
  NodeContainer nodes (src, dst);

  NS_LOG_INFO ("Create channels.");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  NetDeviceContainer ndc1 = csma.Install (net1);
  NetDeviceContainer ndc2 = csma.Install (net2);
  NetDeviceContainer ndc3 = csma.Install (net3);
  NetDeviceContainer ndc4 = csma.Install (net4);
  NetDeviceContainer ndc5 = csma.Install (net5);
  NetDeviceContainer ndc6 = csma.Install (net6);
  NetDeviceContainer ndc7 = csma.Install (net7);
  NetDeviceContainer ndc8 = csma.Install (net8);
  NetDeviceContainer ndc9 = csma.Install (net9);
  NetDeviceContainer ndc10 = csma.Install (net10);
  NetDeviceContainer ndc11 = csma.Install (net11);
  NetDeviceContainer ndc12 = csma.Install (net12);
  NetDeviceContainer ndc13 = csma.Install (net13);
  NetDeviceContainer ndc14 = csma.Install (net14);

  NS_LOG_INFO ("Create IPv6 and routing");
  RipNgHelper ripNgRouting;

  ripNgRouting.ExcludeInterface (z, 1);
  ripNgRouting.ExcludeInterface (u, 4);

  //Costos de los enlaces

  //Enlace z-x
  ripNgRouting.SetInterfaceMetric (z, 2, 8);
  ripNgRouting.SetInterfaceMetric (x, 1, 8);

  //Enlace z-y
  ripNgRouting.SetInterfaceMetric (z, 3, 12);
  ripNgRouting.SetInterfaceMetric (y, 1, 12);

  //Enlace y-x
  ripNgRouting.SetInterfaceMetric (y, 2, 6);
  ripNgRouting.SetInterfaceMetric (x, 2, 6);

  //Enlace y-v
  ripNgRouting.SetInterfaceMetric (y, 3, 8);
  ripNgRouting.SetInterfaceMetric (v, 1, 8);

  //Enlace y-t
  ripNgRouting.SetInterfaceMetric (y, 4, 7);
  ripNgRouting.SetInterfaceMetric (t, 1, 7);

  //Enlace x-v
  ripNgRouting.SetInterfaceMetric (x, 3, 3);
  ripNgRouting.SetInterfaceMetric (v, 2, 3);

  //Enlace x-w
  ripNgRouting.SetInterfaceMetric (x, 4, 6);
  ripNgRouting.SetInterfaceMetric (w, 1, 6);

  //Enlace v-t
  ripNgRouting.SetInterfaceMetric (v, 3, 4);
  ripNgRouting.SetInterfaceMetric (t, 2, 4);

  //Enlace v-w
  ripNgRouting.SetInterfaceMetric (v, 4, 4);
  ripNgRouting.SetInterfaceMetric (w, 2, 4);

  //Enlace v-u
  ripNgRouting.SetInterfaceMetric (v, 5, 3);
  ripNgRouting.SetInterfaceMetric (u, 1, 3);

  //Enlace w-u
  ripNgRouting.SetInterfaceMetric (w, 3, 3);
  ripNgRouting.SetInterfaceMetric (u, 2, 3);

  //Enlace t-u
  ripNgRouting.SetInterfaceMetric (t, 3, 2);
  ripNgRouting.SetInterfaceMetric (u, 3, 2);

  Ipv6ListRoutingHelper listRH;
  listRH.Add (ripNgRouting, 0);


  InternetStackHelper internetv6;
  internetv6.SetIpv4StackInstall (false);
  internetv6.SetRoutingHelper (listRH);
  internetv6.Install (routers);

  InternetStackHelper internetv6Nodes;
  internetv6Nodes.SetIpv4StackInstall (false);
  internetv6Nodes.Install (nodes);

  //Asignar direcciones IPv6

  NS_LOG_INFO ("Assign IPv6 Addresses.");
  Ipv6AddressHelper ipv6;

  ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic1 = ipv6.Assign (ndc1);
  iic1.SetForwarding (1, true);
  iic1.SetDefaultRouteInAllNodes (1);

  ipv6.SetBase (Ipv6Address ("2001:0:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic2 = ipv6.Assign (ndc2);
  iic2.SetForwarding (0, true);
  iic2.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:2::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic3 = ipv6.Assign (ndc3);
  iic3.SetForwarding (0, true);
  iic3.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:3::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic4 = ipv6.Assign (ndc4);
  iic4.SetForwarding (0, true);
  iic4.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:4::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic5 = ipv6.Assign (ndc5);
  iic5.SetForwarding (0, true);
  iic5.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:5::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic6 = ipv6.Assign (ndc6);
  iic6.SetForwarding (0, true);
  iic6.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:6::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic7 = ipv6.Assign (ndc7);
  iic7.SetForwarding (0, true);
  iic7.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:7::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic8 = ipv6.Assign (ndc8);
  iic8.SetForwarding (0, true);
  iic8.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:8::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic9 = ipv6.Assign (ndc9);
  iic9.SetForwarding (0, true);
  iic9.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:9::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic10 = ipv6.Assign (ndc10);
  iic10.SetForwarding (0, true);
  iic10.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:10::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic11 = ipv6.Assign (ndc11);
  iic11.SetForwarding (0, true);
  iic11.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:11::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic12 = ipv6.Assign (ndc12);
  iic12.SetForwarding (0, true);
  iic12.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:0:12::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic13 = ipv6.Assign (ndc13);
  iic13.SetForwarding (0, true);
  iic13.SetForwarding (1, true);

  ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer iic14 = ipv6.Assign (ndc14);
  iic14.SetForwarding (0, true);
  iic14.SetDefaultRouteInAllNodes (0);

  //Instrucciones para imprimir tablas de ruta de los routers a los 30, 60, 90 y 120 segundos
  if (printRoutingTables)
    {
      RipNgHelper routingHelper;

      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);

      routingHelper.PrintRoutingTableAt (Seconds (30.0), z, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), y, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), x, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), w, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), v, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), t, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), u, routingStream);


      routingHelper.PrintRoutingTableAt (Seconds (60.0), z, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), y, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), x, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), w, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), v, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), t, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), u, routingStream);

      routingHelper.PrintRoutingTableAt (Seconds (90.0), z, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (90.0), y, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (90.0), x, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (90.0), w, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (90.0), v, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (90.0), t, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (90.0), u, routingStream);

      routingHelper.PrintRoutingTableAt (Seconds (120.0), z, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (120.0), y, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (120.0), x, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (120.0), w, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (120.0), v, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (120.0), t, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (120.0), u, routingStream);
    }

  //Creacion de la aplicacion: Un ping por segundo desde src a dst por 130 segundos
  NS_LOG_INFO ("Create Applications.");
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 100;
  Time interPacketInterval = Seconds (1.0); //Un ping por segundo
  Ping6Helper ping6;

  ping6.SetLocal (iic1.GetAddress (0, 1));
  ping6.SetRemote (iic14.GetAddress (1, 1));
  ping6.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  ping6.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping6.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps = ping6.Install (src);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (131.0));

  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream ("dynamic-network.tr"));
  csma.EnablePcapAll ("dynamic-network", true);

  //Romper enlaces
  Simulator::Schedule (Seconds (40), &TearDownLink, v, u, 5, 1);
  Simulator::Schedule (Seconds (70), &TearDownLink, x, v, 3, 2);
  Simulator::Schedule (Seconds (100), &TearDownLink, x, w, 4, 1);
  
  //Arrojar simulacion
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (132)); //1 segundo mas que el ultimo ping
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
