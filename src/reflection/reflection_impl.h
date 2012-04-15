#ifndef REFLECTION_IMPL_H
#define REFLECTION_IMPL_H

#include "attribute.h"
#include "class.h"
#include "constructor.h"
#include "function.h"
#include "method.h"
#include "reflection.h"
#include <map>
#include <list>


#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UNIQUE TOKENPASTE2(Unique_, __LINE__)

class ClassRegistry {
public:

	const Class forName(const ::std::string& name) const;
	void registerClass(const ::std::string& name, const Class& c);

	static ClassRegistry& instance();

private:
	ClassRegistry() {}

	::std::map< ::std::string, Class > m_registry;
};

template<class Clazz>
struct ClassRegHelper {
	ClassRegHelper( const char* name ) {
		ClassRegistry::instance().registerClass(name, ClassOf<Clazz>());
	}
};


#define REFL_BEGIN_CLASS(CLASS_NAME) \
	static ClassRegHelper<CLASS_NAME> UNIQUE(#CLASS_NAME); \
template<> ClassImpl<CLASS_NAME>* ClassImpl<CLASS_NAME>::instance() {\
	typedef CLASS_NAME ThisClass;\
	static ClassImpl instance;\
	if (instance.open()) {\
		instance.setFqn(#CLASS_NAME);

#define REFL_END_CLASS \
	instance.close();\
}\
return &instance;\
}

#define REFL_SUPER_CLASS(CLASS_NAME) \
instance.registerSuperClassPriv(ClassOf<CLASS_NAME>());

#define REFL_METHOD(METHOD_NAME, RESULT, ...) \
	instance.registerMethodPriv(make_method<RESULT(ThisClass::*)(__VA_ARGS__)>(#METHOD_NAME, &ThisClass::METHOD_NAME, #RESULT, #__VA_ARGS__));

#define REFL_CONST_METHOD(METHOD_NAME, RESULT, ...) \
	instance.registerMethodPriv(make_method<RESULT(ThisClass::*)(__VA_ARGS__) const>(#METHOD_NAME, &ThisClass::METHOD_NAME, #RESULT, #__VA_ARGS__));

#define REFL_VOLATILE_METHOD(METHOD_NAME, RESULT, ...) \
	instance.registerMethodPriv(make_method<RESULT(ThisClass::*)(__VA_ARGS__) volatile>(#METHOD_NAME, &ThisClass::METHOD_NAME, #RESULT, #__VA_ARGS__));

#define REFL_CONST_VOLATILE_METHOD(METHOD_NAME, RESULT, ...) \
	instance.registerMethodPriv(make_method<RESULT(ThisClass::*)(__VA_ARGS__) const volatile>(#METHOD_NAME, &ThisClass::METHOD_NAME, #RESULT, #__VA_ARGS__));


#define REFL_STATIC_METHOD(METHOD_NAME, RESULT, ...) \
	instance.registerMethodPriv(make_static_method<ThisClass, RESULT(*)(__VA_ARGS__)>(#METHOD_NAME, &ThisClass::METHOD_NAME, #RESULT, #__VA_ARGS__));

#define REFL_ATTRIBUTE(ATTRIBUTE_NAME, TYPE_SPELLING) \
	instance.registerAttributePriv(make_attribute(#ATTRIBUTE_NAME, &ThisClass::ATTRIBUTE_NAME, #TYPE_SPELLING));

#define REFL_DEFAULT_CONSTRUCTOR(...) \
	instance.registerConstructorPriv(make_constructor<ThisClass>(""));

#define REFL_CONSTRUCTOR(...) \
	instance.registerConstructorPriv(make_constructor<ThisClass, __VA_ARGS__>(#__VA_ARGS__));


class FunctionRegistry {
public:

	const ::std::list<Function>& findFunction(const ::std::string& name) const;
	void registerFunction(const ::std::string& name, const Function& func);

	static FunctionRegistry& instance();

private:
	FunctionRegistry() {}

	::std::map< ::std::string, ::std::list<Function> > m_registry;
	const ::std::list<Function> emptyList;
};



template<class FuncType>
struct FuncRegHelper {
	FuncRegHelper( const char* name, FuncType f, const char* rString, const char* args ) {
		FunctionRegistry::instance().registerFunction(name, make_function<FuncType>(name, f, rString, args));
	}
};


#define REFL_FUNCTION(NAME, RESULT, ...) \
	static FuncRegHelper<RESULT (*)(__VA_ARGS__)> UNIQUE(#NAME, &NAME, #RESULT, #__VA_ARGS__);


#endif /* REFLECTION_IMPL_H */

