#pragma once
#include <string>

namespace Daybreak
{
    /**
     * Reads structured tokens from a text source.
     */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    class TTextParser
    {
    public:
        enum class TokenType
        {
            EndOfStream,
            BlankSpace,
            Newline,
            Identifier,
            Symbol,
            Integer,
            Float,
            String,
        };

        struct token_t
        {
            TokenType type;
            std::basic_string_view<CharT, Traits> lexeme;

            bool IsBlankSpace() const noexcept { return type == TokenType::BlankSpace; }
            bool IsNewline() const noexcept { return type == TokenType::Newline; }
            bool IsWhiteSpace() const noexcept { return IsBlankSpace() || IsNewline(); }
        };

    public:
        /** Constructor.
         * \param  text  Text to parse.
         */
        TTextParser(const std::basic_string_view<CharT, Traits>& text) noexcept
            : m_text(text),
              m_textLength(text.size())
        {
        }

    public:
        /** Get if a newline is considered whitespace or a unique token type. */
        bool isNewlineWhitespace() const noexcept { return m_isNewlineWhitespace; }

        /** Set if newline is considered whitespace or a unique token type. */
        void setIsNewlineWhitespace(bool value) noexcept { m_isNewlineWhitespace = value; }

    public:

        token_t readNextToken()
        {
            // Can't read any whitespace if at end of input.
            if (isAtEnd())
            {
                throw std::runtime_error("Cannot read next token when end of input reached");
            }

            // Consume leading wihtespace prior to reading other tokens.
            token_t nextToken;

            if (tryReadWhitespace(&nextToken))
            {
                return nextToken;
            }

            // Get the first character of the next token and use it to intialize the token parsing state machine.
            auto c = m_text[m_position++];

            if (isDigit(c))
            {
                return readNumber(c);
            }
        }

    private:
        bool tryReadWhitespace(token_t * nextToken) noexcept
        {
            if (tryReadBlankSpace(nextToken))
            {
                return true;
            }
            else if (tryReadNewline(nextToken))
            {
                return true;
            }

            return false;
        }

        bool tryReadBlankSpace(token_t * nextToken) noexcept
        {
            // Can't read any whitespace if at end of input.
            if (isAtEnd())
            {
                return false;
            }

            // Keep reading characters so long as they are whitespace.
            auto start = m_position;
            auto end = start;

            auto c = m_text[m_position];

            while (isBlankSpace(c))
            {
                end = m_position;
                c = m_text[m_position++];
            }

            // If at least one blank space character was read, return with a new empty space token.
            if (start != end)
            {
                if (nextToken != nullptr)
                {
                    nextToken->type = TokenType::BlankSpace;
                    nextToken->lexeme = m_text.substr(start, end - start);
                }
                
                return true;
            }
        }

        bool tryReadNewline(token_t * nextToken) noexcept
        {
            // Can't read any whitespace if at end of input.
            if (isAtEnd())
            {
                return false;
            }

            // Try reading a \n or \r\n set of characters.
            if (m_text[m_position] == 'n')
            {
                // Set token to newline before returning.
                if (nextToken != nullptr)
                {
                    nextToken->type = TokenType::Newline;
                    nextToken->lexeme = m_text.substr(m_position, 1);
                }

                // Move to character after newline.
                m_position++;

                // Return that newline was found.
                return true;
            }
            else if (m_text[m_position] == '\r' && m_position + 1 < m_textLength && m_text[m_position + 1] == '\n')
            {
                // Set token to newline before returning.
                if (nextToken != nullptr)
                {
                    nextToken->type = TokenType::Newline;
                    nextToken->lexeme = m_text.substr(m_position, 2);
                }

                // Move to character after newline.
                m_position += 2;

                // Return that newline was found.
                return true;
            }

            // No newlines found.
            return false;
        }

        token_t readNumber()
        {
            // TODO: Implement.
        }

    public:

        /** Return the next word from the string or throw an exception if not possible. */
        std::string_view readNextWord()
        {
            std::string_view result;

            if (!tryReadNextWord(result))
            {
                throw std::runtime_error("Cannot read next word from string");
            }

            return result;
        }

        /** Tries to read the next word from the string. */
        bool tryReadNextWord(std::string_view& result) noexcept
        {
            // Consume any leading whitespace.
            consumeWhitespaceAndNewlines();
            const auto start = m_position;

            // Move forward until the next whitespace character is found.
            while (!isAtEnd() && !isWhitespaceOrNewline(m_text, m_position))
            {
                m_position++;
            }

            const auto end = m_position;

            // Return to the caller the extracted line.
            if (start == end)
            {
                return false;
            }

            result = m_text.substr(start, end - start);
            return true;
        }

        /** Move the parser forward past any whitespace or newline characters. */
        void consumeWhitespaceAndNewlines() noexcept
        {
            while (!isAtEnd() && isWhitespaceOrNewline(m_text, m_position))
            {
                m_position++;
            }
        }

        bool isAtEnd() const noexcept
        {
            return m_position >= m_textLength;
        }

        static bool isBlankSpace(CharT c) noexcept
        {
            return c == ' ' || c == '\t';
        }

        static bool isDigit(CharT c) noexcept
        {
            return c >= '0' && c <= '9';
        }

        /** Check if character is whitespace. */
        static bool isWhitespaceOrNewline(const std::basic_string_view<CharT, Traits>& text, size_t index)
        {
            if (index < text.size())
            {
                const auto c = text[index];
                return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
            }
            else
            {
                throw std::runtime_error("Unexpected end of input when testing if character is whitespace");
            }
        }

        static bool isNewlineChar(
            const std::basic_string_view<CharT, Traits>& text,
            size_t index,
            _In_opt_ size_t * pCharCount = nullptr)
        {
            if (index < text.size())
            {
                return isNewlineChar_NoCheck(text, index, pCharCount);
            }
            else
            {
                throw std::runtime_error("Unexpected end of input when testing if character is newline");
            }
        }

    private:
        static bool isNewlineChar_NoCheck(
            const std::basic_string_view<CharT, Traits>& text,
            size_t index,
            _In_opt_ size_t * pCharCount = nullptr) noexcept
        {
            const auto c = text[index];

            if (c == '\n')
            {
                if (pCharCount != nullptr)
                {
                    *pCharCount = 1;
                }

                return true;
            }
            else if (c == '\r' && index + 1 < text.size() && text[index + 1] == '\n')
            {
                if (pCharCount != nullptr)
                {
                    *pCharCount = 2;
                }

                return true;
            }

            return false;
        }

    private:
        const std::basic_string_view<CharT, Traits> m_text;
        const size_t m_textLength = 0;
        size_t m_position = 0;
        bool m_isNewlineWhitespace = false;
    };

    using TextParser = TTextParser<char>;
    using WTextParser = TTextParser<wchar_t>;
}
