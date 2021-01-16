#include "convert.hh"
#include "core/tree_types.hh"
#include "core/compilation_errors.hh"
#include "utils/utils.hh"

#include <optional>
#include <stdlib.h>
#include <cassert>

using namespace latte::core;

namespace {

program::def_type convert_top_def(TopDef p);
std::vector<program::def_type> convert_list_top_def(ListTopDef listtopdef);
void convert_class_def(ClassDecl p, std::vector<class_def::def_type> &class_defs);
std::vector<class_def::def_type> convert_list_class_def(ListClassDecl listclassdecl);
arg convert_arg(Arg p);
std::vector<arg> convert_list_arg(ListArg listarg);
block::uptr convert_block(Block p);
std::vector<stmt::uptr> convert_list_stmt(ListStmt liststmt);
stmt::uptr convert_stmt(Stmt p);
item::uptr convert_item(Item p);
std::vector<item::uptr> convert_list_item(ListItem listitem);
type::single_type get_single_type(TypeSimple type);
type convert_type(Type p);
void convert_field(Field field, std::vector<nested_var::field_type> &ret);
std::vector<nested_var::field_type> convert_list_field(ListField listfield);
nested_var::base_type convert_field_to_base(nested_var::field_type field);
exp::uptr convert_expr(Expr p);
nested_var::field_type convert_expr_to_field(Expr exp);
std::vector<exp::uptr> convert_list_expr(ListExpr listexpr);
exp::binary::arithmetic_op_type convert_add_op(AddOp p);
exp::binary::arithmetic_op_type convert_mul_op(MulOp p);
exp::binary::rel_op_type convert_rel_op(RelOp p);

template<typename T>
void copy_elem_pos(tree_node *dst, T *src) {
	dst->pos = convert_bnfc_pos(src->line_number, src->char_number);
}

}

namespace latte::parser {

program::uptr convert_program(Program p) {
	auto ret = std::make_unique<program>();
	copy_elem_pos(ret.get(), p);
	switch (p->kind) {
	case Program_::is_Prog:
		ret->defs = convert_list_top_def(p->u.prog_.listtopdef_);
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Program!\n");
		exit(1);
	}
	return ret;
}

} // namespace latte::parser

