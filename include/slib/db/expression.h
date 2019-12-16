/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_DB_DATABASE_EXPR
#define CHECKHEADER_SLIB_DB_DATABASE_EXPR

#include "definition.h"

#include "constants.h"

#include "../core/object.h"
#include "../core/variant.h"

namespace slib
{

	class SqlBuilder;

	class SLIB_EXPORT CDatabaseExpression : public Referable
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		CDatabaseExpression();
		
		~CDatabaseExpression();
		
	public:
		virtual void appendTo(SqlBuilder& buf, sl_bool flagParentheses) = 0;

	};

	class DatabaseExpression;

	template <>
	class SLIB_EXPORT Atomic<DatabaseExpression>
	{
	public:
		AtomicRef<CDatabaseExpression> ref;
		SLIB_ATOMIC_REF_WRAPPER_NO_OP(CDatabaseExpression)
		
	};

	class SLIB_EXPORT DatabaseExpression
	{
	public:
		Ref<CDatabaseExpression> ref;
		SLIB_REF_WRAPPER_NO_OP(DatabaseExpression, CDatabaseExpression)
		
	public:
		DatabaseExpression(const Variant& value);

	public:
		void appendTo(SqlBuilder& buf, sl_bool flagParentheses) const;

	public:
		static DatabaseExpression valueOf(const Variant& value);
		
		static DatabaseExpression column(const String& name);
		
		static DatabaseExpression column(const String& name1, const String& name2);

		static DatabaseExpression raw(const String& expr, sl_bool flagParentheses = sl_true);

		static const DatabaseExpression& parameter();
		
		static const DatabaseExpression& parameter(const String& name);
		
		static const DatabaseExpression& count();
		
		static DatabaseExpression function(const String& name);
		
		static DatabaseExpression function(const String& name, const ListParam<DatabaseExpression>& params);
		
		template <class... ARGS>
		DatabaseExpression function(const String& name, const DatabaseExpression& param, ARGS&&... args)
		{
			DatabaseExpression params[] = { param, Forward<ARGS>(args)... };
			return Fn(name, params);
		}
		
		static DatabaseExpression binaryOp(const String& op, DatabaseExpression& e1, DatabaseExpression& e2);
		
		static DatabaseExpression unaryOp(const String& op, DatabaseExpression& e);
		
		static DatabaseExpression unaryOpSuffix(const String& op, DatabaseExpression& e);

	};

	DatabaseExpression operator&&(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator||(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator==(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator!=(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator>(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator<(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator>=(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator<=(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator+(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator-(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator*(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator/(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator%(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression operator-(const DatabaseExpression& e);
	DatabaseExpression operator!(const DatabaseExpression& e);
	DatabaseExpression IsNull(const DatabaseExpression& e);
	DatabaseExpression IsNotNull(const DatabaseExpression& e);
	DatabaseExpression Like(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression NotLike(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression ILike(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression NotILike(const DatabaseExpression& e1, const DatabaseExpression& e2);
	DatabaseExpression In(const DatabaseExpression& e, const ListParam<DatabaseExpression>& params);
	template <class... ARGS>
	DatabaseExpression In(const DatabaseExpression& e, const DatabaseExpression& param, ARGS&&... args)
	{
		DatabaseExpression params[] = { param, Forward<ARGS>(args)... };
		return In(e, params);
	}
	DatabaseExpression NotIn(const DatabaseExpression& e, const ListParam<DatabaseExpression>& params);
	template <class... ARGS>
	DatabaseExpression NotIn(const DatabaseExpression& e, const DatabaseExpression& param, ARGS&&... args)
	{
		DatabaseExpression params[] = { param, Forward<ARGS>(args)... };
		return NotIn(e, params);
	}
	DatabaseExpression Concat(const ListParam<DatabaseExpression>& params);
	template <class... ARGS>
	DatabaseExpression Concat(const DatabaseExpression& param1, const DatabaseExpression& param2, ARGS&&... args)
	{
		DatabaseExpression params[] = { param1, param2, Forward<ARGS>(args)... };
		return Concat(params);
	}
	DatabaseExpression Abs(const DatabaseExpression& e);

	DatabaseExpression Max(const DatabaseExpression& e);
	DatabaseExpression Min(const DatabaseExpression& e);
	DatabaseExpression Count(const DatabaseExpression& e);
	DatabaseExpression Sum(const DatabaseExpression& e);
	DatabaseExpression Avg(const DatabaseExpression& e);
	DatabaseExpression GroupConcat(const DatabaseExpression& e, const String& seperator = sl_null);

}

#endif
