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
  int ans_ = 0;

public:
  int GetAnswer() const { return ans_; }

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

  void exitQuery(ArithmeticParser::QueryContext *ctx) override {
    ans_ = property_map_.get(ctx->expression());
  }
};

int eval(const std::string &line) {
  antlr4::ANTLRInputStream input(line);
  ArithmeticLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  ArithmeticParser parser(&tokens);
  antlr4::tree::ParseTree *parse_tree = parser.query();

  EvaluatorListener listener;
  antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parse_tree);

  std::cout << parse_tree->toStringTree(&parser) << std::endl;
  return listener.GetAnswer();
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
