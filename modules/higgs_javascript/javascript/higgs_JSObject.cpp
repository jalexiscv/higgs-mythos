/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

namespace higgs
{

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wsubobject-linkage")
class JSObject::Impl
{
public:
    using ValuePtr = detail::qjs::QuickJSContext::ValuePtr;
    using ValueHandle = detail::QuickJSWrapper::ValueHandle;

    explicit Impl (detail::QuickJSWrapper* engineIn)
        : Impl (ValueHandle { *engineIn, detail::qjs::JS_GetGlobalObject (engineIn->getQuickJSContext()) })
    {
    }

    std::unique_ptr<Impl> getChild (const Identifier& prop) const
    {
        auto* engine = valuePtr.getEngine();

        if (engine == nullptr)
            return rawToUniquePtr (new Impl (ValueHandle{}));

        return rawToUniquePtr (new Impl (ValueHandle { *engine, detail::getOrCreateProperty (engine->getQuickJSContext(),
                                                                                             valuePtr.get(),
                                                                                             prop.toString().toRawUTF8()) }));
    }

    std::unique_ptr<Impl> getChild (int64 index) const
    {
        jassert (isArray());
        auto* engine = valuePtr.getEngine();

        if (engine == nullptr)
            return rawToUniquePtr (new Impl (ValueHandle{}));

        return rawToUniquePtr (new Impl (ValueHandle { *engine, detail::qjs::JS_GetPropertyUint32 (engine->getQuickJSContext(),
                                                                                                   valuePtr.get(),
                                                                                                   detail::toUint32 (index)) }));
    }

    bool hasProperty (const Identifier& name) const
    {
        if (auto* ctx = getContext())
            return detail::hasProperty (ctx, valuePtr.get(), name.toString().toRawUTF8());

        return false;
    }

    void setProperty (const Identifier& name, const var& value) const
    {
        if (auto* ctx = getContext())
            detail::qjs::JS_SetPropertyStr (ctx, valuePtr.get(), name.toString().toRawUTF8(), detail::higgsToQuickJs (value, ctx));
    }

    void setProperty (int64 index, const var& value) const
    {
        if (auto* ctx = getContext())
            detail::qjs::JS_SetPropertyInt64 (ctx, valuePtr.get(), index, detail::higgsToQuickJs (value, ctx));
    }

    var get() const
    {
        auto* ctx = getContext();

        if (ctx == nullptr)
            return var::undefined();

        if (auto* opaque = detail::qjs::JS_GetOpaque (valuePtr.get(), detail::DynamicObjectWrapper::getClassId()))
            if (detail::DynamicObjectWrapper::getDynamicObjects().count (opaque) != 0)
                return { static_cast<detail::DynamicObjectWrapper*> (opaque)->object.get() };

        return detail::discardError (detail::quickJSToHiggs ({ detail::qjs::JS_DupValue (ctx, valuePtr.get()), ctx }));
    }

    detail::VarOrError invokeMethod (const Identifier& methodName, Span<const var> args) const
    {
        auto* engine = valuePtr.getEngine();

        if (engine == nullptr)
            return String ("The JavascriptEngine that owned this object has been destroyed");

        const detail::QuickJSWrapper::ScopedExecution execution { *engine };

        if (! hasProperty (methodName))
        {
            jassertfalse;
            return {};
        }

        auto* ctx = engine->getQuickJSContext();
        const auto methodAtom = JS_NewAtom (ctx, methodName.toString().toRawUTF8());
        ScopeGuard scope { [&] { detail::qjs::JS_FreeAtom (ctx, methodAtom); } };

        detail::JSFunctionArguments arguments { ctx, args };

        ValuePtr returnVal { detail::qjs::JS_Invoke (ctx,
                                                     valuePtr.get(),
                                                     methodAtom,
                                                     arguments.getSize(),
                                                     arguments.getArguments()),
                             ctx };

        return detail::quickJSToHiggs (returnVal);
    }

