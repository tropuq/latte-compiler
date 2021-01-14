#include <cstdio>

extern "C" {
#include "parser/Parser.h"
}

// #include "backend/asm/gen_asm.hh"
// #include "backend/ic/control_flow_graph.hh"
// #include "backend/ic/gen_ic.hh"
#include "core/code_pos.hh"
#include "core/compilation_errors.hh"
#include "core/tree_types.hh"
// #include "frontend/simplifier.hh"
// #include "frontend/verifier.hh"
#include "parser/convert.hh"
#include "utils/code_printer.hh"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace {

latte::core::code_pos syntax_error_pos;

extern "C" {
void latteerror(const char *) {
	extern YYLTYPE lattelloc;
	syntax_error_pos = latte::core::convert_bnfc_pos(lattelloc.first_line, lattelloc.first_column);
}
}

latte::core::program::uptr get_program_tree(const char *filename) {
	FILE *input;
	if (filename)
		input = fopen(filename, "r");
	else
		input = stdin;
	if (!input)
		throw latte::core::compilation_error("failed to open the file");

	auto parsed_tree = pProgram(input);
	if (filename)
		fclose(input);
	if (!parsed_tree)
		throw latte::core::compilation_error(syntax_error_pos, "syntax error");
	return latte::parser::convert_program(parsed_tree);
}

struct out_filenames {
	std::string exec_name;
	std::string code_name;
	std::string obj_name;
	out_filenames(const char *filename) {
		fs::path path(filename);
		auto parent = path.parent_path();
		auto in_dir_no_ext_name = path.stem();
		std::string no_ext_name = parent / path.stem();
		code_name = no_ext_name + ".s";
		obj_name = no_ext_name + ".o";
		exec_name = std::move(no_ext_name);
	}
};

void compile(latte::core::program::uptr &prog, out_filenames f) {
	std::cerr << prog << std::endl;
	// latte::frontend::verify_program(prog);
	// latte::frontend::simplify_program(prog);
	// auto qcode = latte::backend::convert_to_quad_prog(prog);
	// std::vector<latte::backend::control_flow_graph> cfgs;
	// for (auto &f : qcode.funcs)
	// 	cfgs.emplace_back(latte::backend::control_flow_graph(f));
	// auto asm_code = latte::backend::gen_asm_for_prog(cfgs);

	// std::fstream out(f.code_name, std::ios::out);
	// out << asm_code << std::endl;
	// system(concat("nasm -felf64 ", f.code_name, " && ld ", f.obj_name,
	// 	" lib/runtime.o -lc --dynamic-linker=/lib64/ld-linux-x86-64.so.2 -o ", f.exec_name).c_str());
}

} // namespace

int main(int argc, char **argv) {
	if (argc != 2)
		return 1;
	const char *filename = argv[1];

	latte::code_printer printer(filename);

	try {
		auto prog = get_program_tree(filename);
		compile(prog, out_filenames(filename));
	} catch (latte::core::compilation_error &err) {
		std::cerr << "ERROR" << std::endl;
		err.print_error(std::cerr, printer);
		return 1;
	}
	std::cerr << "OK" << std::endl;
}
