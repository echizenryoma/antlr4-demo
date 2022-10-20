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
  antlr4::tree::ParseTreeProperty<int> int_map_;
  antlr4::tree::ParseTreeProperty<bool> expression_map_;
  bool ans_ = 0;

public:
  bool GetAnswer() const { return ans_; }

  void exitInteger(ArithmeticParser::IntegerContext *ctx) override {
    int ans = std::stod(ctx->getText());
    int_map_.put(ctx, ans);
  }

  void exitParenthesesExpression(
      ArithmeticParser::ParenthesesExpressionContext *ctx) override {
    expression_map_.put(ctx, expression_map_.get(ctx->expression()));
  }

  void exitLogicJoin(ArithmeticParser::LogicJoinContext *ctx) override {
    bool lhs = expression_map_.get(ctx->expression(0));
    bool rhs = expression_map_.get(ctx->expression(1));
    bool ans = false;
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
    expression_map_.put(ctx, ans);
  }

  void exitValueCompare(ArithmeticParser::ValueCompareContext *ctx) override {
    int lhs = int_map_.get(ctx->integer(0));
    int rhs = int_map_.get(ctx->integer(1));
    bool ans = false;
    switch (ctx->op->getType()) {
    case ArithmeticLexer::GREATER_THAN:
      ans = (lhs > rhs);
      break;
    case ArithmeticLexer::LESS_THAN:
      ans = (lhs < rhs);
      break;
    case ArithmeticLexer::EQUAL:
      ans = (lhs == rhs);
      break;
    case ArithmeticLexer::NO_GREATER_THAN:
      ans = (lhs <= rhs);
      break;
    case ArithmeticLexer::NO_LESS_THAN:
      ans = (lhs >= rhs);
      break;
    default:
      ans = false;
      break;
    }
    expression_map_.put(ctx, ans);
  }

  void exitQuery(ArithmeticParser::QueryContext *ctx) override {
    ans_ = expression_map_.get(ctx->expression());
  }
};

bool eval(const std::string &line) {
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
