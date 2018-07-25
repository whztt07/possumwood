#include <boost/test/unit_test.hpp>

#include <dependency_graph/graph.h>
#include <dependency_graph/node_base.inl>
#include <dependency_graph/node.h>
#include <dependency_graph/port.inl>
#include <dependency_graph/rtti.h>
#include <dependency_graph/metadata_register.h>

#include <actions/actions.h>

#include <possumwood_sdk/app.h>

#include "common.h"

using namespace dependency_graph;
using possumwood::io::json;

namespace {

dependency_graph::NodeBase& findNode(dependency_graph::Network& net, const std::string& name) {
	for(auto& n : net.nodes())
		if(n.name() == name)
			return n;

	BOOST_REQUIRE(false && "Node not found, fail");
	throw;
}

dependency_graph::NodeBase& findNode(const std::string& name) {
	return findNode(possumwood::AppCore::instance().graph(), name);
}

}

BOOST_AUTO_TEST_CASE(network_in_out_rename) {
	possumwood::App app;

	// make sure the static handles are initialised
	passThroughNode();

	// three nodes, a connection, blind data
	{
		const json result(
			{
				{
					"nodes", {
						{"network_0", {
							{"name", "test_network"},
							{"type", "network"},
							{"blind_data", nullptr},
							{"nodes", {
								{"input_0", {
									{"name", "this_is_an_input"},
									{"type", "input"},
									{"blind_data", nullptr}
								}},
								{"pass_through_0", {
									{"name", "pass_through"},
									{"type", "pass_through"},
									{"blind_data", nullptr}
								}},
								{"output_0", {
									{"name", "this_is_an_output"},
									{"type", "output"},
									{"blind_data", nullptr}
								}}
							}},
							{
								"connections", {
									{
										{"in_node", "pass_through_0"},
										{"in_port", "input"},
										{"out_node", "input_0"},
										{"out_port", "data"}
									},
									{
										{"in_node", "output_0"},
										{"in_port", "data"},
										{"out_node", "pass_through_0"},
										{"out_port", "output"}
									}
								}
							},
							{"ports", {
								{"input", 5.0},
							}},
						}}
					},
				},
				{
					"connections", "[]"_json
				}
			}
		);

		BOOST_REQUIRE_NO_THROW(app.loadFile(result));

		// lets make sure this loads and saves correctly
		{
			::json json;
			BOOST_REQUIRE_NO_THROW(app.saveFile(json, false));
			BOOST_CHECK_EQUAL(json, result);

			BOOST_REQUIRE_NO_THROW(app.loadFile(json));
		}

		// input port on the network should match the above
		auto& net = findNode("test_network").as<dependency_graph::Network>();
		BOOST_CHECK_EQUAL(net.portCount(), 2u);
		BOOST_CHECK_EQUAL(net.port(0).name(), "this_is_an_input");
		BOOST_CHECK_EQUAL(net.port(1).name(), "this_is_an_output");

		// rename the input node
		// which should change metadata and rename the input port on the network

	}
}
