#pragma once
#include <string>

namespace Daybreak::TextUtils
{
    /** Character constants. */
    template<typename T> struct Constants { };

    template<> struct Constants<char>
    {
        const static char * WhitespaceChars;
    };

    template<> struct Constants<wchar_t>
    {
        const static wchar_t * WhitespaceChars;
    };

    /**
     * Read lines of text from a string with optional support for ignoring comments.
     */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    class TStringLineReader
    {
    public:
        /** Constructor. 
         * \param  text  Text to parse.
         */
        TStringLineReader(
            const std::basic_string_view<CharT, Traits>& text) noexcept
            : m_text(text)
        {
        }

        /** Constructor.
         * \param  text  Text to parse.
         * \parma  commentChar  Character to treat as starting a comment.
         */
        TStringLineReader(
            const std::basic_string_view<CharT, Traits>& text,
            CharT commentChar) noexcept
            : m_text(text),
              m_commentChar(commentChar),
              m_stripComments(true)
        {
        }

        /** Returns the next line from the text. */
        std::basic_string_view<CharT, Traits> readNextLine() noexcept
        {
            const auto start = m_position;
            const auto size = m_text.size();

            auto end = start;
            auto firstCommentChar = std::basic_string_view<CharT, Traits>::npos;

            // Read from the current position until the next newline is found. Once the loop has exited m_position will
            // either be pointing at the first character after the next newline, OR it will be at the end of the text.
            while (m_position < size)
            {
                const auto c = m_text[m_position++];

                // Record the position of this character if it is the start of a comment. Comments will be removed from
                // the string when reading a line (if enabled).
                if (m_stripComments && c == m_commentChar)
                {
                    firstCommentChar = m_position - 1;
                }

                // Check next character is a newline or not. Windows style newlines (\r\n) require special handling!
                // When a carriage return is found, peek at the next character to see if it is the expected newline
                // character. If it newline is found, then simply register the newline and adjust m_position to point
                // to the first character after the newline. Otherwise (\r is not followed by a \n) add the character
                // to the current line.
                if (c == '\n')
                {
                    // Trivial newline found - end of the current line. m_position is left pointing to the next
                    // character after the newline.
                    end = m_position - 1;
                    break;
                }
                else if (c == '\r' && m_position < size && m_text[m_position] == '\n')
                {
                    // Windows CRLF newline found. 
                    end = m_position - 1;
                    m_position++;
                    break;
                }
                else
                {
                    // Part of the current line.
                    end = m_position;
                    continue;
                }
            }

            // If a comment was found in this line then adjust the end of the line to be the start of the comment.
            if (firstCommentChar != std::basic_string_view<CharT, Traits>::npos)
            {
                end = firstCommentChar;
            }

            // Return to the caller the extracted line.
            return m_text.substr(start, end - start);
        }

        /** Check if there is another line to be read from the text. */
        bool hasNextLine() const noexcept
        {
            return m_position < m_text.size();
        }

        /** Reset the line reader to the beginning of the input text. */
        void reset() noexcept
        {
            m_position = 0;
        }

    private:
        std::basic_string_view<CharT, Traits> m_text;
        typename std::basic_string_view<CharT, Traits>::size_type m_position = 0;
        CharT m_commentChar = 0;
        bool m_stripComments = false;
    };

    using StringLineReader = TStringLineReader<char>;
    using WStringLineReader = TStringLineReader<wchar_t>;
    using U16WStringLineReader = TStringLineReader<char16_t>;
    using U32WStringLineReader = TStringLineReader<char32_t>;

