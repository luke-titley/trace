#include "lsd/lsdParser.h"
#include "src/private_Parser.h"

//------------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------------
lsd::Parser::Parser(Stage & stage, const char * filename):
    m_parser(new lsd_private::Parser(stage, filename))
{}

//------------------------------------------------------------------------------
lsd::Parser:: ~Parser()
{
    delete m_parser;
}

//------------------------------------------------------------------------------
void lsd::Parser::parse()
{
    m_parser->parse();
}
