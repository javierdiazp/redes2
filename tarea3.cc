#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-table-entry.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RipNgSimpleRouting");

int main(int argc, char *argv[]) {
  bool verbose = false;
  bool printRoutingTables = true;
  bool showPings = false;
  std::string SplitHorizon ("PoisonReverse");

  CommandLine cmd;
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.AddValue ("printRoutingTables", "Print routing tables at 30, 60 and 90 seconds", printRoutingTables);
  cmd.AddValue ("showPings", "Show Ping6 reception", showPings);
  cmd.AddValue ("splitHorizonStrategy", "Split Horizon strategy to use (NoSplitHorizon, SplitHorizon, PoisonReverse)", SplitHorizon);
  cmd.Parse (argc, argv);

  if (verbose)
    {
      LogComponentEnable ("RipNgSimpleRouting", LOG_LEVEL_INFO);
      LogComponentEnable ("RipNg", LOG_LEVEL_ALL);
      LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_INFO);
      LogComponentEnable ("Ipv6Interface", LOG_LEVEL_ALL);
      LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_ALL);
      LogComponentEnable ("NdiscCache", LOG_LEVEL_ALL);
      LogComponentEnable ("Ping6Application", LOG_LEVEL_ALL);
    }

  if (showPings)
    {
      LogComponentEnable ("Ping6Application", LOG_LEVEL_INFO);
    }

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

  ripNgRouting.SetInterfaceMetric (x, 1, 2);

  Ipv6ListRoutingHelper listRH;
  listRH.Add (ripNgRouting, 0); //agregar esta linea tira segfault


  InternetStackHelper internetv6;
  internetv6.SetIpv4StackInstall (false);
  internetv6.SetRoutingHelper (listRH);
  internetv6.Install (routers);

  InternetStackHelper internetv6Nodes;
  internetv6Nodes.SetIpv4StackInstall (false);
  internetv6Nodes.Install (nodes);




	NS_LOG_INFO ("Done.");
}
