//
// Project: clibparser
// Created by bajdcc
//

#include <iostream>
#include "cgen.h"
#include "cast.h"
#include "cexception.h"

#define AST_IS_KEYWORD(node) ((node)->flag == ast_keyword)
#define AST_IS_KEYWORD_N(node, k) (AST_IS_KEYWORD(node) && (node)->data._keyword == (k))
#define AST_IS_OP(node) ((node)->flag == ast_operator)
#define AST_IS_OP_N(node, k) (AST_IS_OP(node) && (node)->data._op == (k))
#define AST_IS_ID(node) ((node)->flag == ast_literal)

#define LOG_TYPE 1

namespace clib {

    string_t sym_t::to_string() const {
        return "[Symbol]";
    }

    int sym_t::size() const {
        return 0;
    }

    symbol_t sym_t::get_type() const {
        return s_sym;
    }

    type_t::type_t(int ptr) : ptr(ptr) {}

    symbol_t type_t::get_type() const {
        return s_type;
    }

    type_base_t::type_base_t(lexer_t type, int ptr) : type_t(ptr), type(type) {}

    string_t type_base_t::to_string() const {
        std::stringstream ss;
        ss << LEX_STRING(type);
        for (int i = 0; i < ptr; ++i) {
            ss << '*';
        }
        return ss.str();
    }

    int type_base_t::size() const {
        if (ptr > 0)
            return sizeof(void *);
        switch (type) {
#define DEFINE_LEXER_SIZE(t) case l_##t: return LEX_SIZEOF(t);
            DEFINE_LEXER_SIZE(char)
            DEFINE_LEXER_SIZE(uchar)
            DEFINE_LEXER_SIZE(short)
            DEFINE_LEXER_SIZE(ushort)
            DEFINE_LEXER_SIZE(int)
            DEFINE_LEXER_SIZE(uint)
            DEFINE_LEXER_SIZE(long)
            DEFINE_LEXER_SIZE(ulong)
            DEFINE_LEXER_SIZE(float)
            DEFINE_LEXER_SIZE(double)
            DEFINE_LEXER_SIZE(operator)
            DEFINE_LEXER_SIZE(keyword)
            DEFINE_LEXER_SIZE(identifier)
            DEFINE_LEXER_SIZE(string)
            DEFINE_LEXER_SIZE(comment)
            DEFINE_LEXER_SIZE(space)
            DEFINE_LEXER_SIZE(newline)
            DEFINE_LEXER_SIZE(error)
#undef DEFINE_LEXER_SIZE
            default:
                assert(!"invalid get_type");
                return 0;
        }
    }

    symbol_t type_base_t::get_type() const {
        return s_type_base;
    }

    symbol_t type_typedef_t::get_type() const {
        return s_type_typedef;
    }

    sym_id_t::sym_id_t(const std::shared_ptr<type_t> &base, const string_t &id)
        : base(base), id(id) {}

    string_t sym_id_t::to_string() const {
        std::stringstream ss;
        ss << base->to_string() << " " << id << ", ";
        ss << "Class: " << sym_class_string(clazz) << ", ";
        ss << "Addr: " << addr;
        return ss.str();
    }

    symbol_t sym_id_t::get_type() const {
        return s_sym_id;
    }

    void cgen::gen(ast_node *node) {
        symbols.clear();
        symbols.emplace_back();
        tmp.clear();
        tmp.emplace_back();
        ast.clear();
        ast.emplace_back();
        gen_rec(node, 0);
    }

    template<class T>
    static void gen_recursion(ast_node *node, int level, T f) {
        if (node == nullptr)
            return;
        auto i = node;
        if (i->next == i) {
            f(i, level);
            return;
        }
        f(i, level);
        i = i->next;
        while (i != node) {
            f(i, level);
            i = i->next;
        }
    }

    template<class T>
    static std::vector<T *> gen_get_children(T *node) {
        std::vector<T *> v;
        if (node == nullptr)
            return v;
        auto i = node;
        if (i->next == i) {
            v.push_back(i);
            return v;
        }
        v.push_back(i);
        i = i->next;
        while (i != node) {
            v.push_back(i);
            i = i->next;
        }
        return v;
    }

