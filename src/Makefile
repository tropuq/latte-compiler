CC = gcc
CXX = g++ -std=c++17
CCFLAGS = -O3 -g -Wall -Wextra -fdiagnostics-color=always -I./

OBJS = Absyn.o Lexer.o Parser.o convert.o verifier.o simplifier.o \
	code_printer.o gen_ic.o control_flow_graph.o gen_asm.o asm_code.o \
	remove_dead_code.o lcse.o

.PHONY : clean

all : latc_x86_64

clean :
	rm -f *.o latc_x86_64

latc_x86_64 : ${OBJS} latc_x86_64.o
	${CXX} ${OBJS} latc_x86_64.o -o latc_x86_64 -lstdc++fs

Absyn.o : parser/Absyn.c parser/Absyn.h
	${CC} ${CCFLAGS} -c parser/Absyn.c

Lexer.o: parser/Lexer.c parser/Parser.h parser/Absyn.h
	${CC} ${CCFLAGS} -c parser/Lexer.c

Parser.o : parser/Parser.c parser/Absyn.h
	${CC} ${CCFLAGS} -c parser/Parser.c

latc_x86_64.o: latc_x86_64.cc parser/Parser.h parser/Absyn.h \
 backend/asm/gen_asm.hh backend/asm/asm_code.hh core/tree_types.hh \
 core/code_pos.hh utils/utils.hh backend/ic/control_flow_graph.hh \
 backend/ic/code_types.hh backend/ic/gen_ic.hh core/compilation_errors.hh \
 utils/code_printer.hh utils/colors.hh frontend/simplifier.hh \
 frontend/verifier.hh parser/convert.hh
	${CXX} ${CCFLAGS} -c latc_x86_64.cc

convert.o: parser/convert.cc parser/convert.hh parser/Absyn.h \
 core/tree_types.hh core/code_pos.hh utils/utils.hh
	${CXX} ${CCFLAGS} -c parser/convert.cc

verifier.o: frontend/verifier.cc core/compilation_errors.hh \
 core/code_pos.hh utils/code_printer.hh utils/colors.hh utils/utils.hh \
 core/tree_types.hh frontend/verifier.hh
	${CXX} ${CCFLAGS} -c frontend/verifier.cc

simplifier.o: frontend/simplifier.cc core/compilation_errors.hh \
 core/code_pos.hh utils/code_printer.hh utils/colors.hh utils/utils.hh \
 core/tree_types.hh frontend/simplifier.hh
	${CXX} ${CCFLAGS} -c frontend/simplifier.cc

code_printer.o: utils/code_printer.cc utils/code_printer.hh \
 utils/colors.hh utils/utils.hh
	${CXX} ${CCFLAGS} -c utils/code_printer.cc

gen_ic.o: backend/ic/gen_ic.cc utils/utils.hh backend/ic/code_types.hh \
 core/tree_types.hh core/code_pos.hh backend/ic/gen_ic.hh \
 backend/ic/ident_allocator.hh backend/ic/code_builder.hh
	${CXX} ${CCFLAGS} -c backend/ic/gen_ic.cc

control_flow_graph.o: backend/ic/control_flow_graph.cc \
 backend/ic/control_flow_graph.hh backend/ic/code_types.hh \
 core/tree_types.hh core/code_pos.hh utils/utils.hh backend/ic/lcse.hh
	${CXX} ${CCFLAGS} -c backend/ic/control_flow_graph.cc

asm_code.o: backend/asm/asm_code.cc backend/asm/asm_code.hh \
 core/tree_types.hh core/code_pos.hh utils/utils.hh
	${CXX} ${CCFLAGS} -c backend/asm/asm_code.cc

gen_asm.o: backend/asm/gen_asm.cc backend/asm/asm_code.hh \
 core/tree_types.hh core/code_pos.hh utils/utils.hh \
 backend/ic/control_flow_graph.hh backend/ic/code_types.hh \
 backend/asm/const_string_allocator.hh backend/asm/description_manager.hh \
 backend/asm/stack_allocator.hh backend/asm/remove_dead_code.hh
	${CXX} ${CCFLAGS} -c backend/asm/gen_asm.cc

remove_dead_code.o: backend/asm/remove_dead_code.cc backend/asm/asm_code.hh \
 core/tree_types.hh core/code_pos.hh utils/utils.hh \
 backend/asm/remove_dead_code.hh
	${CXX} ${CCFLAGS} -c backend/asm/remove_dead_code.cc

lcse.o: backend/ic/lcse.cc backend/ic/lcse.hh backend/ic/code_types.hh \
 core/tree_types.hh core/code_pos.hh utils/utils.hh
	${CXX} ${CCFLAGS} -c backend/ic/lcse.cc
