#pragma once
#include <string>

namespace Daybreak
{
    /**
     * Reads lines of text from a text source.
     */
    template<typename CharT, class Traits = std::char_traits<CharT>>
    class TLineReader
    {
    public:
        /** Constructor. 
         * \param  text  Text to parse.
         */
        TLineReader(const std::basic_string_view<CharT, Traits>& text) noexcept
            : m_text(text)
        {
        }

        /** Returns the next line from the text. */
        std::basic_string_view<CharT, Traits> readNextLine() noexcept
        {
            const auto start = m_position;
            const auto size = m_text.size();

            auto end = start;

            // Read from the current position until the next newline is found. Once the loop has exited m_position will
            // either be pointing at the first character after the next newline, OR it will be at the end of the text.
            while (m_position < size)
            {
                const auto c = m_text[m_position++];

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
        size_t m_position = 0;
    };

    using LineReader = TLineReader<char>;
    using WLineReader = TLineReader<wchar_t>;
    using U16LineReader = TLineReader<char16_t>;
    using U32LineReader = TLineReader<char32_t>;
}