    void cgen::gen_rec(ast_node *node, int level) {
        if (node == nullptr)
            return;
        auto rec = [this](auto n, auto l) { this->gen_rec(n, l); };
        auto type = (ast_t) node->flag;
        tmp.emplace_back();
        ast.emplace_back();
        switch (type) {
            case ast_root: {// 根结点，全局声明
                gen_recursion(node->child, level, rec);
            }
                break;
            case ast_collection: {
                auto children = gen_get_children(node->child);
                gen_coll(children, level + 1, node);
            }
                break;
            case ast_keyword:
            case ast_operator:
            case ast_literal:
            case ast_string:
            case ast_char:
            case ast_uchar:
            case ast_short:
            case ast_ushort:
            case ast_int:
            case ast_uint:
            case ast_long:
            case ast_ulong:
            case ast_float:
            case ast_double:
                ast.back().push_back(node);
                break;
        }
        if (!tmp.back().empty()) {
            //assert(tmp.back().size() == 1);
            auto &top = tmp[tmp.size() - 2];
            for (auto &t : tmp.back()) {
                top.push_back(t);
            }
        }
        tmp.pop_back();
        if (!ast.back().empty()) {
            auto &top = ast[ast.size() - 2];
            std::copy(ast.back().begin(), ast.back().end(), std::back_inserter(top));
        }
        ast.pop_back();
    }

