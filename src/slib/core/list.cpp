#include "../../../inc/slib/core/list.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_ROOT_OBJECT(IIteratorBase)

const char _List_ClassID[] = "list";

SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(Ref<Referable>)

SLIB_NAMESPACE_END
