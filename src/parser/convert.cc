#include "convert.hh"

#include <stdlib.h>
#include <stdio.h>

using namespace latte::core;

namespace {

func::uptr convert_top_def(TopDef p);
std::vector<func::uptr> convert_list_top_def(ListTopDef listtopdef);
arg convert_arg(Arg p);
std::vector<arg> convert_list_arg(ListArg listarg);
block::uptr convert_block(Block p);
std::vector<stmt::uptr> convert_list_stmt(ListStmt liststmt);
stmt::uptr convert_stmt(Stmt p);
item::uptr convert_item(Item p);
std::vector<item::uptr> convert_list_item(ListItem listitem);
type convert_type(Type p);
exp::uptr convert_expr(Expr p);
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
		ret->funcs = convert_list_top_def(p->u.prog_.listtopdef_);
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting Program!\n");
		exit(1);
	}
	return ret;
}

} // namespace latte::parser

namespace {

func::uptr convert_top_def(TopDef p) {
	auto ret = std::make_unique<func>();
	copy_elem_pos(ret.get(), p);
	switch (p->kind) {
	case TopDef_::is_FnDef:
		ret->tp = convert_type(p->u.fndef_.type_);
		ret->id = ident(p->u.fndef_.ident_);
		ret->args = convert_list_arg(p->u.fndef_.listarg_);
		ret->bl = convert_block(p->u.fndef_.block_);
		break;
	default:
		fprintf(stderr, "Error: bad kind field when converting TopDef!\n");
		exit(1);
	}
	return ret;
}

std::vector<func::uptr> convert_list_top_def(ListTopDef listtopdef) {
	std::vector<func::uptr> ret;
	while (listtopdef != nullptr) {
		ret.emplace_back(convert_top_def(listtopdef->topdef_));
		listtopdef = listtopdef->listtopdef_;
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
	case Stmt_::is_Ass:
		ret->val = stmt::ass {
			.id = ident(p->u.ass_.ident_),
			.e = convert_expr(p->u.ass_.expr_),
		};
		break;
	case Stmt_::is_Incr:
		ret->val = stmt::unary {
			.id = ident(p->u.incr_.ident_),
			.op = stmt::unary::op_type::INCR,
		};
		break;
	case Stmt_::is_Decr:
		ret->val = stmt::unary {
			.id = ident(p->u.decr_.ident_),
			.op = stmt::unary::op_type::DECR,
		};
		break;
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

type convert_type(Type p) {
	type ret;
	copy_elem_pos(&ret, p);
	switch (p->kind) {
	case Type_::is_Int:
		ret.val = type::type_enum::INT;
		break;
	case Type_::is_Str:
		ret.val = type::type_enum::STR;
		break;
	case Type_::is_Bool:
		ret.val = type::type_enum::BOOL;
		break;
	case Type_::is_Void:
		ret.val = type::type_enum::VOID;
		break;
	case Type_::is_Fun:
	default:
		fprintf(stderr, "Error: bad kind field when converting Type!\n");
		exit(1);
	}
	return ret;
}

exp::uptr convert_expr(Expr p) {
	auto ret = std::make_unique<exp>();
	copy_elem_pos(ret.get(), p);
	switch (p->kind) {
	case Expr_::is_EVar:
		ret->val = exp::var {
			.id = ident(p->u.evar_.ident_),
		};
		break;
	case Expr_::is_ELitInt:
		ret->val = p->u.elitint_.integer_;
		break;
	case Expr_::is_ELitTrue:
		ret->val = true;
		break;
	case Expr_::is_ELitFalse:
		ret->val = false;
		break;
	case Expr_::is_EApp:
		ret->val = exp::call {
			.id = ident(p->u.eapp_.ident_),
			.params = convert_list_expr(p->u.eapp_.listexpr_),
		};
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
