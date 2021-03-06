#include <possumwood_sdk/node_implementation.h>

#include <lualib.h>

#include "datatypes/context.h"
#include "datatypes/state.h"

namespace {

dependency_graph::InAttr<possumwood::lua::Context> a_inContext;
dependency_graph::OutAttr<possumwood::lua::Context> a_outContext;

dependency_graph::State compute(dependency_graph::Values& data) {
	possumwood::lua::Context context = data.get(a_inContext);

	context.addModule("math", [](possumwood::lua::State& state) {
		// load the math library
		luaL_requiref(state, LUA_MATHLIBNAME, luaopen_math, 1);
		lua_pop(state, 1);
	});

	data.set(a_outContext, context);

	return dependency_graph::State();
}

void init(possumwood::Metadata& meta) {
	meta.addAttribute(a_inContext, "in_context");
	meta.addAttribute(a_outContext, "out_context");

	meta.addInfluence(a_inContext, a_outContext);

	meta.setCompute(&compute);
}

possumwood::NodeImplementation s_impl("lua/modules/math", init);

}
