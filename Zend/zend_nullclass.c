#include "zend.h"
#include "zend_API.h"
#include "zend_nullclass.h"

zend_class_entry *zend_nullclass_def = NULL;

ZEND_BEGIN_ARG_INFO(arginfo_nullclass_call, 0)
	ZEND_ARG_INFO(0, unused0)
	ZEND_ARG_INFO(0, unused1)
ZEND_END_ARG_INFO()

ZEND_METHOD(nullclass, __call)
{
	RETURN_NULL();
}

const static zend_function_entry default_nullclass_functions[] = {
	ZEND_ME(nullclass, __call, arginfo_nullclass_call, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void zend_register_default_nullclass(TSRMLS_D)
{
	zend_class_entry ce;

	/* Call this "0_nullclass" since that's a parse error to refer to from
	 * external code, but still allows us to directly instantiate the class
	 * internally. I guess ReflectionClass will still pick it up, but I don't
	 * see a better way to hide it. */
	INIT_CLASS_ENTRY(ce, "0_nullclass", default_nullclass_functions);
	zend_nullclass_def = zend_register_internal_class(&ce TSRMLS_CC);
}
