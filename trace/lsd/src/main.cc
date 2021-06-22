//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "lsd/lsd.h"
#include "lsd/lsdParser.h"
#include "cstring"

//------------------------------------------------------------------------------
int main()
{
    lsd::Stage stage;
    lsd::Parser parser(stage, "files/cornelBox.lsd");
    parser.parse();

    lsd::StageIterator stageIterator(stage);
    stageIterator.begin();
    while(stageIterator.next())
    {
        const lsd::Node & node = *stageIterator;
        stageIterator.recurseIntoChildren(true);
        if(node.isPolyMesh())
        {
            const lsd::PolyMesh * polyMesh = node.as<lsd::PolyMesh>();
            for(size_t i=0; i != polyMesh->m_triangles->size(); ++i)
            {
                lsd::Triangle & triangle = (*polyMesh->m_triangles)[i];
#if 0
                std::cout << triangle.a << std::endl;
                std::cout << triangle.b << std::endl;
                std::cout << triangle.c << std::endl;
#endif
            }
        }
#if 0
        if(model.m_reference != 0)
        {
            std::cout << "  Reference:" << stage.getString(model.m_reference) << std::endl;
        }
#endif
    }
};