    void cgen::gen_coll(const std::vector<ast_node *> &nodes, int level, ast_node *node) {
        switch (node->data._coll) {
            case c_program:
                break;
            case c_primaryExpression:
                break;
            case c_constant:
                break;
            case c_postfixExpression:
                break;
            case c_argumentExpressionList:
                break;
            case c_unaryExpression:
                break;
            case c_unaryOperator:
                break;
            case c_castExpression:
                break;
            case c_multiplicativeExpression:
                break;
            case c_additiveExpression:
                break;
            case c_shiftExpression:
                break;
            case c_relationalExpression:
                break;
            case c_equalityExpression:
                break;
            case c_andExpression:
                break;
            case c_exclusiveOrExpression:
                break;
            case c_inclusiveOrExpression:
                break;
            case c_logicalAndExpression:
                break;
            case c_logicalOrExpression:
                break;
            case c_conditionalExpression:
                break;
            case c_assignmentExpression:
                break;
            case c_assignmentOperator:
                break;
            case c_expression:
                break;
            case c_constantExpression:
                break;
            case c_declaration:
                break;
            case c_declarationSpecifiers:
                break;
            case c_declarationSpecifiers2:
                break;
            case c_declarationSpecifier:
                break;
            case c_initDeclaratorList:
                break;
            case c_initDeclarator:
                break;
            case c_storageClassSpecifier:
                break;
            case c_typeSpecifier:
                break;
            case c_structOrUnionSpecifier:
                break;
            case c_structOrUnion:
                break;
            case c_structDeclarationList:
                break;
            case c_structDeclaration:
                break;
            case c_specifierQualifierList:
                break;
            case c_structDeclaratorList:
                break;
            case c_structDeclarator:
                break;
            case c_enumSpecifier:
                break;
            case c_enumeratorList:
                break;
            case c_enumerator:
                break;
            case c_enumerationConstant:
                break;
            case c_typeQualifier:
                break;
            case c_declarator:
                break;
            case c_directDeclarator:
                break;
            case c_pointer:
                break;
            case c_typeQualifierList:
                break;
            case c_parameterTypeList:
                break;
            case c_parameterList:
                break;
            case c_parameterDeclaration:
                break;
            case c_identifierList:
                break;
            case c_typeName:
                break;
            case c_abstractDeclarator:
                break;
            case c_directAbstractDeclarator:
                break;
            case c_typedefName:
                break;
            case c_initializer:
                break;
            case c_initializerList:
                break;
            case c_designation:
                break;
            case c_designatorList:
                break;
            case c_designator:
                break;
            case c_statement:
                break;
            case c_labeledStatement:
                break;
            case c_compoundStatement:
                break;
            case c_blockItemList:
                break;
            case c_blockItem:
                break;
            case c_expressionStatement:
                break;
            case c_selectionStatement:
                break;
            case c_iterationStatement:
                break;
            case c_forCondition:
                break;
            case c_forDeclaration:
                break;
            case c_forExpression:
                break;
            case c_jumpStatement:
                break;
            case c_compilationUnit:
                break;
            case c_translationUnit:
                break;
            case c_externalDeclaration:
                break;
            case c_functionDefinition:
                symbols.emplace_back();
                break;
            case c_declarationList:
                break;
        }
        for (auto &node : nodes) {
            gen_rec(node, level);
        }
        auto &asts = ast.back();
        switch (node->data._coll) {
            case c_program:
                break;
            case c_primaryExpression:
                break;
            case c_constant:
                break;
            case c_postfixExpression:
                break;
            case c_argumentExpressionList:
                break;
            case c_unaryExpression:
                break;
            case c_unaryOperator:
                break;
            case c_castExpression:
                break;
            case c_multiplicativeExpression:
                break;
            case c_additiveExpression:
                break;
            case c_shiftExpression:
                break;
            case c_relationalExpression:
                break;
            case c_equalityExpression:
                break;
            case c_andExpression:
                break;
            case c_exclusiveOrExpression:
                break;
            case c_inclusiveOrExpression:
                break;
            case c_logicalAndExpression:
                break;
            case c_logicalOrExpression:
                break;
            case c_conditionalExpression:
                break;
            case c_assignmentExpression:
                break;
            case c_assignmentOperator:
                break;
            case c_expression:
                break;
            case c_constantExpression:
                break;
            case c_declaration:
                break;
            case c_declarationSpecifiers: {
                std::shared_ptr<type_t> base_type;
                if (AST_IS_KEYWORD_N(asts[0], k_unsigned)) { // unsigned ...
                    if (asts.size() == 1) {
                        asts.erase(asts.begin());
                        base_type = std::make_shared<type_base_t>(l_int);
                    } else {
                        assert(asts.size() > 1 && AST_IS_KEYWORD(asts[1]));
                        lexer_t type;
                        switch (asts[1]->data._keyword) {
                            case k_char:
                                type = l_uchar;
                                break;
                            case k_short:
                                type = l_short;
                                break;
                            case k_int:
                                type = l_uint;
                                break;
                            case k_long:
                                type = l_ulong;
                                break;
                            default:
                                error("invalid unsigned * get_type");
                                break;
                        }
                        asts.erase(asts.begin());
                        asts.erase(asts.begin());
                        base_type = std::make_shared<type_base_t>(type);
                    }
                } else {
                    lexer_t type;
                    switch (asts[0]->data._keyword) {
                        case k_char:
                            type = l_char;
                            break;
                        case k_double:
                            type = l_double;
                            break;
                        case k_float:
                            type = l_float;
                            break;
                        case k_int:
                            type = l_int;
                            break;
                        case k_long:
                            type = l_long;
                            break;
                        case k_short:
                            type = l_short;
                            break;
                        default:
                            error("invalid unsigned * get_type");
                            break;
                    }
                    asts.erase(asts.begin());
                    base_type = std::make_shared<type_base_t>(type);
                }
                if (!asts.empty()) {
                    for (auto &a : asts) {
                        assert(AST_IS_OP_N(a, op_times));
                    }
                    base_type->ptr = asts.size();
                }
                asts.clear();
#if LOG_TYPE
                std::cout << "[DEBUG] Type: " << base_type->to_string() << std::endl;
#endif
                tmp.back().push_back(base_type);
            }
                break;
            case c_declarationSpecifiers2:
                break;
            case c_declarationSpecifier:
                break;
            case c_initDeclaratorList: {
                auto clazz = z_undefined;
                if (node->parent->parent->data._coll == c_externalDeclaration) {
                    clazz = z_global_var;
                } else {
                    clazz = z_local_var;
                }
                auto type = std::dynamic_pointer_cast<type_base_t>((tmp.rbegin() + 1)->front());
                {
                    assert(AST_IS_ID(asts[0]));
                    auto new_id = std::make_shared<sym_id_t>(type, asts[0]->data._string);
                    new_id->clazz = clazz;
                    allocate(*new_id);
                    symbols.back().insert(std::make_pair(asts[0]->data._string, new_id));
#if LOG_TYPE
                    std::cout << "[DEBUG] Id: " << new_id->to_string() << std::endl;
#endif
                }
                auto ptr = 0;
                for (int i = 1; i < asts.size(); ++i) {
                    if (AST_IS_OP_N(asts[i], op_times)) {
                        ptr++;
                    } else {
                        assert(AST_IS_ID(asts[i]));
                        auto new_type = std::make_shared<type_base_t>(type->type, ptr);
                        auto new_id = std::make_shared<sym_id_t>(new_type, asts[i]->data._string);
                        new_id->clazz = clazz;
                        allocate(*new_id);
#if LOG_TYPE
                        std::cout << "[DEBUG] Id: " << new_id->to_string() << std::endl;
#endif
                        symbols.back().insert(std::make_pair(asts[i]->data._string, new_id));
                        ptr = 0;
                    }
                }
            }
                break;
            case c_initDeclarator:
                break;
            case c_storageClassSpecifier:
                break;
            case c_typeSpecifier:
                break;
            case c_structOrUnionSpecifier:
                break;
            case c_structOrUnion:
                break;
            case c_structDeclarationList:
                break;
            case c_structDeclaration:
                break;
            case c_specifierQualifierList:
                break;
            case c_structDeclaratorList:
                break;
            case c_structDeclarator:
                break;
            case c_enumSpecifier:
                break;
            case c_enumeratorList:
                break;
            case c_enumerator:
                break;
            case c_enumerationConstant:
                break;
            case c_typeQualifier:
                break;
            case c_declarator:
                break;
            case c_directDeclarator:
                break;
            case c_pointer:
                break;
            case c_typeQualifierList:
                break;
            case c_parameterTypeList:
                break;
            case c_parameterList:
                break;
            case c_parameterDeclaration:
                break;
            case c_identifierList:
                break;
            case c_typeName:
                break;
            case c_abstractDeclarator:
                break;
            case c_directAbstractDeclarator:
                break;
            case c_typedefName:
                break;
            case c_initializer:
                break;
            case c_initializerList:
                break;
            case c_designation:
                break;
            case c_designatorList:
                break;
            case c_designator:
                break;
            case c_statement:
                break;
            case c_labeledStatement:
                break;
            case c_compoundStatement:
                break;
            case c_blockItemList:
                break;
            case c_blockItem:
                break;
            case c_expressionStatement:
                break;
            case c_selectionStatement:
                break;
            case c_iterationStatement:
                break;
            case c_forCondition:
                break;
            case c_forDeclaration:
                break;
            case c_forExpression:
                break;
            case c_jumpStatement:
                break;
            case c_compilationUnit:
                break;
            case c_translationUnit:
                break;
            case c_externalDeclaration:
                break;
            case c_functionDefinition:
                symbols.pop_back();
                break;
            case c_declarationList:
                break;
        }
    }

    void cgen::error(const string_t &str) {
        std::stringstream ss;
        ss << "GEN ERROR: " << str;
        throw cexception(ss.str());
    }

    void cgen::allocate(sym_id_t &id) {
        if (id.base->get_type() == s_type_base) {
            const auto &type = std::dynamic_pointer_cast<type_base_t>(id.base);
            if (id.clazz == z_global_var) {
                id.addr = data.size();
                for (auto i = 0; i < type->size(); ++i) {
                    data.push_back(0);
                }
            } else if (id.clazz == z_local_var) {

            } else {
                assert(!"not supported");
            }
        } else {
            assert(!"not supported");
        }
    }

    std::tuple<sym_class_t, string_t> sym_class_string_list[] = {
        std::make_tuple(z_undefined, "undefined"),
        std::make_tuple(z_global_var, "global id"),
        std::make_tuple(z_local_var, "local id"),
    };

    const string_t &sym_class_string(sym_class_t t) {
        assert(t >= z_undefined && t < z_end);
        return std::get<1>(sym_class_string_list[t]);
    }
}