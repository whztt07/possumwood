#pragma once

#include <memory>
#include <vector>

#include <boost/range/iterator_range.hpp>
#include <boost/iterator/indirect_iterator.hpp>

#include <actions/traits.h>

#include "variable.h"

namespace possumwood { namespace lua {

class State;

class Context {
	public:
		Context();
		~Context();

		Context(const Context& con);
		Context& operator = (const Context& con);

		void addVariable(std::unique_ptr<Variable>&& var);

		bool operator == (const Context& c) const;
		bool operator != (const Context& c) const;

		typedef boost::indirect_iterator<std::vector<std::unique_ptr<Variable>>::const_iterator> const_var_iterator;
		boost::iterator_range<const_var_iterator> variables() const;

	private:
		std::vector<std::unique_ptr<Variable>> m_variables;

	friend class State;

	friend std::ostream& operator << (std::ostream& out, const Context& st);
};

std::ostream& operator << (std::ostream& out, const Context& st);

}

template<>
struct Traits<lua::Context> {
	static constexpr std::array<float, 3> colour() {
		return std::array<float, 3>{{0.5, 0.5, 1}};
	}
};

}