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

#include "slib/db/expression.h"

#include "slib/db/sql.h"
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_DEFINE_ROOT_OBJECT(CDatabaseExpression)

	namespace priv
	{
		namespace db_expr
		{
		
			enum class BinaryOp
			{
				NONE,
				AND,
				OR,
				EQUAL,
				NOT_EQUAL,
				GT,
				LT,
				GTE,
				LTE,
				PLUS,
				MINUS,
				MULTIPLY,
				DIVIDE,
				MOD,
				LIKE,
				NOT_LIKE,
				ILIKE,
				NOT_ILIKE,
				IN,
				NOT_IN,
				GROUP_CONCAT
			};
		
			enum class UnaryOp
			{
				MINUS,
				NOT,
				IS_NULL,
				IS_NOT_NULL,
				ABS,
				MAX,
				MIN,
				COUNT,
				SUM,
				AVG
			};
		
			class ValueExpression : public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				Variant value;
				
			public:
				ValueExpression(const Variant& _value): value(_value)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					switch (value.getType()) {
						case VariantType::Null:
							builder.appendStatic("null");
							break;
						case VariantType::Int32:
						case VariantType::Uint32:
						case VariantType::Int64:
						case VariantType::Uint64:
						case VariantType::Float:
						case VariantType::Double:
						case VariantType::Boolean:
							builder.append(value.getString());
							break;
						case VariantType::Time:
						case VariantType::String8:
						case VariantType::Sz8:
							builder.append(ParseUtil::applyBackslashEscapes(value.getString(), sl_false));
						case VariantType::String16:
						case VariantType::Sz16:
							builder.append(ParseUtil::applyBackslashEscapes(value.getString16(), sl_false));
						default:
							builder.appendStatic("null");
							break;
					}
				}
				
			};
		
			SLIB_DEFINE_OBJECT(ValueExpression, CDatabaseExpression)
			
			class IdentifierExpression : public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				String name;
				
			public:
				IdentifierExpression(const String& _name): name(_name)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					builder.appendIdentifier(name);
				}
				
			};
			
			SLIB_DEFINE_OBJECT(IdentifierExpression, CDatabaseExpression)
			
			class IdentifierExpression2 : public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				String name1;
				String name2;
				
			public:
				IdentifierExpression2(const String& _name1, const String& _name2): name1(_name1), name2(_name2)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					if (name1.isNotEmpty()) {
						builder.appendIdentifier(name1);
						builder.appendStatic(".");
					}
					builder.appendIdentifier(name2);
				}
				
			};
			
			SLIB_DEFINE_OBJECT(IdentifierExpression2, CDatabaseExpression)
			
			class RawExpression : public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				String expr;
				sl_bool flagSupportParentheses;
				
			public:
				RawExpression(const String& _expr, sl_bool _flagParentheses): expr(_expr), flagSupportParentheses(_flagParentheses)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					if (flagParentheses && flagSupportParentheses) {
						builder.appendStatic("(");
						builder.append(expr);
						builder.appendStatic(")");
					} else {
						builder.append(expr);
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(RawExpression, CDatabaseExpression)
			
			class ParameterExpression : public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				String name;
				
			public:
				ParameterExpression()
				{
				}
				
				ParameterExpression(const String& _name): name(_name)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					builder.appendParameter(name);
				}
				
			};
			
			SLIB_DEFINE_OBJECT(ParameterExpression, CDatabaseExpression)
			
			class CountAllExpression : public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					builder.appendStatic("COUNT(*)");
				}
				
			};

			SLIB_DEFINE_OBJECT(CountAllExpression, CDatabaseExpression)
		
			class BinaryOpExpression: public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				BinaryOp op;
				DatabaseExpression expr1;
				DatabaseExpression expr2;
				
			public:
				BinaryOpExpression(BinaryOp _op, const DatabaseExpression& _expr1, const DatabaseExpression& _expr2): op(_op), expr1(_expr1), expr2(_expr2)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					sl_uint32 lenOp = 0;
					const char* szOp = sl_null;
					sl_bool flagFunction = sl_false;
					switch (op) {
						case BinaryOp::AND:
							szOp = "AND";
							lenOp = 3;
							break;
						case BinaryOp::OR:
							szOp = "OR";
							lenOp = 2;
							break;
						case BinaryOp::EQUAL:
							szOp = "=";
							lenOp = 1;
							break;
						case BinaryOp::NOT_EQUAL:
							szOp = "!=";
							lenOp = 2;
							break;
						case BinaryOp::GT:
							szOp = ">";
							lenOp = 1;
							break;
						case BinaryOp::LT:
							szOp = "<";
							lenOp = 1;
							break;
						case BinaryOp::GTE:
							szOp = ">=";
							lenOp = 2;
							break;
						case BinaryOp::LTE:
							szOp = "<=";
							lenOp = 2;
							break;
						case BinaryOp::PLUS:
							szOp = "+";
							lenOp = 1;
							break;
						case BinaryOp::MINUS:
							szOp = "-";
							lenOp = 1;
							break;
						case BinaryOp::MULTIPLY:
							szOp = "*";
							lenOp = 1;
							break;
						case BinaryOp::DIVIDE:
							szOp = "/";
							lenOp = 1;
							break;
						case BinaryOp::MOD:
							szOp = "%";
							lenOp = 1;
							break;
						case BinaryOp::LIKE:
							szOp = "LIKE";
							lenOp = 4;
							break;
						case BinaryOp::NOT_LIKE:
							szOp = "NOT LIKE";
							lenOp = 8;
							break;
						case BinaryOp::ILIKE:
							if (builder.dialect == DatabaseDialect::PostgreSQL) {
								szOp = "ILIKE";
								lenOp = 5;
							} else {
								szOp = "LIKE";
								lenOp = 4;
							}
							break;
						case BinaryOp::NOT_ILIKE:
							if (builder.dialect == DatabaseDialect::PostgreSQL) {
								szOp = "NOT ILIKE";
								lenOp = 9;
							} else {
								szOp = "NOT LIKE";
								lenOp = 8;
							}
							break;
						case BinaryOp::IN:
							szOp = "IN";
							lenOp = 2;
							break;
						case BinaryOp::NOT_IN:
							szOp = "NOT IN";
							lenOp = 6;
							break;
						case BinaryOp::GROUP_CONCAT:
							flagFunction = sl_true;
							if (builder.dialect == DatabaseDialect::PostgreSQL) {
								szOp = "string_agg";
								lenOp = 10;
							} else {
								szOp = "GROUP_CONCAT";
								lenOp = 12;
							}
							break;
						case BinaryOp::NONE:
							break;
					}
					if (!lenOp) {
						return;
					}
					if (flagFunction) {
						builder.appendStatic(szOp, lenOp);
						builder.appendStatic("(");
						expr1.appendTo(builder, sl_false);
						builder.appendStatic(", ");
						expr2.appendTo(builder, sl_false);
						builder.appendStatic(")");
					} else {
						if (flagParentheses) {
							builder.appendStatic("(");
						}
						sl_bool flagSubParentheses1 = sl_true;
						sl_bool flagSubParentheses2 = sl_true;
						BinaryOp binarySubOp1 = BinaryOp::NONE;
						BinaryOp binarySubOp2 = BinaryOp::NONE;
						if (IsInstanceOf<BinaryOpExpression>(expr1.ref.get())) {
							binarySubOp1 = ((BinaryOpExpression*)(expr1.ref.get()))->op;
						}
						if (IsInstanceOf<BinaryOpExpression>(expr2.ref.get())) {
							binarySubOp2 = ((BinaryOpExpression*)(expr2.ref.get()))->op;
						}
						switch (op) {
							case BinaryOp::PLUS:
							case BinaryOp::MINUS:
								if (binarySubOp1 == BinaryOp::PLUS || binarySubOp1 == BinaryOp::MINUS) {
									flagSubParentheses1 = sl_false;
								}
								break;
							case BinaryOp::MULTIPLY:
							case BinaryOp::DIVIDE:
								if (binarySubOp1 == BinaryOp::MULTIPLY || binarySubOp1 == BinaryOp::DIVIDE) {
									flagSubParentheses1 = sl_false;
								}
								break;
							case BinaryOp::AND:
								if (binarySubOp1 == BinaryOp::AND || (binarySubOp1 >= BinaryOp::EQUAL && binarySubOp1 <= BinaryOp::LTE)) {
									flagSubParentheses1 = sl_false;
								}
								if (binarySubOp2 == BinaryOp::AND || (binarySubOp2 >= BinaryOp::EQUAL && binarySubOp2 <= BinaryOp::LTE)) {
									flagSubParentheses2 = sl_false;
								}
								break;
							case BinaryOp::OR:
								if (binarySubOp1 == BinaryOp::OR || (binarySubOp1 >= BinaryOp::EQUAL && binarySubOp1 <= BinaryOp::LTE)) {
									flagSubParentheses1 = sl_false;
								}
								if (binarySubOp2 == BinaryOp::OR || (binarySubOp2 >= BinaryOp::EQUAL && binarySubOp2 <= BinaryOp::LTE)) {
									flagSubParentheses2 = sl_false;
								}
								break;
							default:
								break;
						}
						expr1.appendTo(builder, flagSubParentheses1);
						builder.appendStatic(" ");
						builder.appendStatic(szOp, lenOp);
						builder.appendStatic(" ");
						expr2.appendTo(builder, flagSubParentheses2);
						if (flagParentheses) {
							builder.appendStatic(")");
						}
					}
				}
				
			};
	
			SLIB_DEFINE_OBJECT(BinaryOpExpression, CDatabaseExpression)

			class CustomBinaryOpExpression: public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				String op;
				DatabaseExpression expr1;
				DatabaseExpression expr2;
				
			public:
				CustomBinaryOpExpression(const String& _op, const DatabaseExpression& _expr1, const DatabaseExpression& _expr2): op(_op), expr1(_expr1), expr2(_expr2)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					if (flagParentheses) {
						builder.appendStatic("(");
					}
					expr1.appendTo(builder, sl_true);
					builder.appendStatic(" ");
					builder.append(op);
					builder.appendStatic(" ");
					expr2.appendTo(builder, sl_true);
					if (flagParentheses) {
						builder.appendStatic(")");
					}
				}
				
			};
	
			SLIB_DEFINE_OBJECT(CustomBinaryOpExpression, CDatabaseExpression)
		
			class UnaryOpExpression: public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				UnaryOp op;
				DatabaseExpression expr;
				
			public:
				UnaryOpExpression(UnaryOp _op, const DatabaseExpression& _expr): op(_op), expr(_expr)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					sl_bool flagFunction = sl_false;
					sl_bool flagSuffix = sl_false;
					const char* szOp = sl_null;
					sl_uint32 lenOp = 0;
					switch (op) {
						case UnaryOp::MINUS:
							szOp = "-";
							lenOp = 1;
							break;
						case UnaryOp::NOT:
							szOp = "NOT ";
							lenOp = 4;
							break;
						case UnaryOp::IS_NULL:
							szOp = " IS NULL";
							lenOp = 8;
							flagSuffix = sl_true;
							break;
						case UnaryOp::IS_NOT_NULL:
							szOp = " IS NOT NULL";
							lenOp = 12;
							flagSuffix = sl_true;
							break;
						case UnaryOp::ABS:
							szOp = "ABS";
							lenOp = 3;
							flagFunction = sl_true;
							break;
						case UnaryOp::MAX:
							szOp = "MAX";
							lenOp = 3;
							flagFunction = sl_true;
							break;
						case UnaryOp::MIN:
							szOp = "MIN";
							lenOp = 3;
							flagFunction = sl_true;
							break;
						case UnaryOp::COUNT:
							szOp = "COUNT";
							lenOp = 5;
							flagFunction = sl_true;
							break;
						case UnaryOp::SUM:
							szOp = "SUM";
							lenOp = 3;
							flagFunction = sl_true;
							break;
						case UnaryOp::AVG:
							szOp = "AVG";
							lenOp = 3;
							flagFunction = sl_true;
							break;
					}
					if (!lenOp) {
						return;
					}
					if (flagFunction) {
						builder.appendStatic(szOp, lenOp);
						builder.appendStatic("(");
						expr.appendTo(builder, sl_false);
						builder.appendStatic(")");
					} else {
						if (flagParentheses) {
							builder.appendStatic("(");
						}
						if (flagSuffix) {
							expr.appendTo(builder, sl_true);
							builder.appendStatic(szOp, lenOp);
						} else {
							builder.appendStatic(szOp, lenOp);
							expr.appendTo(builder, sl_true);
						}
						if (flagParentheses) {
							builder.appendStatic(")");
						}
					}
				}
				
			};
	
			SLIB_DEFINE_OBJECT(UnaryOpExpression, CDatabaseExpression)

			class CustomUnaryOpExpression: public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				String op;
				sl_bool flagSuffix;
				DatabaseExpression expr;
				
			public:
				CustomUnaryOpExpression(const String& _op, sl_bool _flagSuffix, const DatabaseExpression& _expr): op(_op), flagSuffix(_flagSuffix), expr(_expr)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					if (flagParentheses) {
						builder.appendStatic("(");
					}
					if (flagSuffix) {
						expr.appendTo(builder, sl_true);
						builder.appendStatic(" ");
						builder.append(op);
					} else {
						builder.append(op);
						builder.appendStatic(" ");
						expr.appendTo(builder, sl_true);
					}
					if (flagParentheses) {
						builder.appendStatic(")");
					}
				}
				
			};
	
			SLIB_DEFINE_OBJECT(CustomUnaryOpExpression, CDatabaseExpression)

			class FunctionExpression: public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				String name;
				List<DatabaseExpression> params;
				
			public:
				FunctionExpression(const String& _name, const List<DatabaseExpression> _params): name(_name), params(_params)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					builder.append(name);
					builder.appendStatic("(");
					ListLocker<DatabaseExpression> items(params);
					for (sl_size i = 0; i < items.count; i++) {
						if (i) {
							builder.appendStatic(", ");
						}
						items[i].appendTo(builder, sl_false);
					}
					builder.appendStatic(")");
				}
				
			};
	
			SLIB_DEFINE_OBJECT(FunctionExpression, CDatabaseExpression)

			class SetExpression: public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				List<DatabaseExpression> elements;
				
			public:
				SetExpression(const List<DatabaseExpression> _elements): elements(_elements)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					builder.appendStatic("(");
					ListLocker<DatabaseExpression> items(elements);
					for (sl_size i = 0; i < items.count; i++) {
						if (i) {
							builder.appendStatic(", ");
						}
						items[i].appendTo(builder, sl_false);
					}
					builder.appendStatic(")");
				}
				
			};
	
			SLIB_DEFINE_OBJECT(SetExpression, CDatabaseExpression)

			class ConcatExpression: public CDatabaseExpression
			{
				SLIB_DECLARE_OBJECT
				
			public:
				List<DatabaseExpression> elements;
				
			public:
				ConcatExpression(const List<DatabaseExpression> _elements): elements(_elements)
				{
				}
				
			public:
				void appendTo(SqlBuilder& builder, sl_bool flagParentheses) override
				{
					ListLocker<DatabaseExpression> items(elements);
					if (items.count == 0) {
						return;
					}
					if (items.count == 1) {
						items[0].appendTo(builder, sl_true);
						return;
					}
					if (builder.dialect == DatabaseDialect::MySQL) {
						builder.appendStatic("CONCAT(");
						for (sl_size i = 0; i < items.count; i++) {
							if (i) {
								builder.appendStatic(", ");
							}
							items[i].appendTo(builder, sl_false);
						}
						builder.appendStatic(")");
					} else {
						if (flagParentheses) {
							builder.appendStatic("(");
						}
						for (sl_size i = 0; i < items.count; i++) {
							if (i) {
								builder.appendStatic(" || ");
							}
							items[i].appendTo(builder, sl_true);
						}
						if (flagParentheses) {
							builder.appendStatic(")");
						}
					}
				}
				
			};
	
			SLIB_DEFINE_OBJECT(ConcatExpression, CDatabaseExpression)

		}
	}

	using namespace priv::db_expr;

	CDatabaseExpression::CDatabaseExpression()
	{
	}

	CDatabaseExpression::~CDatabaseExpression()
	{
	}


	DatabaseExpression::DatabaseExpression(const Variant& value): ref(new ValueExpression(value))
	{
	}
	
	void DatabaseExpression::appendTo(SqlBuilder& builder, sl_bool flagParentheses) const
	{
		CDatabaseExpression* obj = ref.get();
		if (obj) {
			obj->appendTo(builder, flagParentheses);
		} else {
			builder.appendStatic("null");
		}
	}

	DatabaseExpression DatabaseExpression::valueOf(const Variant& value)
	{
		return new ValueExpression(value);
	}

	DatabaseExpression DatabaseExpression::column(const String& name)
	{
		return new IdentifierExpression(name);
	}

	DatabaseExpression DatabaseExpression::column(const String& name1, const String& name2)
	{
		return new IdentifierExpression2(name1, name2);
	}

	DatabaseExpression DatabaseExpression::raw(const String& expr, sl_bool flagParentheses)
	{
		return new RawExpression(expr, flagParentheses);
	}

	const DatabaseExpression& DatabaseExpression::parameter()
	{
		SLIB_SAFE_STATIC(DatabaseExpression, expr, new ParameterExpression)
		return expr;
	}

	const DatabaseExpression& DatabaseExpression::parameter(const String& name)
	{
		SLIB_SAFE_STATIC(DatabaseExpression, expr, new ParameterExpression(name))
		return expr;
	}

	const DatabaseExpression& DatabaseExpression::count()
	{
		SLIB_SAFE_STATIC(DatabaseExpression, expr, new CountAllExpression)
		return expr;
	}

	DatabaseExpression DatabaseExpression::function(const String& name)
	{
		return function(name, sl_null);
	}

	DatabaseExpression DatabaseExpression::function(const String& name, const ListParam<DatabaseExpression>& params)
	{
		return new FunctionExpression(name, params.toList());
	}

	DatabaseExpression DatabaseExpression::binaryOp(const String& op, DatabaseExpression& e1, DatabaseExpression& e2)
	{
		return new CustomBinaryOpExpression(op, e1, e2);
	}

	DatabaseExpression DatabaseExpression::unaryOp(const String& op, DatabaseExpression& e)
	{
		return new CustomUnaryOpExpression(op, sl_false, e);
	}

	DatabaseExpression DatabaseExpression::unaryOpSuffix(const String& op, DatabaseExpression& e)
	{
		return new CustomUnaryOpExpression(op, sl_true, e);
	}

	DatabaseExpression operator&&(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::AND, e1, e2);
	}

	DatabaseExpression operator||(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::OR, e1, e2);
	}

	DatabaseExpression operator==(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::EQUAL, e1, e2);
	}

	DatabaseExpression operator!=(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::NOT_EQUAL, e1, e2);
	}

	DatabaseExpression operator>(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::GT, e1, e2);
	}

	DatabaseExpression operator<(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::LT, e1, e2);
	}

	DatabaseExpression operator>=(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::GTE, e1, e2);
	}

	DatabaseExpression operator<=(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::LTE, e1, e2);
	}

	DatabaseExpression operator+(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::PLUS, e1, e2);
	}

	DatabaseExpression operator-(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::MINUS, e1, e2);
	}

	DatabaseExpression operator*(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::MULTIPLY, e1, e2);
	}

	DatabaseExpression operator/(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::DIVIDE, e1, e2);
	}

	DatabaseExpression operator%(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::MOD, e1, e2);
	}

	DatabaseExpression operator-(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::MINUS, e);
	}

	DatabaseExpression operator!(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::NOT, e);
	}

	DatabaseExpression IsNull(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::IS_NULL, e);
	}

	DatabaseExpression IsNotNull(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::IS_NOT_NULL, e);
	}

	DatabaseExpression Like(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::LIKE, e1, e2);
	}

	DatabaseExpression NotLike(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::NOT_LIKE, e1, e2);
	}

	DatabaseExpression ILike(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::ILIKE, e1, e2);
	}

	DatabaseExpression NotILike(const DatabaseExpression& e1, const DatabaseExpression& e2)
	{
		return new BinaryOpExpression(BinaryOp::NOT_ILIKE, e1, e2);
	}

	DatabaseExpression In(const DatabaseExpression& e, const ListParam<DatabaseExpression>& params)
	{
		return new BinaryOpExpression(BinaryOp::IN, e, new SetExpression(params.toList()));
	}

	DatabaseExpression NotIn(const DatabaseExpression& e, const ListParam<DatabaseExpression>& params)
	{
		return new BinaryOpExpression(BinaryOp::NOT_IN, e, new SetExpression(params.toList()));
	}

	DatabaseExpression Concat(const ListParam<DatabaseExpression>& params)
	{
		return new ConcatExpression(params.toList());
	}

	DatabaseExpression Abs(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::ABS, e);
	}

	DatabaseExpression Max(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::MAX, e);
	}

	DatabaseExpression Min(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::MIN, e);
	}

	DatabaseExpression Count(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::COUNT, e);
	}

	DatabaseExpression Sum(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::SUM, e);
	}

	DatabaseExpression Avg(const DatabaseExpression& e)
	{
		return new UnaryOpExpression(UnaryOp::AVG, e);
	}

	DatabaseExpression GroupConcat(const DatabaseExpression& e, const String& seperator)
	{
		return new BinaryOpExpression(BinaryOp::GROUP_CONCAT, e, DatabaseExpression::valueOf(seperator));
	}

}
