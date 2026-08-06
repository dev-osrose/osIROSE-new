#include "gtest/gtest.h"

#include "connection.h"

#include <string>

// escapeData is the only thing standing between client-supplied strings and the
// raw `CALL create_account(...)` / `CALL create_char(...)` statements built with
// fmt::format in the login and char servers. Everything else goes through
// sqlpp23 and is parameterised; these five sites are not.

TEST(EscapeData, LeavesPlainTextAlone) {
  EXPECT_EQ("hello", Core::escapeData("hello"));
  EXPECT_EQ("Player123", Core::escapeData("Player123"));
  EXPECT_EQ("", Core::escapeData(""));
}

TEST(EscapeData, EscapesSingleQuote) {
  EXPECT_EQ("\\'", Core::escapeData("'"));
  EXPECT_EQ("O\\'Brien", Core::escapeData("O'Brien"));
}

TEST(EscapeData, EscapesDoubleQuote) {
  EXPECT_EQ("\\\"", Core::escapeData("\""));
  EXPECT_EQ("say \\\"hi\\\"", Core::escapeData("say \"hi\""));
}

TEST(EscapeData, EscapesLikeWildcards) {
  EXPECT_EQ("\\%", Core::escapeData("%"));
  EXPECT_EQ("\\_", Core::escapeData("_"));
  EXPECT_EQ("100\\%\\_sure", Core::escapeData("100%_sure"));
}

TEST(EscapeData, StripsSemicolons) {
  EXPECT_EQ("", Core::escapeData(";"));
  EXPECT_EQ("DROP TABLE accounts", Core::escapeData(";DROP TABLE accounts;"));
}

TEST(EscapeData, EscapesEveryOccurrenceNotJustTheFirst) {
  EXPECT_EQ("\\'\\'\\'", Core::escapeData("'''"));
  EXPECT_EQ("a\\%b\\%c\\%d", Core::escapeData("a%b%c%d"));
}

TEST(EscapeData, HandlesAllSpecialsTogether) {
  EXPECT_EQ("\\'\\\"\\%\\_", Core::escapeData("'\"%_;"));
}

// The classic injection shape: close the quote, chain a statement. The quote is
// escaped and the semicolon removed, so neither half survives.
TEST(EscapeData, NeutralisesQuoteThenStatementInjection) {
  const auto escaped = Core::escapeData("' OR 1=1; --");
  EXPECT_EQ(std::string::npos, escaped.find(";"));
  EXPECT_EQ("\\' OR 1=1 --", escaped);
}

// Documents a real limitation rather than asserting safety: the backslash
// itself is not escaped, so input ending in a backslash escapes the quote that
// fmt::format puts after it. escapeData is not a general-purpose escaper.
TEST(EscapeData, DoesNotEscapeBackslashes) {
  EXPECT_EQ("\\", Core::escapeData("\\"));
  EXPECT_EQ("ends\\", Core::escapeData("ends\\"));
}

// Not idempotent: the backslash inserted by a first pass is left alone, but the
// character it escapes gets escaped again. Callers must escape exactly once.
TEST(EscapeData, IsNotIdempotent) {
  const auto once = Core::escapeData("'");
  const auto twice = Core::escapeData(once);
  EXPECT_EQ("\\'", once);
  EXPECT_EQ("\\\\'", twice);
  EXPECT_NE(once, twice);
}