namespace {

program::def_type convert_top_def(TopDef p) {
	switch (p->kind) {
	case TopDef_::is_FnDef: {
		auto ret = std::make_unique<func>();
		copy_elem_pos(ret.get(), p);
		ret->tp = convert_type(p->u.fndef_.type_);
		ret->id = ident(p->u.fndef_.ident_);
		ret->args = convert_list_arg(p->u.fndef_.listarg_);
		ret->bl = convert_block(p->u.fndef_.block_);
		return ret;
	}
	case TopDef_::is_ClassDef: {
		auto ret = std::make_unique<class_def>();
		copy_elem_pos(ret.get(), p);
		ret->class_id = ident(p->u.classdef_.ident_);
		ret->defs = convert_list_class_def(p->u.classdef_.classblock_->u.classblk_.listclassdecl_);
		return ret;
	}
	case TopDef_::is_ClassDefExt: {
		auto ret = std::make_unique<class_def>();
		copy_elem_pos(ret.get(), p);
		ret->class_id = ident(p->u.classdefext_.ident_1);
		ret->extends_id = ident(p->u.classdefext_.ident_2);
		ret->defs = convert_list_class_def(p->u.classdefext_.classblock_->u.classblk_.listclassdecl_);
		return ret;
	}
	default:
		fprintf(stderr, "Error: bad kind field when converting TopDef!\n");
		exit(1);
	}
}

std::vector<program::def_type> convert_list_top_def(ListTopDef listtopdef) {
	std::vector<program::def_type> ret;
	while (listtopdef != nullptr) {
		ret.emplace_back(convert_top_def(listtopdef->topdef_));
		listtopdef = listtopdef->listtopdef_;
	}
	return ret;
}

void convert_class_def(ClassDecl p, std::vector<class_def::def_type> &class_defs) {
	switch (p->kind) {
	case ClassDecl_::is_FieldDecl: {
		auto listident = p->u.fielddecl_.listident_;
		auto type = p->u.fielddecl_.type_;
		while (listident != nullptr) {
			class_def::field ret;
			copy_elem_pos(&ret, p);
			ret.id = ident(listident->ident_);
			ret.tp = convert_type(type);
			class_defs.emplace_back(std::move(ret));
			listident = listident->listident_;
		}
		break;
	}
	case ClassDecl_::is_MethodDecl: {
		auto ret = std::make_unique<func>();
		copy_elem_pos(ret.get(), p);
		ret->tp = convert_type(p->u.methoddecl_.type_);
		ret->id = ident(p->u.methoddecl_.ident_);
		ret->args = convert_list_arg(p->u.methoddecl_.listarg_);
		ret->bl = convert_block(p->u.methoddecl_.block_);
		class_defs.emplace_back(std::move(ret));
		break;
	}
	default:
		fprintf(stderr, "Error: bad kind field when converting ClassDef!\n");
		exit(1);
	}
}

std::vector<class_def::def_type> convert_list_class_def(ListClassDecl listclassdecl) {
	std::vector<class_def::def_type> ret;
	while (listclassdecl != nullptr) {
		convert_class_def(listclassdecl->classdecl_, ret);
		listclassdecl = listclassdecl->listclassdecl_;
	}
	return ret;
}

arg convert_arg(Arg p) {
	arg ret;
	copy_elem_pos(&ret, p);
	switch (p->kind) {
	case Arg_::is_Ar:
		ret.id = ident(p->u.ar_.ident_);
		ret.tp = convert_type(p->u.ar_.type_);
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Arg!\n");
		exit(1);
	}
	return ret;
}

std::vector<arg> convert_list_arg(ListArg listarg) {
	std::vector<arg> ret;
	while (listarg != nullptr) {
		ret.emplace_back(convert_arg(listarg->arg_));
		listarg = listarg->listarg_;
	}
	return ret;
}

block::uptr convert_block(Block p) {
	auto ret = std::make_unique<block>();
	copy_elem_pos(ret.get(), p);
	switch (p->kind) {
	case Block_::is_Blk:
		ret->stmts = convert_list_stmt(p->u.blk_.liststmt_);
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Block!\n");
		exit(1);
	}
	return ret;
}

std::vector<stmt::uptr> convert_list_stmt(ListStmt liststmt) {
	std::vector<stmt::uptr> ret;
	while (liststmt != nullptr) {
		ret.emplace_back(convert_stmt(liststmt->stmt_));
		liststmt = liststmt->liststmt_;
	}
	return ret;
}

stmt::uptr convert_stmt(Stmt p) {
	auto ret = std::make_unique<stmt>();
	copy_elem_pos(ret.get(), p);
	switch (p->kind) {
	case Stmt_::is_BStmt:
		ret->val = stmt::block {
			.bl = convert_block(p->u.bstmt_.block_),
		};
		break;
	case Stmt_::is_Decl:
		ret->val = stmt::decl {
			.tp = convert_type(p->u.decl_.type_),
			.items = convert_list_item(p->u.decl_.listitem_),
		};
		break;
	case Stmt_::is_Ass: {
		auto l = convert_expr(p->u.ass_.expr_1);
		if (!std::holds_alternative<nested_var>(l->val))
			throw compilation_error(convert_bnfc_pos(p->line_number, p->char_number),
				"non-assignable type");
		ret->val = stmt::ass {
			.v = std::move(std::get<nested_var>(l->val)),
			.e = convert_expr(p->u.ass_.expr_2),
		};
		break;
	}
	case Stmt_::is_Incr: {
		auto l = convert_expr(p->u.incr_.expr_);
		if (!std::holds_alternative<nested_var>(l->val))
			throw compilation_error(convert_bnfc_pos(p->line_number, p->char_number),
				"non-assignable type");
		ret->val = stmt::unary {
			.v = std::move(std::get<nested_var>(l->val)),
			.op = stmt::unary::op_type::INCR,
		};
		break;
	}
	case Stmt_::is_Decr: {
		auto l = convert_expr(p->u.incr_.expr_);
		if (!std::holds_alternative<nested_var>(l->val))
			throw compilation_error(convert_bnfc_pos(p->line_number, p->char_number),
				"non-assignable type");
		ret->val = stmt::unary {
			.v = std::move(std::get<nested_var>(l->val)),
			.op = stmt::unary::op_type::DECR,
		};
		break;
	}
	case Stmt_::is_Ret:
		ret->val = stmt::ret {
			.e = convert_expr(p->u.ret_.expr_),
		};
		break;
	case Stmt_::is_VRet:
		ret->val = stmt::ret {
			.e = std::nullopt,
		};
		break;
	case Stmt_::is_Cond:
		ret->val = stmt::cond {
			.e = convert_expr(p->u.cond_.expr_),
			.s_true = convert_stmt(p->u.cond_.stmt_),
			.s_false = std::nullopt,
		};
		break;
	case Stmt_::is_CondElse:
		ret->val = stmt::cond {
			.e = convert_expr(p->u.condelse_.expr_),
			.s_true = convert_stmt(p->u.condelse_.stmt_1),
			.s_false = convert_stmt(p->u.condelse_.stmt_2),
		};
		break;
	case Stmt_::is_While:
		ret->val = stmt::loop_while {
			.e = convert_expr(p->u.while_.expr_),
			.s = convert_stmt(p->u.while_.stmt_),
		};
		break;
	case Stmt_::is_For: {
		auto l = convert_expr(p->u.for_.expr_);
		if (!std::holds_alternative<nested_var>(l->val))
			throw compilation_error(convert_bnfc_pos(p->u.for_.expr_->line_number,
				p->u.for_.expr_->char_number),
				"invalid reference value type");
		ret->val = stmt::loop_for {
			.tp = get_single_type(p->u.for_.typesimple_),
			.id = ident(p->u.for_.ident_),
			.v = std::move(std::get<nested_var>(l->val)),
			.s = convert_stmt(p->u.for_.stmt_),
		};
		break;
	}
	case Stmt_::is_SExp:
		ret->val = stmt::exp {
			.e = convert_expr(p->u.sexp_.expr_),
		};
		break;
	case Stmt_::is_Empty:
		ret->val = stmt::empty {};
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Stmt!\n");
		exit(1);
	}
	return ret;
}

item::uptr convert_item(Item p) {
	auto ret = std::make_unique<item>();
	copy_elem_pos(ret.get(), p);
	switch (p->kind) {
	case Item_::is_NoInit:
		ret->id = ident(p->u.noinit_.ident_);
		ret->init_val = std::nullopt;
		break;
	case Item_::is_Init:
		ret->id = ident(p->u.init_.ident_);
		ret->init_val = convert_expr(p->u.init_.expr_);
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Item!\n");
		exit(1);
	}
	return ret;
}

std::vector<item::uptr> convert_list_item(ListItem listitem) {
	std::vector<item::uptr> ret;
	while (listitem != nullptr) {
		ret.emplace_back(convert_item(listitem->item_));
		listitem = listitem->listitem_;
	}
	return ret;
}


type::single_type get_single_type(TypeSimple type) {
	switch (type->kind) {
	case TypeSimple_::is_Int:
		return type::single_type {type::single_type::type_enum::INT};
	case TypeSimple_::is_Str:
		return type::single_type {type::single_type::type_enum::STR};
	case TypeSimple_::is_Bool:
		return type::single_type {type::single_type::type_enum::BOOL};
	case TypeSimple_::is_Class:
		return type::single_type {type::single_type::class_type {ident(type->u.class_.ident_)}};
	default:
		fprintf(stderr, "Error: bad kind field when converting Type!\n");
		exit(1);
	}
}

type convert_type(Type p) {
	type ret;
	copy_elem_pos(&ret, p);
	switch (p->kind) {
	case Type_::is_SingleType:
		ret.val = get_single_type(p->u.singletype_.typesimple_);
		break;
	case Type_::is_ArrayType:
		ret.val = type::array_type {type::single_type {get_single_type(p->u.singletype_.typesimple_)}};
		break;
	case Type_::is_Void:
		ret.val = type::single_type {type::single_type::type_enum::VOID};
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Type!\n");
		exit(1);
	}
	return ret;
}

void convert_field(Field field, std::vector<nested_var::field_type> &ret) {
	auto convert_field_val = [&](FieldVal field_val) {
		switch (field_val->kind) {
		case FieldVal_::is_FieldValCall: {
			nested_var::field_type f;
			f.val = func_call {
				.id = ident(field_val->u.fieldvalcall_.ident_),
				.params = convert_list_expr(field_val->u.fieldvalcall_.listexpr_)
			};
			copy_elem_pos(&f, field);
			ret.emplace_back(std::move(f));
			break;
		}
		case FieldVal_::is_FieldValSingle:
			nested_var::field_type f;
			f.val = field_val->u.fieldvalsingle_.ident_;
			copy_elem_pos(&f, field);
			ret.emplace_back(std::move(f));
			break;
		}
	};

	switch (field->kind) {
	case Field_::is_FieldSingle:
		convert_field_val(field->u.fieldsingle_.fieldval_);
		break;
	case Field_::is_FieldArray: {
		convert_field_val(field->u.fieldarray_.fieldval_);
		nested_var::field_type arr_pos;
		arr_pos.val = nested_var::array_pos {convert_expr(field->u.fieldarray_.expr_)};
		copy_elem_pos(&arr_pos, field);
		ret.emplace_back(std::move(arr_pos));
		break;
	}
	case Field_::is_FieldSelf: {
		nested_var::field_type self;
		self.val = nested_var::self {};
		copy_elem_pos(&self, field);
		ret.emplace_back(std::move(self));
		break;
	}
	default:
		fprintf(stderr, "Error: bad kind field when converting Type!\n");
		exit(1);
	}
}

std::vector<nested_var::field_type> convert_list_field(ListField listfield) {
	std::vector<nested_var::field_type> ret;
	while (listfield != nullptr) {
		convert_field(listfield->field_, ret);
		listfield = listfield->listfield_;
	}
	return ret;
}

nested_var::base_type convert_field_to_base(nested_var::field_type field) {
	nested_var::base_type ret;
	std::visit(overloaded {
		[&](func_call &f) {
			ret.val = std::move(f);
		},
		[&](nested_var::field_ident &f) {
			ret.val = std::move(f);
		},
		[&](nested_var::self &f) {
			ret.val = std::move(f);
		},
		[&](auto &) {
			assert(false);
		},
	}, field.val);
	ret.pos = field.pos;
	return ret;
}

exp::uptr convert_expr(Expr p) {
	auto ret = std::make_unique<exp>();
	copy_elem_pos(ret.get(), p);
	switch (p->kind) {
	case Expr_::is_ENewObject: {
		auto class_id = ident(p->u.enewobject_.ident_);
		nested_var::base_type base;
		base.val = new_val {
			.tp = type::single_type::class_type {std::move(class_id)},
			.arr_size = std::nullopt,
		};
		copy_elem_pos(&base, p);
		auto nest_var = nested_var {
			.base = std::move(base),
			.fields = {},
		};
		ret->val = std::move(nest_var);
		break;
	}
	case Expr_::is_ENewArray: {
		auto type = get_single_type(p->u.enewarray_.typesimple_);
		nested_var::base_type base;
		base.val = new_val {
			.tp = type::array_type {type},
			.arr_size = convert_expr(p->u.enewarray_.expr_),
		};
		copy_elem_pos(&base, p);
		auto nest_var = nested_var {
			.base = std::move(base),
			.fields = {},
		};
		ret->val = std::move(nest_var);
		break;
	}
	case Expr_::is_ETmpVar: {
		auto base_e = convert_expr(p->u.etmpvar_.expr_);
		if (!std::holds_alternative<nested_var>(base_e->val) ||
				!std::holds_alternative<new_val>(std::get<nested_var>(base_e->val).base.val))
			throw compilation_error(convert_bnfc_pos(p->line_number, p->char_number),
				"invalid tmp value type - only 'new ...' allowed here");
		ret->val = nested_var {
			.base = std::move(std::get<nested_var>(base_e->val).base),
			.fields = convert_list_field(p->u.etmpvar_.listfield_)
		};
		break;
	}
	case Expr_::is_ETmpArrayElem: {
		auto base_e = convert_expr(p->u.etmparrayelem_.expr_1);
		if (!std::holds_alternative<nested_var>(base_e->val) ||
				!std::holds_alternative<new_val>(std::get<nested_var>(base_e->val).base.val))
			throw compilation_error(convert_bnfc_pos(p->line_number, p->char_number),
				"invalid tmp value type - only 'new ...' allowed here");
		std::vector<nested_var::field_type> fields;
		fields.emplace_back(convert_expr_to_field(p->u.etmparrayelem_.expr_2));
		ret->val = nested_var {
			.base = std::move(std::get<nested_var>(base_e->val).base),
			.fields = std::move(fields)
		};
		break;
	}
	case Expr_::is_ETmpArrayElemVar: {
		auto base_e = convert_expr(p->u.etmparrayelemvar_.expr_1);
		if (!std::holds_alternative<nested_var>(base_e->val) ||
				!std::holds_alternative<new_val>(std::get<nested_var>(base_e->val).base.val))
			throw compilation_error(convert_bnfc_pos(p->line_number, p->char_number),
				"invalid tmp value type - only 'new ...' allowed here");
		// check base_e == new_val
		auto arr_pos = convert_expr_to_field(p->u.etmparrayelemvar_.expr_2);
		auto fields = convert_list_field(p->u.etmparrayelemvar_.listfield_);
		fields.insert(fields.begin(), std::move(arr_pos));
		ret->val = nested_var {
			.base = std::move(std::get<nested_var>(base_e->val).base),
			.fields = std::move(fields)
		};
		break;
	}
	case Expr_::is_ECastedNull: {
		if (p->u.ecastednull_.expr_->kind != Expr_::is_EVar ||
			p->u.ecastednull_.expr_->u.evar_.listfield_->field_->kind != Field_::is_FieldSingle ||
			p->u.ecastednull_.expr_->u.evar_.listfield_->field_->u.fieldsingle_.fieldval_->kind !=
				FieldVal_::is_FieldValSingle)
			throw compilation_error(convert_bnfc_pos(p->line_number, p->char_number), "invalid cast type");
		auto class_id = ident(p->u.ecastednull_
			.expr_->u.evar_
			.listfield_->field_->u.fieldsingle_
			.fieldval_->u.fieldvalsingle_.ident_);
		ret->val = exp::null_casted {type::single_type::class_type {
			std::move(class_id)
		}};
		break;
	}
	case Expr_::is_ECastedArrNull:
		ret->val = exp::null_casted {type::array_type {
			get_single_type(p->u.ecastedarrnull_.typesimple_)}
		};
		break;
	case Expr_::is_ENull:
		ret->val = exp::null {};
		break;
	case Expr_::is_EVar: {
		auto fields = convert_list_field(p->u.evar_.listfield_);
		auto base = convert_field_to_base(std::move(fields[0]));
		fields.erase(fields.begin());
		ret->val = nested_var {
			.base = std::move(base),
			.fields = std::move(fields)
		};
		break;
	}
	case Expr_::is_ELitInt:
		ret->val = p->u.elitint_.integer_;
		break;
	case Expr_::is_ELitTrue:
		ret->val = true;
		break;
	case Expr_::is_ELitFalse:
		ret->val = false;
		break;
	case Expr_::is_EString:
		ret->val = std::string(p->u.estring_.string_);
		break;
	case Expr_::is_Neg:
		ret->val = exp::unary {
			.e = convert_expr(p->u.neg_.expr_),
			.op = exp::unary::op_type::NEG,
		};
		break;
	case Expr_::is_Not:
		ret->val = exp::unary {
			.e = convert_expr(p->u.not_.expr_),
			.op = exp::unary::op_type::NOT,
		};
		break;
	case Expr_::is_EMul:
		ret->val = exp::binary {
			.e1 = convert_expr(p->u.emul_.expr_1),
			.e2 = convert_expr(p->u.emul_.expr_2),
			.op = convert_mul_op(p->u.emul_.mulop_),
		};
		break;
	case Expr_::is_EAdd:
		ret->val = exp::binary {
			.e1 = convert_expr(p->u.eadd_.expr_1),
			.e2 = convert_expr(p->u.eadd_.expr_2),
			.op = convert_add_op(p->u.eadd_.addop_),
		};
		break;
	case Expr_::is_ERel:
		ret->val = exp::binary {
			.e1 = convert_expr(p->u.erel_.expr_1),
			.e2 = convert_expr(p->u.erel_.expr_2),
			.op = convert_rel_op(p->u.erel_.relop_),
		};
		break;
	case Expr_::is_EAnd:
		ret->val = exp::binary {
			.e1 = convert_expr(p->u.eand_.expr_1),
			.e2 = convert_expr(p->u.eand_.expr_2),
			.op = exp::binary::bool_op_type::AND,
		};
		break;
	case Expr_::is_EOr:
		ret->val = exp::binary {
			.e1 = convert_expr(p->u.eor_.expr_1),
			.e2 = convert_expr(p->u.eor_.expr_2),
			.op = exp::binary::bool_op_type::OR,
		};
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Expr!\n");
		exit(1);
	}
	return ret;
}

nested_var::field_type convert_expr_to_field(Expr exp) {
	nested_var::field_type ret;
	copy_elem_pos(&ret, exp);
	ret.val = convert_expr(exp);
	return ret;
}

std::vector<exp::uptr> convert_list_expr(ListExpr listexpr) {
	std::vector<exp::uptr> ret;
	while (listexpr != nullptr) {
		ret.emplace_back(convert_expr(listexpr->expr_));
		listexpr = listexpr->listexpr_;
	}
	return ret;
}

exp::binary::arithmetic_op_type convert_add_op(AddOp p) {
	switch (p->kind) {
	case AddOp_::is_Plus:
		return exp::binary::arithmetic_op_type::ADD;
	case AddOp_::is_Minus:
		return exp::binary::arithmetic_op_type::SUB;
	default:
		fprintf(stderr, "Error: bad kind field when converting AddOp!\n");
		exit(1);
	}
}

exp::binary::arithmetic_op_type convert_mul_op(MulOp p) {
	switch (p->kind) {
	case MulOp_::is_Times:
		return exp::binary::arithmetic_op_type::MUL;
	case MulOp_::is_Div:
		return exp::binary::arithmetic_op_type::DIV;
	case MulOp_::is_Mod:
		return exp::binary::arithmetic_op_type::MOD;
	default:
		fprintf(stderr, "Error: bad kind field when converting MulOp!\n");
		exit(1);
	}
}

exp::binary::rel_op_type convert_rel_op(RelOp p) {
	switch (p->kind) {
	case RelOp_::is_LTH:
		return exp::binary::rel_op_type::LTH;
	case RelOp_::is_LE:
		return exp::binary::rel_op_type::LE;
	case RelOp_::is_GTH:
		return exp::binary::rel_op_type::GTH;
	case RelOp_::is_GE:
		return exp::binary::rel_op_type::GE;
	case RelOp_::is_EQU:
		return exp::binary::rel_op_type::EQU;
	case RelOp_::is_NE:
		return exp::binary::rel_op_type::NE;
	default:
		fprintf(stderr, "Error: bad kind field when converting RelOp!\n");
		exit(1);
	}
}

}
