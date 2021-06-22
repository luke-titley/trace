#include "private_Parser.h"
#include <iostream>
#include <cstring>
#include <cctype>

//------------------------------------------------------------------------------
bool isWhiteSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

//------------------------------------------------------------------------------
bool skipWhiteSpace(std::ifstream & infile, char & c)
{
     while (infile.get(c) && isWhiteSpace(c))
     {
     }

     if (!infile.good())
     {
        return false;
     }
     return true;
}

//------------------------------------------------------------------------------
bool readString(std::ifstream & infile, std::string & str, const char quotation)
{
    char c;
    bool escape = false;
    while(infile.get(c))
    {
        if(!escape)
        {
            if(c == '\\')
            {
                escape = true;
                continue;
            }
            else if(c == quotation)
            {
                return true;
            }
        }
        str.push_back(c);
        escape = false;
    }
    return false;
}

//------------------------------------------------------------------------------
bool isSpecial(const char c)
{
    switch(c)
    {
        case '{':
        case '}':
        case ')':
        case '(':
        case '[':
        case ']':
        case '=':
        case ',':
            return true;
        default:
            break;
    }
    return false;
}

//------------------------------------------------------------------------------
void readWord(std::ifstream & infile, char & c, std::string & str)
{
    str.push_back(c);
    while (infile.get(c) && !isWhiteSpace(c))
    {
        if(isSpecial(c))
        {
            infile.putback(c);
            break;
        }
        str.push_back(c);
    }
}

//------------------------------------------------------------------------------
bool readNumber(std::ifstream & infile,
                char & c,
                std::string & str,
                bool & isFloat)
{
    bool hasNegative = false;
    str.push_back(c);
    while (infile.get(c))
    {
        if(c == '.')
        {
            if(isFloat) // We've already come across a decimal point, 
            {           // we can't have two.
                infile.putback(c);
                break;
            }
            isFloat = true;
        }
        else if(c == '-')
        {
            if(hasNegative)
            {
                infile.putback(c);
                return true;
            }
            hasNegative = true;
        }
        else if(!isdigit(c))
        {
            infile.putback(c);
            return true;
        }
        str.push_back(c);
    }
    return false;
}

//------------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------------
lsd_private::Parser::Parser(lsd::Stage & stage, const char * filename):
    m_stage(stage)
{
    m_infile.open(filename);
}

//------------------------------------------------------------------------------
int lsd_private::Parser::lex()
{
    char c;
    if(skipWhiteSpace(m_infile, c))
    {
        // Braces
        if(isSpecial(c))
        {
            return c;
        }

        // Strings
        m_buffer.clear();
        if(c == '"')
        {
            if(!readString(m_infile, m_buffer, '"'))
            {
                return 0;
            }
            d_val__.m_string = m_stage.allocArray<char>(m_buffer.size()+1);
            strcpy(d_val__.m_string, m_buffer.c_str());
            return STRING;
        }
        // Asset path
        else if(c == '@')
        {
            if(!readString(m_infile, m_buffer, '@'))
            {
                return 0;
            }
            d_val__.m_string = m_stage.allocArray<char>(m_buffer.size()+1);
            strcpy(d_val__.m_string, m_buffer.c_str());
            return ASSET;
        }
        // Number
        else if(isdigit(c) || c == '-')
        {
            bool isFloat = false;
            if(!readNumber(m_infile, c, m_buffer, isFloat))
            {
                return 0;
            }
            if(isFloat)
            {
                d_val__.m_double = atof(m_buffer.c_str());
                return DOUBLE;
            }
            else
            {
                d_val__.m_int = atoi(m_buffer.c_str());
                return INT;
            }
        }

        // Keywords and Identifiers
        else
        {
            readWord(m_infile, c, m_buffer);

#if 0
            std::cout << "Token: "  << m_buffer << std::endl;
#endif
            if(m_buffer == "def")
            {
                return DEF;
            }
            else if(m_buffer == "Xform")
            {
                return XFORM;
            }
            else if(m_buffer == "PolyMesh")
            {
                return POLYMESH;
            }
            else if(m_buffer == "verticies")
            {
                return VERTICIES;
            }
            else if(m_buffer == "triangles")
            {
                return TRIANGLES;
            }
            else if(m_buffer == "Triangle")
            {
                return TRIANGLE;
            }
            else if(m_buffer == "Point")
            {
                return POINT;
            }

            d_val__.m_string = m_stage.allocArray<char>(m_buffer.size()+1);
            strcpy(d_val__.m_string, m_buffer.c_str());

            return IDENTIFIER;
        }
    }
    return 0;
}