    NamedValueSet getProperties() const
    {
        auto* ctx = getContext();

        if (ctx == nullptr)
            return {};

        NamedValueSet result;

        ValuePtr names { detail::qjs::JS_GetOwnPropertyNames2 (ctx,
                                                               valuePtr.get(),
                                                               detail::qjs::JS_GPN_ENUM_ONLY | detail::qjs::JS_GPN_STRING_MASK,
                                                               detail::qjs::JS_ITERATOR_KIND_KEY),
                         ctx };


        if (auto v = detail::discardError (detail::quickJSToHiggs (names)); const auto* propertyNames = v.getArray())
        {
            for (const auto& name : *propertyNames)
            {
                if (name.isString())
                {
                    const Identifier prop { name.toString() };
                    result.set (prop, getChild (prop)->get());
                }
            }
        }

        return result;
    }

    bool isArray() const
    {
        if (auto* ctx = getContext())
            return detail::qjs::JS_IsArray (ctx, valuePtr.get());

        return false;
    }

    int64 getSize() const
    {
        if (! isArray())
        {
            jassertfalse;
            return 0;
        }

        auto* ctx = getContext();
        ValuePtr lengthProp { detail::qjs::JS_GetPropertyStr (ctx, valuePtr.get(), "length"), ctx };
        uint32_t length = 0;
        detail::qjs::JS_ToUint32 (ctx, &length, lengthProp.get());
        return (int64) length;
    }

private:
    explicit Impl (ValueHandle ptr)
        : valuePtr (std::move (ptr))
    {
    }

    detail::qjs::JSContext* getContext() const
    {
        auto* engine = valuePtr.getEngine();
        return engine != nullptr ? engine->getQuickJSContext() : nullptr;
    }

    ValueHandle valuePtr;
};
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

JSObject::JSObject (detail::QuickJSWrapper* engine)
    : impl (new Impl (engine))
{
}

JSObject::JSObject (std::unique_ptr<Impl> implIn)
    : impl (std::move (implIn))
{
}

JSObject::JSObject (const JSObject& other)
    : impl (new Impl (*other.impl))
{
}

JSObject::~JSObject() = default;

JSObject::JSObject (JSObject&&) noexcept = default;

JSObject& JSObject::operator= (const JSObject& other)
{
    JSObject { other }.swap (*this);
    return *this;
}

JSObject& JSObject::operator= (JSObject&& other) noexcept = default;

JSObject JSObject::getChild (const Identifier& name) const
{
    return JSObject { impl->getChild (name) };
}

JSObject JSObject::operator[] (const Identifier& name) const
{
    return getChild (name);
}

bool JSObject::isArray() const
{
    return impl->isArray();
}

int64 JSObject::getSize() const
{
    return impl->getSize();
}

JSObject JSObject::getChild (int64 index) const
{
    jassert (isArray());
    return JSObject { impl->getChild (index) };
}

JSObject JSObject::operator[] (int64 index) const
{
    return getChild (index);
}

bool JSObject::hasProperty (const Identifier& name) const
{
    return impl->hasProperty (name);
}

var JSObject::get() const
{
    return impl->get();
}

void JSObject::setProperty (const Identifier& name, const var& value) const
{
    impl->setProperty (name, value);
}

void JSObject::setProperty (int64 index, const var& value) const
{
    impl->setProperty (index, value);
}

var JSObject::invokeMethod (const Identifier& methodName,
                            Span<const var> args,
                            Result* result) const
{
    const auto varOrError = impl->invokeMethod (methodName, args);

    if (result != nullptr)
    {
        const auto* e = std::get_if<String> (&varOrError);
        *result = e != nullptr ? Result::fail (*e) : Result::ok();
    }

    return detail::discardError (varOrError);
}

NamedValueSet JSObject::getProperties() const
{
    return impl->getProperties();
}

void JSObject::swap (JSObject& other) noexcept
{
    std::swap (impl, other.impl);
}

} // namespace higgs
