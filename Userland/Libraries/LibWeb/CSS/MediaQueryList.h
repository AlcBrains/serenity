/*
 * Copyright (c) 2021-2022, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Forward.h>
#include <LibWeb/CSS/MediaQuery.h>
#include <LibWeb/DOM/EventTarget.h>

namespace Web::CSS {

// 4.2. The MediaQueryList Interface, https://drafts.csswg.org/cssom-view/#the-mediaquerylist-interface
class MediaQueryList final : public DOM::EventTarget {
    WEB_PLATFORM_OBJECT(MediaQueryList, DOM::EventTarget);

public:
    static JS::NonnullGCPtr<MediaQueryList> create(DOM::Document&, NonnullRefPtrVector<MediaQuery>&&);

    virtual ~MediaQueryList() override = default;

    String media() const;
    bool matches() const;
    bool evaluate();

    void add_listener(DOM::IDLEventListener*);
    void remove_listener(DOM::IDLEventListener*);

    void set_onchange(Bindings::CallbackType*);
    Bindings::CallbackType* onchange();

private:
    MediaQueryList(DOM::Document&, NonnullRefPtrVector<MediaQuery>&&);

    virtual void visit_edges(Cell::Visitor&) override;

    JS::NonnullGCPtr<DOM::Document> m_document;
    NonnullRefPtrVector<MediaQuery> m_media;
};

}
