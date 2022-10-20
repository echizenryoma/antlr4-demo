#include <iostream>
#include <string>
#include <unordered_map>

#include "ArithmeticLexer.h"
#include "ArithmeticParser.h"
#include "ArithmeticParserBaseListener.h"
#include "antlr4-runtime.h"
#include "tree/ParseTreeProperty.h"

using namespace antlrcpptest;

class EvaluatorListener : public ArithmeticParserBaseListener {
protected:
  antlr4::tree::ParseTreeProperty<int> property_map_;

public:
  void exitInteger(ArithmeticParser::IntegerContext *ctx) override {
    int ans = std::stod(ctx->getText());
    property_map_.put(ctx, ans);
  }

  void exitParenthesesExpression(
      ArithmeticParser::ParenthesesExpressionContext *ctx) override {
    property_map_.put(ctx, property_map_.get(ctx->expression()));
  }

  void exitLogicJoin(ArithmeticParser::LogicJoinContext *ctx) override {
    int lhs = property_map_.get(ctx->expression(0));
    int rhs = property_map_.get(ctx->expression(1));
    int ans = 0;
    switch (ctx->op->getType()) {
      case ArithmeticLexer::AND:
        ans = lhs && rhs;
        break;
      case ArithmeticLexer::OR:
        ans = lhs || rhs;
        break;
      default:
        ans = 0;
        break;
    }
    property_map_.put(ctx, ans);
  }

  void exitValueCompare(ArithmeticParser::ValueCompareContext *ctx) override {
    int lhs = property_map_.get(ctx->integer(0));
    int rhs = property_map_.get(ctx->integer(1));
    std::cout << "lhs: " << lhs << std::endl;
    std::cout << "rhs: " << rhs << std::endl;
    int ans = 0;
    switch (ctx->op->getType()) {
      case ArithmeticLexer::GREATER_THAN:
        ans = (lhs > rhs) ? 1 : 0;
        break;
      case ArithmeticLexer::LESS_THAN:
        ans = (lhs < rhs) ? 1 : 0;
        break;
      case ArithmeticLexer::EQUAL:
        ans = (lhs == rhs) ? 1 : 0;
        break;
      case ArithmeticLexer::NO_GREATER_THAN:
        ans = (lhs <= rhs) ? 1 : 0;
        break;
      case ArithmeticLexer::NO_LESS_THAN:
        ans = (lhs >= rhs) ? 1 : 0;
        break;
      default:
        ans = 0;
        break;
    }
    property_map_.put(ctx, ans);
  }
};

int eval(const std::string &line) {
  antlr4::ANTLRInputStream input(line);
  ArithmeticLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  ArithmeticParser parser(&tokens);
  antlr4::tree::ParseTree *parse_tree = parser.expression();

  std::cout << parse_tree->toStringTree(&parser) << std::endl;

  EvaluatorListener listener;
  antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parse_tree);
  return 0;
}

int main() {
  std::string line;
  std::cout << ">>> ";
  while (std::getline(std::cin, line)) {
    std::cout << eval(line) << std::endl;
    std::cout << ">>> ";
  }
  return 0;
}
