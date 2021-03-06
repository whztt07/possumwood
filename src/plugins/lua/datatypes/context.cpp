#include "context.h"

namespace possumwood { namespace lua {

Context::Context() {
}

Context::~Context() {
}

Context::Context(const Context& con) {
	for(auto& v : con.m_variables)
		m_variables.push_back(v->clone());

	m_modules = con.m_modules;
}

Context& Context::operator = (const Context& con) {
	m_variables.clear();

	for(auto& v : con.m_variables)
		m_variables.push_back(v->clone());

	m_modules = con.m_modules;

	return *this;
}

bool Context::operator == (const Context& c) const {
	if(c.m_variables.size() != m_variables.size() || c.m_modules.size() != m_modules.size())
		return false;

	{
		auto i1 = m_variables.begin();
		auto i2 = c.m_variables.begin();

		while(i1 != m_variables.end()) {
			if(**i1 != **i2)
				return false;

			++i1;
			++i2;
		}
	}

	{
		auto i1 = m_modules.begin();
		auto i2 = c.m_modules.begin();

		while(i1 != m_modules.end()) {
			if(i1->first != i2->first)
				return false;

			++i1;
			++i2;
		}
	}

	return true;
}

bool Context::operator != (const Context& c) const {
	if(c.m_variables.size() != m_variables.size() || c.m_modules.size() != m_modules.size())
		return true;

	{
		auto i1 = m_variables.begin();
		auto i2 = c.m_variables.begin();

		while(i1 != m_variables.end()) {
			if(**i1 != **i2)
				return true;

			++i1;
			++i2;
		}
	}

	{
		auto i1 = m_modules.begin();
		auto i2 = c.m_modules.begin();

		while(i1 != m_modules.end()) {
			if(i1->first != i2->first)
				return true;

			++i1;
			++i2;
		}
	}

	return false;
}

void Context::addVariable(std::unique_ptr<Variable>&& var) {
	m_variables.push_back(std::move(var));
}

void Context::addModule(const std::string& name, const std::function<void(State&)>& registration) {
	m_modules[name] = registration;
}

boost::iterator_range<Context::const_var_iterator> Context::variables() const {
	return boost::make_iterator_range(
		boost::make_indirect_iterator(m_variables.begin()),
		boost::make_indirect_iterator(m_variables.end())
	);
}

boost::iterator_range<Context::const_module_iterator> Context::modules() const {
	auto fn = [](const std::pair<std::string, std::function<void(State&)>>& val) {
		return val.first;
	};

	return boost::make_iterator_range(
		boost::make_transform_iterator(m_modules.begin(), fn),
		boost::make_transform_iterator(m_modules.end(), fn)
	);
}

std::ostream& operator << (std::ostream& out, const Context& st) {
	std::cout << "Variables: ";
	for(auto& v : st.variables())
		std::cout << v.name() << "=" << v.str() << " ";
	std::cout << std::endl;

	std::cout << "Modules: ";
	for(auto& m : st.modules())
		std::cout << m << " ";
	std::cout << std::endl;

	return out;
}

}}
