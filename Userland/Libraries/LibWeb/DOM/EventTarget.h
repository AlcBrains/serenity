/*
 * Copyright (c) 2020-2022, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/FlyString.h>
#include <AK/Noncopyable.h>
#include <AK/Vector.h>
#include <LibJS/Forward.h>
#include <LibWeb/Bindings/PlatformObject.h>
#include <LibWeb/DOM/DOMEventListener.h>
#include <LibWeb/DOM/ExceptionOr.h>
#include <LibWeb/Forward.h>
#include <LibWeb/HTML/EventHandler.h>

namespace Web::DOM {

class EventTarget : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(EventTarget, Bindings::PlatformObject);

public:
    virtual ~EventTarget();

    virtual bool is_focusable() const { return false; }

    void add_event_listener(FlyString const& type, IDLEventListener* callback, Variant<AddEventListenerOptions, bool> const& options);
    void remove_event_listener(FlyString const& type, IDLEventListener* callback, Variant<EventListenerOptions, bool> const& options);

    // NOTE: These are for internal use only. They operate as though addEventListener(type, callback) was called instead of addEventListener(type, callback, options).
    void add_event_listener_without_options(FlyString const& type, IDLEventListener& callback);
    void remove_event_listener_without_options(FlyString const& type, IDLEventListener& callback);

    virtual bool dispatch_event(Event&);
    ExceptionOr<bool> dispatch_event_binding(Event&);

    virtual EventTarget* get_parent(Event const&) { return nullptr; }

    void add_an_event_listener(DOMEventListener&);
    void remove_an_event_listener(DOMEventListener&);
    void remove_from_event_listener_list(DOMEventListener&);

    Vector<JS::Handle<DOMEventListener>> event_listener_list();

    Function<void(Event const&)> activation_behavior;

    // NOTE: These only exist for checkbox and radio input elements.
    virtual void legacy_pre_activation_behavior() { }
    virtual void legacy_cancelled_activation_behavior() { }
    virtual void legacy_cancelled_activation_behavior_was_not_called() { }

    Bindings::CallbackType* event_handler_attribute(FlyString const& name);
    void set_event_handler_attribute(FlyString const& name, Bindings::CallbackType*);

protected:
    explicit EventTarget(JS::Realm&);

    void element_event_handler_attribute_changed(FlyString const& local_name, String const& value);

    virtual void visit_edges(Cell::Visitor&) override;

private:
    Vector<JS::NonnullGCPtr<DOMEventListener>> m_event_listener_list;

    // https://html.spec.whatwg.org/multipage/webappapis.html#event-handler-map
    // Spec Note: The order of the entries of event handler map could be arbitrary. It is not observable through any algorithms that operate on the map.
    HashMap<FlyString, JS::GCPtr<HTML::EventHandler>> m_event_handler_map;

    Bindings::CallbackType* get_current_value_of_event_handler(FlyString const& name);
    void activate_event_handler(FlyString const& name, HTML::EventHandler& event_handler);
    void deactivate_event_handler(FlyString const& name);
    JS::ThrowCompletionOr<void> process_event_handler_for_event(FlyString const& name, Event& event);
};

bool is_window_reflecting_body_element_event_handler(FlyString const& name);

}