    /**
     * Convert text into a sequence of tokens spearated by a specific character.
     */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    class TStringSplitter
    {
    public:
        /** Constructor.
         * \param  text        Text to split.
         * \param  separators  List of characters to split on.
         */
        TStringSplitter(
            const std::basic_string_view<CharT, Traits>& text,
            const std::basic_string_view<CharT, Traits>& separators,
            bool skipEmptyTokens = false) noexcept
            : m_text(text),
              m_separators(separators),
              m_skipEmptyTokens(skipEmptyTokens)
        {
        }

        /** Returns the next token from the text. */
        std::basic_string_view<CharT, Traits> readNextToken()
        {
            // Throw an exception if there are no more tokens to be read.
            if (m_position >= m_text.size())
            {
                throw std::runtime_error("No more tokens to read for string splitter");
            }

            // Get the next valid token (either any token or the next non-empty token depending on settings).
            const auto textSize = m_text.size();

            while (m_position < textSize)
            {
                // Find the position of the next unread separator character.
                auto nextSeparatorPosition = m_text.find_first_of(m_separators, m_position);

                // If there are no more separator characters then move the position to the end of the input which will
                // cover all remaining unread characters.
                if (nextSeparatorPosition == std::basic_string_view<CharT, Traits>::npos)
                {
                    nextSeparatorPosition = m_text.length();
                }

                // Break out of the loop if this token is non-empty (has at least one whitespace character) or if the
                // splitter is configured to not skip empty tokens.
                auto start = m_position;
                auto end = nextSeparatorPosition;

                m_position = nextSeparatorPosition + 1;

                if (start != end || !m_skipEmptyTokens)
                {
                    return m_text.substr(start, end - start);
                }
            }

            // If this point is reached then the splitter could not find any more non-empty tokens to return.
            return std::basic_string_view<CharT, Traits>();
        }

        /** Check if there is another token to be read from the text. */
        bool hasNextToken() const noexcept
        {
            // Read ahead if skip empty tokens is enabled otherwise check if at end of text.
            if (m_skipEmptyTokens)
            {
                const auto size = m_text.size();
                auto i = m_position;

                while (i < size && isSeparator(m_text[i])) { i++; }
                return (i < size);
            }

            return m_position < m_text.size();
        }

        /** Check if character c is a separator. */
        bool isSeparator(CharT c) const noexcept
        {
            for (const auto& x : m_separators)
            {
                if (x == c)
                {
                    return true;
                }
            }

            return false;
        }

        /** Reset the line reader to the beginning of the input text. */
        void reset() noexcept
        {
            m_position = 0;
        }

        /** Read the remaining unread text and return it. */
        std::basic_string_view<CharT, Traits> readRemaining() noexcept
        {
            auto start = m_position;

            // Move the current position to the end prior to returning.
            m_position = m_text.size();

            // Return the remaining unread portion of the string.
            return m_text.substr(start, m_text.size() - start);
        }

        /** Get if the splitter will skip empty tokens (tokens that do not contain any text). */
        bool skipEmptyTokens() const noexcept { return m_skipEmptyTokens; }

        /** Set if the splitter will skip empty tokens (tokens that do not contain any text). */
        void setSkipEmptyTokens(bool shouldSkip) noexcept { m_skipEmptyTokens = shouldSkip; }

    private:
        std::basic_string_view<CharT, Traits> m_text;
        std::basic_string_view<CharT, Traits> m_separators;
        typename std::basic_string_view<CharT, Traits>::size_type m_position = 0;
        bool m_skipEmptyTokens = false;
    };

    using StringSplitter = TStringSplitter<char>;
    using WStringSplitter = TStringSplitter<wchar_t>;
    using U16StringSplitter = TStringSplitter<char16_t>;
    using U32StringSplitter = TStringSplitter<char32_t>;

    /** Trim whitespace characters from the beginning of input. */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    std::basic_string_view<CharT, Traits> leftTrim(const std::basic_string_view<CharT, Traits>& input)
    {
        auto i = input.find_first_not_of(Constants<CharT>::WhitespaceChars);

        if (i == std::basic_string_view<CharT, Traits>::npos)
        {
            return std::basic_string_view<CharT, Traits>();
        }
        else
        {
            return input.substr(i, input.length() - i);
        }
    }

    /** Trim whitespace characters from the end of input. */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    std::basic_string_view<CharT, Traits> rightTrim(const std::basic_string_view<CharT, Traits>& input)
    {
        auto i = input.find_last_not_of(Constants<CharT>::WhitespaceChars);

        if (i == std::basic_string_view<CharT, Traits>::npos)
        {
            return std::basic_string_view<CharT, Traits>();
        }
        else
        {
            return input.substr(0, i + 1);
        }
    }

    /** Trim whitespace characters from the start and end of input. */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    std::basic_string_view<CharT, Traits> trim(const std::basic_string_view<CharT, Traits>& input)
    {
        auto start = input.find_first_not_of(Constants<CharT>::WhitespaceChars);
        auto end = input.find_last_not_of(Constants<CharT>::WhitespaceChars);

        if (start == std::basic_string_view<CharT, Traits>::npos || end == std::basic_string_view<CharT, Traits>::npos)
        {
            return std::basic_string_view<CharT, Traits>();
        }
        else
        {
            return input.substr(start, end - start + 1);
        }
    }

    /** Test if character is white space. (Not unicode compliant!) */
    template<typename CharT>
    bool isWhitespace(CharT c) noexcept
    {
        const auto i = static_cast<int>(c);
        // 32 = space, 9 = tab, 12 = CR, 13 = LF
        return i == 32 || i == 9 || i == 12 || i == 13;
    }

    /** Test if string is white space. (Not unicode compliant!). */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    bool isWhitespace(const std::basic_string_view<CharT, Traits>& input)
    {
        for (const auto& c : input)
        {
            if (!isWhitespace(c))
            {
                return false;
            }
        }

        return true;
    }
}
