//Copywrite Luke Titley 2015
#ifndef LSDPARSER
#define LSDPARSER

//------------------------------------------------------------------------------
namespace lsd_private
{
    class Parser;
}

//------------------------------------------------------------------------------
namespace lsd
{
class Stage;

//------------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------------
class Parser
{
public:
    Parser(Stage & stage, const char * filename);
    ~Parser();

    void parse();

private:
    lsd_private::Parser * m_parser;
};
} // namespace lsd

#endif
