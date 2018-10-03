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
    class TLineReader
    {
    public:
        /** Constructor. 
         * \param  text  Text to parse.
         */
        TLineReader(
            const std::basic_string_view<CharT, Traits>& text) noexcept
            : m_text(text)
        {
        }

        /** Constructor.
         * \param  text  Text to parse.
         * \parma  commentChar  Character to treat as starting a comment.
         */
        TLineReader(
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

    using LineReader = TLineReader<char>;
    using WLineReader = TLineReader<wchar_t>;
    using U16LineReader = TLineReader<char16_t>;
    using U32LineReader = TLineReader<char32_t>;

    // TODO: implement skipEmptyTokens + tests
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
        std::basic_string_view<CharT, Traits> readNextToken() noexcept
        {
            // Is the splitter at the end of the text?
            if (!hasNextToken())
            {
                return std::basic_string_view<CharT, Traits>();
            }

            // Find the position of the next unread separator character.
            auto nextSeparatorPosition = m_text.find_first_of(m_separators, m_position);

            // If there are no more separator characters then move the position to the end of the input which will
            // cover all remaining unread characters.
            if (nextSeparatorPosition == std::basic_string_view<CharT, Traits>::npos)
            {
                nextSeparatorPosition = m_text.length();
            }
            
            // Return the next token as the unread text formed by [start, end).
            auto start = m_position;
            auto end = nextSeparatorPosition;

            m_position = nextSeparatorPosition + 1;

            return m_text.substr(start, end - start);
        }

        /** Check if there is another token to be read from the text. */
        bool hasNextToken() const noexcept
        {
            return m_position < m_text.size();
        }

        /** Reset the line reader to the beginning of the input text. */
        void reset() noexcept
        {
            m_position = 0;
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
}
