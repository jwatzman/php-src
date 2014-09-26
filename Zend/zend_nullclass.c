#include "zend.h"
#include "zend_API.h"
#include "zend_nullclass.h"

static zend_class_entry *zend_nullclass_ce;
static zval zend_nullclass;
static int zend_nullclass_init = 0;

ZEND_METHOD(nullclass, __call)
{
	RETURN_NULL();
}

const static zend_function_entry default_nullclass_functions[] = {
	ZEND_ME(nullclass, __call, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void zend_register_default_nullclass(TSRMLS_D) /* {{{ */
{
	zend_class_entry foo;
	INIT_CLASS_ENTRY(foo, "NullClass", default_nullclass_functions);
	zend_nullclass_ce = zend_register_internal_class(&foo TSRMLS_CC);
}
/* }}} */

void zend_get_nullclass(zval *z)
{
	/*
	if (!zend_nullclass_init)
	{
		zend_nullclass_init = 1;
		object_and_properties_init(&zend_nullclass, &zend_nullclass_ce, NULL);
	}

	return &zend_nullclass;
	*/
	object_and_properties_init(z, zend_nullclass_ce, NULL);
}
