#pragma once

#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>

#include <QWidget>

#include <dependency_graph/node_base.inl>
#include <dependency_graph/port.inl>
#include <dependency_graph/unique_id.h>

#include <possumwood_sdk/app.h>
#include <actions/actions.h>

#include "factory.inl"

namespace possumwood { namespace properties {

class property_base : public boost::noncopyable {
	public:
		enum {
			kInput = 1,
			kOutput = 2,
			kDisabled = 4,
			// kDirty = 8 // not needed - the widget refreshes itself immediately
		};

		virtual ~property_base() = 0;

		virtual QWidget* widget() = 0;
		virtual std::type_index type() const = 0;

		unsigned flags() const;
		void setFlags(unsigned flags);

		/// called when the UI value is changed by the user
		boost::signals2::connection valueCallback(std::function<void()> fn);

		/// transfer the typed value to a port
		virtual void valueToPort(dependency_graph::Port& port) const = 0;
		/// transfer the typed value from a port
		virtual void valueFromPort(dependency_graph::Port& port) = 0;

	protected:
		/// can be reimplemented to change the UI's properties based on flag changes
		virtual void onFlagsChanged(unsigned flags);

		/// a callback to be connected to widget's "onChange" signal
		void callValueChangedCallbacks();

	private:
		unsigned m_flags;
		boost::signals2::signal<void()> m_valueChanged;
};

template<typename T, typename DERIVED>
class property : public property_base {
	public:
		typedef T result_type;
		typedef DERIVED derived_type;

		property() : m_blockedSignals(false) {
		}

		virtual ~property() {
			// make sure the factory is not optimized away
			static factory_typed<DERIVED>* dummy;
			dummy = &s_pf;
			dummy->type();
		}

		virtual void get(T& value) const = 0;
		virtual void set(const T& value) = 0;

		virtual std::type_index type() const override {
			return typeid(T);
		}

	private:
		virtual void valueToPort(dependency_graph::Port& port) const override {
			// transfer the templated value
			if((flags() & kInput) && !(flags() & kDisabled) && !m_blockedSignals) {
				// get the current value
				T value = port.get<T>();
				// allow the UI to change it (or a part of it)
				get(value);
				// and use action to apply it to a port (making it undoable)
				possumwood::actions::setValue(port, value);
			}
		}

		void valueFromPort(dependency_graph::Port& port) override {
			// transfer the templated value
			if(!m_blockedSignals && port.type() == dependency_graph::unmangledName(type().name())) {
				bool block = widget()->blockSignals(true);
				m_blockedSignals = true;
				set(port.get<T>());
				widget()->blockSignals(block);
				m_blockedSignals = false;
			}
		}

		bool m_blockedSignals;

		static factory_typed<DERIVED> s_pf;
};

template<typename T, typename DERIVED>
factory_typed<DERIVED> property<T, DERIVED>::s_pf;

} }
