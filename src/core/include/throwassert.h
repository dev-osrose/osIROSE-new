// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file throwassert
 * \brief Implements the \s ThrowAssert class
 *
 * Usage : throw_assert(false, "Unexpected state value " << state);
 *
 * \author L3nn0x
 * \date march 2017
 */

#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include "logconsole.h"

namespace Core {

class AssertionFailureException : public std::exception {
    public:
        class StreamFormatter {
            public:
                operator std::string() const {
                    return stream.str();
                }

                template <typename T>
                StreamFormatter &operator<<(const T& value) {
                    stream << value;
                    return *this;
                }

            private:
                std::ostringstream stream;
        };

        void log_error() {
            auto logger = CLog::GetLogger(log_type::ASSERT).lock();
            logger->error(report);
        }

        AssertionFailureException(const char *expression, const char *file, int line, const std::string &message) :
            expression(expression), file(file), line(line), message(message) {
                std::ostringstream outStream;
                if (!message.empty())
                    outStream << message << ": ";
                std::string expressionString = expression;
                if (expressionString == "false" || expressionString == "0" || expressionString == "FALSE")
                    outStream << "Unreachable code assertion";
                else
                    outStream << "Assertion '" << expression << "'";
                outStream << " failed in file '" << file << "' line " << line;
                report = outStream.str();
                log_error();
            }

        virtual const char *what() const throw() {
            return report.c_str();
        }

        const char *get_expression() const throw() {
            return expression;
        }

        const char *get_file() const throw() {
            return file;
        }

        int get_line() const throw() {
            return line;
        }

        const char *get_message() const throw() {
            return message.c_str();
        }

        virtual ~AssertionFailureException() throw() = default;

    private:
        const char *expression;
        const char *file;
        int line;
        std::string message;
        std::string report;
};

#define throw_assert(EXPRESSION, MESSAGE) if (!(EXPRESSION)) { throw Core::AssertionFailureException(#EXPRESSION, __FILE__, __LINE__, (Core::AssertionFailureException::StreamFormatter() << MESSAGE)); }
}
