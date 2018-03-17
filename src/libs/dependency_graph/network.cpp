#include "network.h"

#include "graph.h"

namespace dependency_graph {

Network::Network(Network* parent) : NodeBase("network", parent), m_nodes(this) {
}

Network::~Network() {
}

bool Network::empty() const {
	return m_nodes.empty();
}

void Network::clear() {
	// clear only nodes - connections will clear themselves with the nodes
	m_nodes.clear();
}

Nodes& Network::nodes() {
	return m_nodes;
}

const Nodes& Network::nodes() const {
	return m_nodes;
}

Connections& Network::connections() {
	return m_connections;
}

const Connections& Network::connections() const {
	return m_connections;
}

Port& Network::port(size_t index) {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

const Port& Network::port(size_t index) const {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

const size_t Network::portCount() const {
	return 0;
}

void Network::computeInput(size_t index) {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

void Network::computeOutput(size_t index) {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

Datablock& Network::datablock() {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

void Network::setDatablock(const Datablock& data) {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

namespace {

static Metadata s_networkMetadata("network");

}

const Metadata& Network::metadata() const {
	return s_networkMetadata;
}

const Datablock& Network::datablock() const {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

const State& Network::state() const {
	assert(false);
	throw std::runtime_error("Network has no ports, for now");
}

std::unique_ptr<NodeBase> Network::makeNode(const std::string& name, const Metadata* md) {
	if(md != &s_networkMetadata)
		return std::unique_ptr<NodeBase>(new Node(name, md, this));
	else
		return std::unique_ptr<NodeBase>(new Network(this));
}

}